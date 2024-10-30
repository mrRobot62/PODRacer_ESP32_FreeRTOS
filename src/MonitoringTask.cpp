#include "MonitoringTask.h"

void monitoringTask(void* parameter) {
    const TickType_t xDelay = pdMS_TO_TICKS(30000); // 30 Sekunden in Ticks
    while (true) {
        // Hole die Heap-Größe (Gesamt- und freien Speicher)
        size_t freeHeap = xPortGetFreeHeapSize();
        size_t minFreeHeap = xPortGetMinimumEverFreeHeapSize();
        // Ausgabe des Heap-Speichers
        Serial.println("===== System Monitoring =====");
        Serial.print("Freier Heap-Speicher: ");
        Serial.print(freeHeap);
        Serial.println(" Bytes");

        Serial.print("Min. freier Heap-Speicher: ");
        Serial.print(minFreeHeap);
        Serial.println(" Bytes");

        // Warten, bis die 30 Sekunden abgelaufen sind
        vTaskDelay(xDelay);
    }
}
