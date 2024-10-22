#include <Arduino.h>
#include <HardwareSerial.h>
#include "ReceiverTask.h"
#include "freertos/semphr.h"
#include "RECVStruct.h"  // Dies stellt sicher, dass die RECV_Struct-Definition bekannt ist

// Globale RECV-Struktur extern einbinden
extern struct RECV_Struct RECV;

SemaphoreHandle_t xMutex;



void receiverTask(void *parameter) {
  // Beispiel: Setup für SBUS Empfang (angepasst auf deinen Empfänger)
  HardwareSerial sbusSerial(2); // UART2 für SBUS
  sbusSerial.begin(100000);


  while (1) {
    if (sbusSerial.available()) {
      if (xSemaphoreTake(xMutex, (TickType_t)10) == pdTRUE) {
        // Empfange SBUS-Daten und schreibe in die globale RECV-Struktur
        for (int i = 0; i < 16; i++) {
          RECV.channel[i] = sbusSerial.read();  // Beispiel-Daten (echte Dekodierung implementieren)
        }
        RECV.armingAllowed = true;
        RECV.isArmed = false;
        xSemaphoreGive(xMutex);
      }
    }
    
    Serial.println("Receiver running...");
    vTaskDelay(2500 / portTICK_PERIOD_MS);
  }
}
