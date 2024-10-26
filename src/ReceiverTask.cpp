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
  TDataRC rcData;

  Serial.println("Receiver running...");
  #ifdef USE_MOCK_SBUS
    receiver = new MockReceiverSBUS();
  #else
    receiver = new ReceiverSBUS(&hsBus2, SBUS_RX, SBUS_TX );
  #endif
  while (1) {
    // immer den Receiver auslesen
    receiver->read(&rcData);
    //
    // wenn Mutex verfügbar, dann in Queue schreiben
    if (xSemaphoreTake(xReceiverMutex, (TickType_t)10) == pdTRUE) {
      // Daten in Queue senden
      xQueueSend(queueReceiver, &rcData, portMAX_DELAY);
      // Mutex freigeben
      xSemaphoreGive(xReceiverMutex);
      logger->info(rcData, "RECV", "READ");
    }    
    // reset rcData
    memset(&rcData, 0, sizeof(rcData));
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
          logger->info(globalData.rc, "RECV", "WRITE");
          // Lösche das Ereignisbit, nachdem die Aktion abgeschlossen ist
          xEventGroupClearBits(xEventGroup, BIT_RECEIVER);
       }
    }
    vTaskDelay(LOOP_TIME / portTICK_PERIOD_MS);
  }
}
