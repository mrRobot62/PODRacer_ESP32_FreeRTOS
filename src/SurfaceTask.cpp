#include <Arduino.h>
#include <HardwareSerial.h>
#include "SurfaceTask.h"
#include "data_struct.h"
#include "Utils.h"
#include <PID_v1.h>
#include <TFMini.h>
#include "freertos/queue.h"

// Globale RECV-Struktur extern einbinden
extern struct RECV_Struct RECV;

TFMini tfmini;
QueueHandle_t queueDistance;

void surfaceTask(void *parameter) {

  double distance;
    queueDistance = xQueueCreate(10, sizeof(int));
    if (queueDistance == NULL) {
        Serial.println("Failed to create queue in webServerTask");
        vTaskDelete(NULL);
        return;
    }

  while (1) {
    //distance = tfmini.getDistance() ;
    if (distance > 0) {
      // Distance in Queue legen
      if (xQueueSend(queueDistance, (void *)&distance, (TickType_t)10) != pdPASS) {
        Serial.println("Queue send error");
      }
    }
    //Serial.println("Surface running");
    vTaskDelay(pdMS_TO_TICKS(LOOP_TIME));
  }
}
