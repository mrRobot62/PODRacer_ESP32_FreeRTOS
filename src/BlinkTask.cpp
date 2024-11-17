#include <Arduino.h>
#include "BlinkTask.h"
#include "globals.h"
#include "data_struct.h"
#include "freertos/queue.h"

QueueHandle_t queueBlink;

// Funktion zum Lesen der Bitmaske mit Mutex-Schutz
void readBitmask(uint8_t *bitmask)
{
  if (xSemaphoreTake(xbitmaskBlinkMutex, portMAX_DELAY) == pdTRUE)
  {
    memcpy(bitmask, blink_mask, sizeof(blink_mask));
    xSemaphoreGive(xbitmaskBlinkMutex);
  }
}

void blinkTask(void *parameter)
{
  uint8_t ledPin = ((uint32_t *)parameter)[0];
  uint8_t ledIndex = ((uint32_t *)parameter)[1];
  uint8_t bitControl = ((uint32_t *)parameter)[2];

  // GPIO-Modus für die LEDs setzen
  pinMode(LED_STATE, OUTPUT);
  pinMode(LED_ERR1, OUTPUT);
  pinMode(LED_ERR2, OUTPUT);

  while (1)
  {

    // Bitmaske lesen
    uint8_t localBitmask[3];
    readBitmask(localBitmask);

    // Prüfen, ob die LED aktiviert ist
    if (localBitmask[ledIndex] & bitControl)
    {
      // Frequenz bestimmen
      uint32_t blinkDelay = 0;
      if (localBitmask[ledIndex] & 0x01)
        blinkDelay = 50;
      if (localBitmask[ledIndex] & 0x02)
        blinkDelay = 200;
      if (localBitmask[ledIndex] & 0x04)
        blinkDelay = 500;
      if (localBitmask[ledIndex] & 0x08)
        blinkDelay = 1000;

      if (blinkDelay > 0)
      {
        digitalWrite(ledPin, HIGH);
        vTaskDelay(pdMS_TO_TICKS(blinkDelay));
        digitalWrite(ledPin, LOW);
        vTaskDelay(pdMS_TO_TICKS(blinkDelay));
      }
    }
    else
    {
      // LED ausschalten, falls deaktiviert
      digitalWrite(ledPin, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}