#include <Arduino.h>
#include <HardwareSerial.h>
#include "ReceiverTask.h"
#include "ReceiverSBUS.h"
#include "MockReceiverSBUS.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xMutex;
TDataRC queueRCData;

#define SBUS_RX 16
#define SBUS_TX 17

void receiverTask(void *parameter) {
  // Beispiel: Setup für SBUS Empfang (angepasst auf deinen Empfänger)
  HardwareSerial sbusSerial(2); // UART2 für SBUS
  sbusSerial.begin(100000);

  HardwareSerial hsBus2(2);       // used by Receiver for SBUS
  
  // nur das Interface wird als Pointer genutzt
  // dadurch haben wir die Möglichkeit problemlos auch Mock-Klassen zu compilieren
  IReceiver *receiver;

  // lokaler mutex um während des auslesens des SBUS-Receiver einen 
  // lock setzen
  SemaphoreHandle_t xReceiverMutex;
  xReceiverMutex = xSemaphoreCreateMutex();
  
  //
  // lokaler Datenspeicher für RC-daten
  TDataRC *rcData;
  TDataHover *hoverData;

  Serial.println("Receiver running...");
  #ifdef USE_MOCK_SBUS
    receiver = new MockReceiverSBUS(MOCK_DATA_MASK_SBUS); // in main.cpp kann nach bedarf MOCK_DATA_MASK_SBUS angepasst werden
  #else
    receiver = new ReceiverSBUS(&hsBus2, SBUS_RX, SBUS_TX );
  #endif
  while (1) {
    rcData = new TDataRC();
    hoverData = new TDataHover();

    //-----------------------------------------------------------------------------------------
    // --- READ - SBUS Daten 
    //-----------------------------------------------------------------------------------------
    // immer den Receiver auslesen
    //Serial.printf("RECEIVER rcData: %d, %d, %d", rcData->gimbal_max, rcData->gimbal_mid, rcData->gimbal_min);
    receiver->read(rcData);

    // -- arming/preventarming analysieren
    if (rcData->raw_channels[ARMING]) bitSet(rcData->recvStatus, BIT0);
//    if (compare2Bits(rcData->recvStatus, BIT0, BIT2))
    // raw_channel daten in Hover-Struktur kopieren
    memcpy(hoverData->raw_channels, rcData->raw_channels, sizeof(hoverData->raw_channels));
    //
    // wenn Mutex verfügbar, dann in Queue schreiben
    if (xSemaphoreTake(xReceiverMutex, (TickType_t)10) == pdTRUE) {
      //--------------------------------------------------------------
      // aktuelle Daten in Queues senden
      //--------------------------------------------------------------
      xQueueSend(queueReceiver, rcData, portMAX_DELAY);
      xQueueSend(queueHover, hoverData, portMAX_DELAY);
      
      // Mutex freigeben
      xSemaphoreGive(xReceiverMutex);
      if (bitRead(LOG_MASK_RECEIVER, LOGGING_BIT) && bitRead(LOG_MASK_RECEIVER, LOGGING_RECV_READ)) {
        logger->info(*rcData, "RECV", "READ");
      }
    }    

    //-----------------------------------------------------------------------------------------
    // --- WRITE - SBUS Daten 
    //-----------------------------------------------------------------------------------------
    // reset rcData
    memset(rcData, 0, sizeof(&rcData));
    // ab hier beginnt der WRITE-Teil des ReceiverTasks
    // wir prüfen ob ein Event-Bit vom Mixer gesetzt wurde, wenn ja, dann
    // liest der ReceiverTask die global TDataAll Struktur und schreibt die Daten
    // über SBUS.write() zum FlightController

    EventBits_t uxBits = xEventGroupGetBits(xEventGroup);
    // Prüfen, ob das spezifische BIT_RECEIVER gesetzt ist
    if (uxBits & BIT_RECEIVER) {  
      if (xSemaphoreTake(xTDataAllMutex, (TickType_t)10) == pdTRUE) {
          receiver->write(&globalData.rc);
          xSemaphoreGive(xTDataAllMutex);
          if (bitRead(LOG_MASK_RECEIVER, LOGGING_BIT) && bitRead(LOG_MASK_RECEIVER, LOGGING_RECV_WRITE)) {
            logger->info(globalData.rc, "RECV", "WRITE");
          }
          // Lösche das Ereignisbit, nachdem die Aktion abgeschlossen ist
          xEventGroupClearBits(xEventGroup, BIT_RECEIVER);
       }
    }
    delete(rcData);
    delete(hoverData);

    vTaskDelay(LOOP_TIME / portTICK_PERIOD_MS);
  }
}
