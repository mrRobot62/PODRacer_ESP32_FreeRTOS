#include "HoverTask.h"

void HoverTask(void *parameter) {

  TDataHover hoverData;
  HoverController *controller;

  controller = new HoverController();

  while(1)

    if (xQueueReceive(queueHover, &hoverData, 0) == pdPASS) {
      // beachten - alle berechnungen werden im Controller durchgeführt. Das macht den eigentlichen Task übersichtlicher.
      // berechnete Daten werden direkt in hoverData gespeichert
      controller->read(hoverData);

    }

    vTaskDelay(LOOP_TIME / portTICK_PERIOD_MS);
}