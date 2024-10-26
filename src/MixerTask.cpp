#include <Arduino.h>
#include "MixerTask.h"
#include "freertos/queue.h"


QueueHandle_t queueMixer;

void mixerTask(void *parameter) {
  // Queue Daten die empfangen werden können.
  // Jeder Task sendet seine Daten in eine eigene Queue

  TDataRC rcData;   
  TDataSurface surfaceData;
  TDataOFlow oFlowData;
  TDataHover hoverData;
  TDataSteering steeringData;

  //
  // es wird ein globaler Mutex genutzt den der Mixer nutzt
  // um das während dem Schreiben in TDataAll einen Lock zu setzen
  //
  // globaler Mutex: xTDataAllMutex
  xTDataAllMutex = xSemaphoreCreateMutex();

  Serial.println("Mixer running...");

  while (1) {
    // Der Mixer liest zuerst alle Queues aus und speichert die
    // Daten lokal in seinen Structs, anschließend beginner er die Daten
    // zu verarbeitet.
    // 
    // am Ende führt er ein Update der globael TDataAll Struktur durch
    // und setzt folgende EventBits: BIT_RECEIVER | BIT_WEBSERVER
    //
    if (xQueueReceive(queueReceiver, &rcData, 0) == pdPASS) {
        xSemaphoreTake(xTDataAllMutex, portMAX_DELAY);
        globalData.rc = rcData;
        xSemaphoreGive(xTDataAllMutex);
    }
    // nur für test
    memcpy(rcData.new_channels, rcData.raw_channels, sizeof(rcData.new_channels));
    logger->info(rcData, "MIXER","S1");

    if (xQueueReceive(queueSurface, &surfaceData, 0) == pdPASS) {
        xSemaphoreTake(xTDataAllMutex, portMAX_DELAY);
        globalData.sdist = surfaceData;
        xSemaphoreGive(xTDataAllMutex);
    }

    if (xQueueReceive(queueHover, &hoverData, 0) == pdPASS) {
      xSemaphoreTake(xTDataAllMutex, portMAX_DELAY);
      globalData.hover = hoverData;
      xSemaphoreGive(xTDataAllMutex);
    }   



    // zum Schreiben in TDataAll wird ein Mutex benötigt um ein 
    // unbeabsichtes Lesen während des Schreibens zu vermeiden
    //
    if (xSemaphoreTake(xTDataAllMutex, (TickType_t)10) == pdTRUE) {
        globalData.hover = hoverData;
        globalData.sdist = surfaceData;
        globalData.oflow = oFlowData;
        globalData.steering = steeringData;
        globalData.rc = rcData ;
        xSemaphoreGive(xTDataAllMutex);
    }    

    // Setze die Ereignisbits, um die anderen Tasks zu benachrichtigen
    xEventGroupSetBits(xEventGroup, BIT_RECEIVER | BIT_WEBSERVER);

    // if (xQueueSend(sbusQueue, &sbusData, portMAX_DELAY) == pdPASS) {
    //     logger->log("SBUS data sent to Queue");
    // }    
    vTaskDelay(LOOP_TIME / portTICK_PERIOD_MS);
  }
}
