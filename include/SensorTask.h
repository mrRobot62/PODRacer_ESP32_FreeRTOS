#ifndef _SENSOR_TASK_H_
#define _SENSOR_TASK_H_

#include <Arduino.h>
#include "data_struct.h"
#include "SensorWrapperSingleton.h"
#include "Logger.h"

void sensorTask(void *parameter);

#endif