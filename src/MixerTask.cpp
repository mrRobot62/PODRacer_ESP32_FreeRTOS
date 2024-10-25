#include <Arduino.h>
#include "MixerTask.h"
#include "freertos/queue.h"


QueueHandle_t queueMixer;

void mixerTask(void *parameter) {
  int receivedDistance;
  while (1) {
    queueMixer = xQueueCreate(10, sizeof(int));
    if (queueMixer == NULL) {
        Serial.println("Failed to create queue in webServerTask");
        vTaskDelete(NULL);
        return;
    }

    if (xQueueReceive(queueMixer, &receivedDistance, (TickType_t)10)) {
      // Daten periodisieren und in RECV-Struct schreiben
      Serial.printf("Received Distance: %d\n", receivedDistance);
    }
    Serial.println("Mixer running");
    vTaskDelay(4500 / portTICK_PERIOD_MS);
  }
}
