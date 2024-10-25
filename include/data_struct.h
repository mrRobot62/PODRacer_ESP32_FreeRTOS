#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <Arduino.h>

#define NUMBER_OF_CHANNELS 16

enum {OFF, ON};

typedef struct __attribute__((packed)) {
  // status pattern
  // 0b0000 0000
  //           |____ 0=starting mode, 1=ready
  //          |_____ 0=disarmed, 1=armed
  //
  // bit 2-7 unused

  uint8_t status_pattern = 0b0000000;  
 
  // error_pattern
  // 0b0000 0000
  //           |____ Error in receiver task     (bit 0)
  //          |_____ Error in mixer task        (bit 1)
  //          ||____ Error in SurfaceTask       (bit 0+1)
  //         |______ Error in OpticalFlow Task  (bit2)
  //         ||_____ Error in HoverTask         (bit 1+2)
  //        |_______ Error in SteeringTask      (bit 3)
  //
  // Bit 4-7 are used as indivudual task error indicator

  uint8_t error_pattern = 0b00000000;

  /**
   * @brief set status mask. This neu state replace current state
   * @param bit pattern
   */
  void setStatus(uint8_t mask) {
    status_pattern = mask;
  }

  /**
   * @brief set error mask. This mask will be "added" to current error stat (OR)
   * @param mask: bit pattern
   */
  void setError(uint8_t mask) {
    error_pattern |= mask;
  }

} TDataStatus;

/**
 * @brief Datenpunkte für den ReceiverTask
 */
typedef struct __attribute__ ((packed)) {
  uint16_t channels[NUMBER_OF_CHANNELS] = {0};
  uint16_t gimbal_min = 1000;             // lowest gimbal value (calibrated)
  uint16_t gimbal_max = 2000;             // largest gimbal value (calibrated)
  uint16_t gimbal_mid = 1500;             // center postion
  bool fail_safe = false;
  bool lost_frame = false;
  bool is_armed; 
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
  long start_millis;
  long end_millis;
  char fwversion[10];
} TDataGlobal;

typedef struct __attribute__ ((packed)) {
  char domain[5];       // normaly used as short name for task
  char subdomain[5];    // sub category inside a Task
} TDataGroup;

typedef struct __attribute__ ((packed)) {
  uint16_t start = 0xFFEE;
  TDataGlobal global;
  TDataStatus status;
  TDataGroup  group;
  TDataRC     rc;
  TDataSurface sdist;
  TDataOFlow  oflow;
  uint16_t end = 0xEEFF;
} TDataAll;



extern TDataAll      dataAll;
extern TDataGlobal   dataGlobal;
extern TDataRC       dataRC;
extern TDataSurface  dataSDist;
extern TDataOFlow    dataOFlow;



#endif