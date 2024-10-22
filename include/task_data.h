#ifndef _TASK_DATA_H_
#define _TASK_DATA_H_

#include <Arduino.h>

#define NUMBER_CHANNELS 16
#define DATA_SIZE 8

// this struct is used to store data from tasks and transfer data from task to task and into Blackbox
// or transfer data via WIFI to host computer
typedef struct {

} TaskDataStruct;

typedef union {                   
  TaskDataStruct data;                  // the data struct above
  byte bytes [sizeof(TaskDataStruct)];  // Now we know how many bytes are stored
} TaskData;                                  // this union is used to store data onto SD-Card or transport via WIFI. 



#endif