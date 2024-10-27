#include "HoverTask.h"

void HoverTask(void *parameter) {

  TDataHover hoverData;


  while(1)

    if (xQueueReceive(queueHover, &hoverData, 0) == pdPASS) {
        
    }

    vTaskDelay(LOOP_TIME / portTICK_PERIOD_MS);
}