#include <Arduino.h>
#include "BlinkTask.h"
#include "freertos/queue.h"


QueueHandle_t queueBlink;

void blinkTask(void *parameter) {
  BlinkController * controller;
  int receivedBlinkPattern;
  int newPattern = 0;
  while (1) {
    queueBlink = xQueueCreate(10, sizeof(int));
    if (queueBlink == NULL) {
        Serial.println("Failed to create queue in webServerTask");
        vTaskDelete(NULL);
        return;
    }

    if (xQueueReceive(queueBlink, &receivedBlinkPattern, (TickType_t)10)) {
      // Daten periodisieren und in RECV-Struct schreiben
      newPattern = receivedBlinkPattern; // Beispiel-Kanal
      Serial.printf("Received Blink pattern: %d\n", receivedBlinkPattern);
    }
    Serial.println("Blink running");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}