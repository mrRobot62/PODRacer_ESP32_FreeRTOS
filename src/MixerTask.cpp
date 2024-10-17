#include <Arduino.h>
#include "MixerTask.h"
#include "freertos/queue.h"
#include "RECVStruct.h"  // Dies stellt sicher, dass die RECV_Struct-Definition bekannt ist

// Globale RECV-Struktur extern einbinden
extern struct RECV_Struct RECV;

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
      RECV.channel[0] = receivedDistance; // Beispiel-Kanal
      Serial.printf("Received Distance: %d\n", receivedDistance);
    }
    Serial.println("Mixer running");
    vTaskDelay(4500 / portTICK_PERIOD_MS);
  }
}
