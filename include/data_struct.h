#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <freertos/semphr.h>

#include "globals.h"

#define NUMBER_OF_CHANNELS 16
#define MIN_NUMBER_OF_CHANNELS 8    // RPY, Throttle, Thrust  (Arming wird als bool angegeben und nicht im Channel)

#define LOOP_TIME 100



enum {OFF, ON};

//---------------------------------------------------------------
//  Channels Belegung 
//---------------------------------------------------------------
#define ROLL 0
#define PITCH 1
#define THRUST 2
#define YAW 3
#define THROTTLE 4      // über POTI
#define HOVER 4         // Alias für THROTTLE - über POTI
#define ARMING 5        //

//---------------------------------------------------------------
/**
 * @brief Nur für zusätzliche Loginformationen die über denen einer TDataXYZ-Struct hinaus gehen
 * @param doubles: Array von maximal drei double-Werten
 * @param ints: Array von maximal drei 16Bit Integer Werten
 * @param messages: Array von drei zusätzlichen String nachrichten. Diese sollte nicht länger als 15 Bytes sein (sonst nicht mehr lesbar im Terminal)
 */
typedef struct {
  double doubles[3];
  uint16_t ints[3];
  String messages[3];
} TLogAdditionals;

typedef struct {
  uint16_t minGimbal = 1000;
  uint16_t maxGimbal = 2000;
  uint16_t midGimbal = 1500;
  uint8_t gimbalNoise = 5;      // wird als Range genutzt um den Mittelwert nicht als absoluten Wert zu nutzen
  

} TGlobalDefaultValues;

typedef struct __attribute__((packed)) {
  // status pattern (siehe readme_task.md)
  // 0b0000 0000
  //           |____ 0=starting mode, 1=ready
  //          |_____ 0=disarmed, 1=armed
  //         |______ 1=prevent arming, 0=arming possible
  //        |_______ 1=status changed, 0=no change
  //

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
  uint16_t raw_channels[NUMBER_OF_CHANNELS] = {0};      // raw values from receiver
  uint16_t new_channels[MIN_NUMBER_OF_CHANNELS] = {0};  // adjusted values to FC
  uint16_t gimbal_min = 1000;             // lowest gimbal value (calibrated)
  uint16_t gimbal_max = 2000;             // largest gimbal value (calibrated)
  uint16_t gimbal_mid = 1500;             // center postion
  bool fail_safe = false;
  bool lost_frame = false;
  // bool is_armed = false; 
  // bool prevent_arming = true;             // default true, arming not possible
  uint8_t recvStatus = 0b00000000;       //
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
  uint16_t distanceFront    = 0;
  uint16_t distanceRear     = 0;
  bool prevent_arming = true;
} TDataSurface;    

typedef struct __attribute__ ((packed)) {
  uint8_t  pidRGain[3]      = {50,5,0};       // kp, ki, kd for PID-Controller (multiplyed by 100) (0-255 / 100 =  255=2.55, 1=0.01)
  uint8_t  pidPGain[3]      = {50,5,0};       // kp, ki, kd for PID-Controller (multiplyed by 100)
  uint8_t  pidYGain[3]      = {50,5,0};       // kp, ki, kd for PID-Controller (multiplyed by 100)
  uint8_t  biasRPY[3]       = {100,100,100};  // bias for R/P/Y  (multiplyed by 100)
  uint8_t  setPointSlipRP[2]= {0,0};          // set the set point for slipping roll/pitch. normally should be 0.0, value will deviced by 100
  uint16_t x_value          = 0;              // current X-Value from Sensor multiplied by 100
  uint16_t y_value          = 0;              // current Y-Value from Sensor multiplied by 100
  bool prevent_arming = true;

} TDataOFlow;

/**
 * @brief Hover Datenstruktur. Sie wird genutzt um Daten vom Receiver (raw_channels) zu erhalten
 * und um berechnete Daten weiter zu leiten (new_channels)
 * @param raw_channels : Roh-Daten des Receivers
 * @param new_channels : relative Daten insbesondere für RPYT
 * @param prevent_armin: Default true, Armen ist nicht möglich. False: Armen ist möglich
 */
typedef struct __attribute__ ((packed)) {
  uint16_t raw_channels[MIN_NUMBER_OF_CHANNELS] = {0};
  uint16_t new_channels[MIN_NUMBER_OF_CHANNELS] = {0};
  bool prevent_arming = true;

} TDataHover;

typedef struct __attribute__ ((packed)) {
  uint16_t channels[MIN_NUMBER_OF_CHANNELS] = {0};
  bool prevent_arming = true;

} TDataSteering;

/**
 * @brief 
 * @param 
 * @param bool prevent_arming = true;
 * 
 */
typedef struct __attribute__ ((packed)) {
  long start_millis;            // Starting
  long end_millis;              // End (only used to measure timings)
  //bool prevent_ariming = true;
  char fwversion[10];
} TDataGlobal;

/**
 * @brief 
 * @param 
 * @param bool prevent_arming = true;
 * 
 */
typedef struct __attribute__ ((packed)) {
  char domain[5];       // normaly used as short name for task
  char subdomain[5];    // sub category inside a Task
} TDataGroup;

/**
 * @brief Gesamt-Datenstruktur über alle Tasks und zusätzlich Informationen
 * @param global: globale Daten (ms, arming, ...)
 * @param status: led status
 * @param group: Domain/Subdomain
 * @param rc: TDataRC
 * @param sdist: TDataSurface
 * @param oflow: TDataOFlow
 * @param hover: TDataHover
 * @param steering: TDataSteering
 * 
 */
typedef struct __attribute__ ((packed)) {
  uint16_t start = 0xFFEE;
  TDataGlobal global;
  TDataStatus status;
  TDataGroup  group;
  TDataRC     rc;
  TDataSurface sdist;
  TDataOFlow  oflow;
  TDataHover  hover;
  TDataSteering steering;
  uint16_t end = 0xEEFF;
} TDataAll;

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


extern TDataGlobal   dataGlobal;
extern TDataRC       dataRC;
extern TDataSurface  dataSDist;
extern TDataOFlow    dataOFlow;

// Ereignisbits
// aktuell nur für Receiver & Webserver
// wenn das jeweilige Bit gesetzt ist, liest der Task die globale TDataAll 
//
#define BIT_RECEIVER (1 << 0)
#define BIT_WEBSERVER (1 << 1)

// Globale Variablen
extern QueueHandle_t queueReceiver;
extern QueueHandle_t queueSurface;
extern QueueHandle_t queueOFlow;
extern QueueHandle_t queueHover;
extern QueueHandle_t queueSteering;


extern TDataAll globalData;
extern SemaphoreHandle_t xTDataAllMutex;
extern EventGroupHandle_t xEventGroup;



#endif