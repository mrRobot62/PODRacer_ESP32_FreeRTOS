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

  while (!generalFreeRTOSError) // bei globalen Fehlern wird der Task abgebrochen
  {
    // Der Mixer liest zuerst alle Queues aus und speichert die
    // Daten lokal in seinen Structs, anschließend beginnt er die Daten
    // zu verarbeitet.
    //
    // am Ende führt er ein Update der globael TDataAll Struktur durch
    // und setzt folgende EventBits: BIT_RECEIVER | BIT_WEBSERVER
    //
    // Receiver und Webserver lesen ihre Daten aus der globalen Struktur
    // Hintergrund: das ist einfacher, als zwei Objekte in eine Queue für RECV und ein Obj. für WEBServer zu setzen
    //              weniger overhead
    //

    // ---------------------------------------------------------------------------
    // ReceiverTask auslesen
    // ---------------------------------------------------------------------------
    if (xQueueReceive(queueReceiver, &lokalRCData, 0) == pdPASS)
    {
      // xSemaphoreTake(xTDataAllMutex, MUTEX_WAIT_TIMEOUT);
      // An dieser Stelle ist ein Mutex nicht notwendig, da der Mixer aktuell der einzige Task ist, der verändern kann
      // l
      lokalDataAll.rc = lokalRCData;
      // xSemaphoreGive(xTDataAllMutex);
    }
    else
    {
      if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_R1))
      {
        logger->message("No QUEUE-Data from receiver", WARN, millis(), "MIXER", "READ");
      }
    }
    // BEACHTEN: über den IF-Konstrukt steuert ob überhaupt eine Logausgabe stattfinden kann
    // nachfolgende ausgabe ist nur für Testzwecke gedacht
    if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_R1))
    {
      memcpy(lokalRCData.new_channels, lokalRCData.raw_channels, sizeof(lokalRCData.new_channels));
      logger->info(lokalRCData, millis(), "MIXER", "R_IN");
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
      memcpy(&globalData.hover, &lokalDataAll.hover, sizeof(lokalDataAll.hover));
      memcpy(&globalData.sdist, &lokalDataAll.sdist, sizeof(lokalDataAll.sdist));
      memcpy(&globalData.oflow, &lokalDataAll.oflow, sizeof(lokalDataAll.oflow));
      memcpy(&globalData.steering, &lokalDataAll.steering, sizeof(lokalDataAll.steering));
      memcpy(&globalData.status, &lokalDataAll.status, sizeof(lokalDataAll.status));
      memcpy(&globalData.rc, &lokalDataAll.rc, sizeof(lokalDataAll.rc));

      if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_WRITE))
      {
        // Serial.println("xxxxxxx");
        logger->info(globalData.rc, millis(), "MIXER", "OUT");
      }
      xSemaphoreGive(xTDataAllMutex);
    }
    else
    {
      if (CHECK_BIT(LOG_MASK_MIXER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_MIXER, LOGGING_MIX_WRITE))
      {
        logger->message("xTDataAllMutex failed", WARN, millis(), "MIXER", "WRITE");
      }
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
