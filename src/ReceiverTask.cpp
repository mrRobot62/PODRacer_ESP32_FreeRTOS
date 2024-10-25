#include <Arduino.h>
#include <HardwareSerial.h>
#include "ReceiverTask.h"
#include "ReceiverSBUS.h"
#include "MockReceiverSBUS.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xMutex;
TDataRC data;

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

  memset(&data, 0, sizeof(data));
  while (1) {
    #ifdef USE_MOCK_SBUS
      receiver = new MockReceiverSBUS();
    #else
      receiver = new ReceiverSBUS(&hsBus2, SBUS_RX, SBUS_TX );
    #endif

      receiver->read(&data);
      logger->info(data, "RECV", "READ");

    // if (sbusSerial.available()) {
    //   if (xSemaphoreTake(xMutex, (TickType_t)10) == pdTRUE) {
    //     // Empfange SBUS-Daten und schreibe in die globale RECV-Struktur
    //       memset(&data, 0, sizeof(data));
    //       receiver->read(&data);
    //       logger->info(data);
    //     }
    //     xSemaphoreGive(xMutex);
    //   }

    // }
    
    Serial.println("Receiver running...");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
