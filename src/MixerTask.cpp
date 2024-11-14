#include <Arduino.h>
#include "Utils.h"
#include "MixerTask.h"
#include "freertos/queue.h"

QueueHandle_t queueMixer;

void mixerTask(void *parameter)
{
  //
  // Der Mixer speichert alle Task-Daten im ersten Schritt in lokalen Variablen.
  // Diese Daten werden dann ausgewertet und dann gesichert in die globale Struktur TDataAll zurück geschrieben
  //
  //

  TDataAll lokalDataAll;

  TDataRC lokalRCData;
  TDataSurface lokalSurfaceData;
  TDataOFlow lokalOFlowData;
  TDataHover lokalHoverData;
  TDataSteering lokalSteeringData;
  //
  // es wird ein globaler Mutex genutzt den der Mixer nutzt
  // um das während dem Schreiben in TDataAll einen Lock zu setzen
  //
  // globaler Mutex: xTDataAllMutex, wird zum Sperren der TDataAll Struktur genutzt
  xTDataAllMutex = xSemaphoreCreateMutex();

  Serial.println("Mixer running...");

  while (1)
  {
    // Der Mixer liest zuerst alle Queues aus und speichert die
    // Daten lokal in seinen Structs, anschließend beginnt er die Daten
    // zu verarbeitet.
    //
    // am Ende führt er ein Update der globael TDataAll Struktur durch
    // und setzt folgende EventBits: BIT_RECEIVER | BIT_WEBSERVER
    //

    // ---------------------------------------------------------------------------
    // ReceiverTask auslesen
    // ---------------------------------------------------------------------------
    if (xQueueReceive(queueReceiver, &lokalRCData, 0) == pdPASS)
    {
      // xSemaphoreTake(xTDataAllMutex, MUTEX_WAIT_TIMEOUT);
      lokalDataAll.rc = lokalRCData;
      // xSemaphoreGive(xTDataAllMutex);
    }
    else {
      if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_R1))
      {
        logger->message("No QUEUE-Data from receiver", WARN, millis(), "MIXER","READ");
      }
    }
    // BEACHTEN: über den IF-Konstrukt wird gesteuert ob überhaupt eine Logausgabe stattfinden kann
    // nachfolgende ausgabe ist nur für Testzwecke gedacht
    if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_R1))
    {
      memcpy(lokalRCData.new_channels, lokalRCData.raw_channels, sizeof(lokalRCData.new_channels));
      logger->info(lokalRCData, millis(), "MIXER", "R_RC");
    }

    // ---------------------------------------------------------------------------
    // HoverTask auslesen
    // ---------------------------------------------------------------------------
    // if (xQueueReceive(queueHoverSend, &lokalHoverData, 0) == pdPASS)
    // {
    //   xSemaphoreTake(xTDataAllMutex, MUTEX_WAIT_TIMEOUT);
    //   lokalDataAll.hover = lokalHoverData;
    //   xSemaphoreGive(xTDataAllMutex);
    // }

    // // nur für test
    // if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_R2))
    // {
    //   logger->info(lokalHoverData, millis(), "MIXER", "R_HOV");
    // }

    // ---------------------------------------------------------------------------
    // SurfaceDistTask auslesen
    // ---------------------------------------------------------------------------
    // if (xQueueReceive(queueSurface, &lokalSurfaceData, 0) == pdPASS)
    // {
    //   xSemaphoreTake(xTDataAllMutex, MUTEX_WAIT_TIMEOUT);
    //   lokalDataAll.sdist = lokalSurfaceData;
    //   xSemaphoreGive(xTDataAllMutex);
    // }

    // if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_R3))
    // {
    //   logger->info(lokalSurfaceData, millis(), "MIXER", "R_SRF");
    // }

    // ---------------------------------------------------------------------------
    // Ergebnisse in globale TDataAll zurück schreiben
    // ---------------------------------------------------------------------------
    // zum Schreiben in TDataAll wird ein Mutex benötigt um ein
    // unbeabsichtes Lesen während des Schreibens zu vermeiden
    //
    // Beispiel:
    // Recv:           101
    // Hover:          001
    // or:                 101

    // if (B1=1 AND B2=1) B0=0,B2=1
    // else B1=0, B2=0

    // => GlobalSet: 001 (Rest B2)
    // => RecvWrite 001

    if (xSemaphoreTake(xTDataAllMutex, (TickType_t)10) == pdTRUE)
    {
      //
      // WENN ChangeBit und PreventBit gesetzt sind, bedeutet das ,das
      // (a) der Receiver den Status verändert hat
      // (b) ein Task das PreventBit gesetzt hat
      // DANN damit löschen das ArmingBit und setzen explizit das ChangeBit
      // ELSE PreventBit löschen, ChangeBit löschen. Der HoverTask setzt das PreventBit grundsätzlich, wenn RPY <> mid und THR/TRU <> min sind
      //      nur im Zusammenhang mit dem changeBit wird ein tatsächliches preventArming ausgelöst
      if (CHECK_BIT(lokalDataAll.status.status_pattern, PREVENT_BIT) && CHECK_BIT(lokalDataAll.status.status_pattern, CHANGE_BIT))
      {
        // Serial.println("--- STATUS a (Problem) ---");
        CLEAR_BIT(lokalDataAll.status.status_pattern, ARMING_BIT);
        SET_BIT(lokalDataAll.status.status_pattern, PREVENT_BIT);
        SET_BIT(lokalDataAll.status.status_pattern, CHANGE_BIT);
        SET_BIT(lokalDataAll.status.error_pattern, BIT7); // Error PreventArming
      }
      else
      {
        // Serial.println("--- STATUS b (OK) ---");
        SET_BIT(lokalDataAll.status.status_pattern, ARMING_BIT);
        CLEAR_BIT(lokalDataAll.status.status_pattern, PREVENT_BIT);
        CLEAR_BIT(lokalDataAll.status.error_pattern, BIT7);
        CLEAR_BIT(lokalDataAll.status.status_pattern, CHANGE_BIT);
      }

      globalData.status = lokalDataAll.status;
      globalData.hover = lokalDataAll.hover;
      globalData.sdist = lokalDataAll.sdist;
      globalData.oflow = lokalDataAll.oflow;
      globalData.steering = lokalDataAll.steering;
      globalData.rc = lokalDataAll.rc;

      if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_WRITE))
      {
        logger->info(globalData, millis(), "MIXER", "WRITE");
      }
      xSemaphoreGive(xTDataAllMutex);
    }

    // Setze die Ereignisbits, um die anderen Tasks zu benachrichtigen
    xEventGroupSetBits(xEventGroup, BIT_RECEIVER | BIT_WEBSERVER);

    if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_STAT))
    {
      logger->info(globalData.status, millis(), "MIXER", "GSTAT");
    }

    // if (xQueueSend(sbusQueue, &sbusData, portMAX_DELAY) == pdPASS) {
    //     logger->log("SBUS data sent to Queue");
    // }
    vTaskDelay(pdMS_TO_TICKS(LOOP_TIME));
  }
}
