#include <Arduino.h>
#include "BlinkTask.h"
#include "globals.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

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

// void updateMaskBlinkPattern(uint8_t newMask, uint8_t ledIndex)
// {
//   // Zugriff auf die Bitmaske mit Mutex schützen
//   if (xSemaphoreTake(xbitmaskBlinkMutex, portMAX_DELAY) == pdTRUE)
//   {
//     blink_mask[ledIndex] = newMask;
//     xSemaphoreGive(xbitmaskBlinkMutex);
//   }
// }

void _doBlink(uint8_t pin, uint8_t ms)
{
  digitalWrite(pin, HIGH);
  vTaskDelay(pdMS_TO_TICKS(ms));
  digitalWrite(pin, LOW);
  vTaskDelay(pdMS_TO_TICKS(ms));
}

void blinkTask(void *parameter)
{
  uint8_t ledPin = ((uint32_t *)parameter)[0];
  uint8_t ledIndex = ((uint32_t *)parameter)[1];
  uint8_t bitMask = ((uint32_t *)parameter)[2];

  // GPIO-Modus für die LEDs setzen
  pinMode(LED_STATE, OUTPUT);
  pinMode(LED_ERR01, OUTPUT);
  pinMode(LED_ERR02, OUTPUT);

  while (1)
  {

    // Bitmaske lesen
    uint8_t localBitmask[3];
    readBitmask(localBitmask);

    // Prüfen, ob die LED aktiviert ist
    if (localBitmask[ledIndex] & bitMask)
    {
      // Frequenz bestimmen
      uint32_t blinkDelay = 0;
      if (localBitmask[ledIndex] & 0x01) // BIT 0
        blinkDelay += 50;
      if (localBitmask[ledIndex] & 0x02) // BIT 1
        blinkDelay += 200;
      if (localBitmask[ledIndex] & 0x04) // BIT 2
        blinkDelay += 500;
      if (localBitmask[ledIndex] & 0x08) // BIT 3
        blinkDelay += 1000;

      if (blinkDelay > 0)
      {
        if (CHECK_BIT(localBitmask[0], BIT7) ||
            CHECK_BIT(localBitmask[1], BIT7) ||
            CHECK_BIT(localBitmask[2], BIT7))
        {
          _doBlink(LED_STATE, blinkDelay);
          _doBlink(LED_ERR01, blinkDelay);
          _doBlink(LED_ERR02, blinkDelay);
        }
        else
        {
          // 0x80, 0x40, 0x20, 0x10 - dann abbruch (128,64,32,16)
          for (uint8_t bit = 0x80; bit >= 0x10; bit /= 2)
          {
            if (CHECK_BIT(localBitmask[0], bit))
              _doBlink(LED_STATE, blinkDelay);
            if (CHECK_BIT(localBitmask[1], bit))
              _doBlink(LED_ERR01, blinkDelay);
            if (CHECK_BIT(localBitmask[2], bit))
              _doBlink(LED_ERR02, blinkDelay);
          }
        }
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