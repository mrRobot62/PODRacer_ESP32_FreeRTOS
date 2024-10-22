#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <Arduino.h>

#define NUMBER_OF_CHANNELS 16

enum {OFF, ON};

typedef struct __attribute__ ((packed)) {
  uint16_t channels[NUMBER_OF_CHANNELS] = {0};
  uint16_t gimbal_min = 1000;             // lowest gimbal value (calibrated)
  uint16_t gimbal_max = 2000;             // largest gimbal value (calibrated)
  uint16_t gimbal_mid = 1500;             // center postion
} TDataRC;

typedef struct __attribute__ ((packed)) {
  uint8_t pinTOF            = 5;
  uint8_t pinRXLidar        = 0;
  uint8_t pinTXLidar        = 2;

  uint8_t pidTOF[3]         = {50,5,0}; // represent a floating number multiplied by 100 for ToF-Sensor P/I/D
  uint8_t pidLIDAR[3]       = {50,5,0}; // represent a floating number multiplied by 100 for LIDAR P/I/D
  uint16_t minHeight        = 100;      // minimal height over ground in mm
  uint16_t maxHeight        = 1000;     // maximal height over ground in mm
  uint16_t minHoverHeight   = 400;      // minimal height over ground in mm
  uint16_t maxHoverHeight   = 600;      // maximal height over ground in mm
  uint8_t thresholdRangeMM  = 50;       // Threshold for height in mm (0-100)
  uint8_t continousScanRate = 50;       // Threshold for scans 50ms
  uint16_t sensorConditionMin=0;        //
  uint16_t sensorConditionMax=1000;     //
  uint8_t pidOutputError    =100;       // maximal value calculated by pid-controller
  uint8_t tofAdjustment     =75;        // represent floating number multiplied by 100 (=> 0.75 = 75)
  uint8_t lidarAdjustment   =75;        // represent floating number multiplied by 100 (=> 0.75 = 75)
  uint8_t bias              =75;        // please adjust this value -> higher more throttle outcome, lower -> less throttle outcome (Multiplicator for PIDAdjustesValue. This value is the new "HOVERING-Value")
} TDataSurface;    

typedef struct __attribute__ ((packed)) {
  uint8_t  pidRGain[3]      = {50,5,0};       // kp, ki, kd for PID-Controller (multiplyed by 100) (0-255 / 100 =  255=2.55, 1=0.01)
  uint8_t  pidPGain[3]      = {50,5,0};       // kp, ki, kd for PID-Controller (multiplyed by 100)
  uint8_t  pidYGain[3]      = {50,5,0};       // kp, ki, kd for PID-Controller (multiplyed by 100)
  uint8_t  biasRPY[3]       = {100,100,100};  // bias for R/P/Y  (multiplyed by 100)
  uint8_t  setPointSlipRP[2]= {0,0};          // set the set point for slipping roll/pitch. normally should be 0.0, value will deviced by 100
} TDataOFlow;

typedef struct __attribute__ ((packed)) {
  uint8_t header;
  long start_millis;
  long end_millis;
  char fwversion[10];
} TDataGlobal;

typedef struct __attribute__ ((packed)) {
  char domain[5];       // normaly used as short name for task
  char subdomain[5];    // sub category inside a Task
} TDataGroup;

typedef struct __attribute__ ((packed)) {
  TDataGlobal global;
  TDataGroup  group;
  TDataRC     rc;
  TDataSurface sdist;
  TDataOFlow  oflow;
  uint16_t    crc;
} TDataAll;


extern TDataAll      dataAll;
extern TDataGlobal   dataGlobal;
extern TDataRC       dataRC;
extern TDataSurface  dataSDist;
extern TDataOFlow    dataOFlow;



#endif