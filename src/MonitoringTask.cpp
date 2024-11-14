#include "MonitoringTask.h"


TESP32Memory memory;

void monitoringTask(void* parameter) {
    while (true) {
        // Hole die Heap-Größe (Gesamt- und freien Speicher)
        memory.freeHeap = xPortGetFreeHeapSize();
        memory.minFreeHeap = xPortGetMinimumEverFreeHeapSize();

        logger->info(memory, millis());

        // Warten, bis die 30 Sekunden abgelaufen sind
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
