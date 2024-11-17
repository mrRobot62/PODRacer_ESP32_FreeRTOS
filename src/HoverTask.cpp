#include "HoverTask.h"

void hoverTask(void *parameter)
{

  TDataHover lokalHoverData;
  HoverController *controller;

  controller = new HoverController();

  while (!generalFreeRTOSError) // bei globalen Fehlern wird der Task abgebrochen

    if (xQueueReceive(queueHoverRcv, &lokalHoverData, 0) == pdPASS)
    {
      // beachten - alle berechnungen werden im Controller durchgeführt. Das macht den eigentlichen Task übersichtlicher.
      // berechnete Daten werden direkt in lokalHoverData gespeichert
      controller->read(lokalHoverData);
      if (CHECK_BIT(LOG_MASK_HOVER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_HOVER, LOGGING_HOVER_READ))
        logger->info(lokalHoverData, millis(), "HOVER", "QUE_R");
    }
  xQueueSend(queueHoverSend, &lokalHoverData, portMAX_DELAY);
  if (CHECK_BIT(LOG_MASK_HOVER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_HOVER, LOGGING_HOVER_SEND))
    logger->info(lokalHoverData, millis(), "HOVER", "QUE_S");
  vTaskDelay(pdMS_TO_TICKS(LOOP_TIME));
}