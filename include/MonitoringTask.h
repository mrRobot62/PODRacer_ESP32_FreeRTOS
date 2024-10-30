#ifndef MONITORING_TASK_H
#define MONITORING_TASK_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Funktion zum Starten des Monitoring-Tasks
void monitoringTask(void *parameter);

#endif // MONITORING_TASK_H
