#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <freertos/semphr.h>

#include "globals.h"

#define MUTEX_WAIT_TIMEOUT pdMS_TO_TICKS(50)

#define NUMBER_OF_CHANNELS 16
#define MIN_NUMBER_OF_CHANNELS 8 // RPY, Throttle, Thrust  (Arming wird als bool angegeben und nicht im Channel)

#define QUEUE_SIZE 25
#define LOOP_TIME 100

enum
{
  OFF,
  ON
};

//---------------------------------------------------------------
//  Channels Belegung
//---------------------------------------------------------------

enum
{
  ROLL,     // 0
  PITCH,    // 1
  THROTTLE, // 2
  YAW,      // 3
  ARM,      // 4
  HOVER,    // 7
  AUX2,     // 5
  AUX3,     // 6
  NUM_CHANNELS
};

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief Channel-Mapping,
   * Default ROLL = CH0, PITCH=CH1, THR=CH7(POTI), YAW=CH3, HOVER=CH2, ARM=CH4, AUX2=CH5, AUX3=CH6
   *
   */
  // uint8_t ch_map[NUM_CHANNELS] = {0, 1, 7, 3, 4, 2, 5, 6}; => in der main.cpp !
  extern uint8_t ch_map[NUM_CHANNELS]; // Nur die Deklaration

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------
// RC-Globals
//---------------------------------------------------------------
#define GIMBAL_MAX 2000
#define GIMBAL_MID 1500
#define GIMBAL_MIN 1000
#define GIMBAL_NOISE 10
//---------------------------------------------------------------

/**
 * @brief Nur für zusätzliche Loginformationen die über denen einer TDataXYZ-Struct hinaus gehen
 * @param doubles: Array von maximal drei double-Werten
 * @param ints: Array von maximal drei 16Bit Integer Werten
 * @param messages: Array von drei zusätzlichen String nachrichten. Diese sollte nicht länger als 15 Bytes sein (sonst nicht mehr lesbar im Terminal)
 */
typedef struct
{
  double doubles[3];
  uint16_t ints[3];
  String messages[3];
} TLogAdditionals;

typedef struct
{
  uint16_t minGimbal = GIMBAL_MIN;
  uint16_t maxGimbal = GIMBAL_MAX;
  uint16_t midGimbal = GIMBAL_MID;
  uint8_t gimbalNoise = GIMBAL_NOISE; // wird als Range genutzt um den Mittelwert nicht als absoluten Wert zu nutzen

} TSBUSGlobalDefaultValues;

/**
 * @brief Status Pattern / Error Pattern
 * @param status_pattern : B0-B2 für Arming/Prevent/Change
 * B7 = LED Indikator, 1=system running (ready), 0 = starting
 * @param error_pattern: BitX wenn 1, dann Fehler im Bereich
 */
typedef struct __attribute__((packed))
{
  // status pattern (siehe readme_task.md)
  // 0b0000 0000
  //   |_____________ 0=starting mode, 1=ready
  //           |_____ Bit0 = 0=disarmed, 1=armed
  //          |______ Bit1 = 1=prevent arming, 0=arming possible
  //         |_______ Bit2 = 1=status changed, 0=no change
  //        |________ Bit3 = free
  //
  //      |__________ Bit4 = free
  //     |___________ Bit5 = free
  //    |____________ Bit6 = free
  //   |_____________ Bit7 = 0=starting mode, 1=ready

  uint8_t status_pattern = 0b0000000;
  // error_pattern
  // 0b0000 0000
  //           |____ Bit0 = Error in receiver task
  //          |_____ Bit1 = Error in mixer Task
  //         |______ Bit2 = Error in hover Task
  //        |_______ Bit3 = Error in surface Task
  //
  //      |________ Bit4 = free
  //     |_________ Bit5 = free
  //    |__________ Bit6 = free
  //   |___________ Bit7 = free
  //  |____________ Bit0 = PreventArming
  //

  uint8_t error_pattern = 0b00000000;

  /**
   * @brief set error mask. This mask will be "added" to current error stat (OR)
   * @param mask: bit pattern
   */
  void setError(uint8_t mask)
  {
    error_pattern |= mask;
  }

} TDataStatus;

/**
 * @brief Datenpunkte für den ReceiverTask
 */
typedef struct __attribute__((packed))
{
  uint16_t raw_channels[NUMBER_OF_CHANNELS] = {0};     // raw values from receiver
  uint16_t new_channels[MIN_NUMBER_OF_CHANNELS] = {0}; // adjusted values to FC
  uint16_t gimbal_min = 1000;                          // lowest gimbal value (calibrated)
  uint16_t gimbal_max = 2000;                          // largest gimbal value (calibrated)
  uint16_t gimbal_mid = 1500;                          // center postion
  bool fail_safe = false;
  bool lost_frame = false;
  bool is_armed = false;
  // bool prevent_arming = true;             // default true, arming not possible
  uint8_t armingMask = 0b00000000; // siehe
} TDataRC;

typedef struct __attribute__((packed))
{
  uint8_t pinTOF = 5;
  uint8_t pinRXLidar = 0;
  uint8_t pinTXLidar = 2;

  uint8_t pidTOF[3] = {50, 5, 0};     // represent a floating number multiplied by 100 for ToF-Sensor P/I/D
  uint8_t pidLIDAR[3] = {50, 5, 0};   // represent a floating number multiplied by 100 for LIDAR P/I/D
  uint16_t minHeight = 100;           // minimal height over ground in mm
  uint16_t maxHeight = 1000;          // maximal height over ground in mm
  uint16_t minHoverHeight = 400;      // minimal height over ground in mm
  uint16_t maxHoverHeight = 600;      // maximal height over ground in mm
  uint8_t thresholdRangeMM = 50;      // Threshold for height in mm (0-100)
  uint8_t continousScanRate = 50;     // Threshold for scans 50ms
  uint16_t sensorConditionMin = 0;    //
  uint16_t sensorConditionMax = 1000; //
  uint8_t pidOutputError = 100;       // maximal value calculated by pid-controller
  uint8_t tofAdjustment = 75;         // represent floating number multiplied by 100 (=> 0.75 = 75)
  uint8_t lidarAdjustment = 75;       // represent floating number multiplied by 100 (=> 0.75 = 75)
  uint8_t bias = 75;                  // please adjust this value -> higher more throttle outcome, lower -> less throttle outcome (Multiplicator for PIDAdjustesValue. This value is the new "HOVERING-Value")
  uint16_t distanceFront = 0;
  uint16_t distanceRear = 0;
  bool prevent_arming = true;
} TDataSurface;

typedef struct __attribute__((packed))
{
  uint8_t pidRGain[3] = {50, 5, 0};     // kp, ki, kd for PID-Controller (multiplyed by 100) (0-255 / 100 =  255=2.55, 1=0.01)
  uint8_t pidPGain[3] = {50, 5, 0};     // kp, ki, kd for PID-Controller (multiplyed by 100)
  uint8_t pidYGain[3] = {50, 5, 0};     // kp, ki, kd for PID-Controller (multiplyed by 100)
  uint8_t biasRPY[3] = {100, 100, 100}; // bias for R/P/Y  (multiplyed by 100)
  uint8_t setPointSlipRP[2] = {0, 0};   // set the set point for slipping roll/pitch. normally should be 0.0, value will deviced by 100
  uint16_t x_value = 0;                 // current X-Value from Sensor multiplied by 100
  uint16_t y_value = 0;                 // current Y-Value from Sensor multiplied by 100
  bool prevent_arming = true;

} TDataOFlow;

/**
 * @brief Hover Datenstruktur. Sie wird genutzt um Daten vom Receiver (raw_channels) zu erhalten
 * und um berechnete Daten weiter zu leiten (new_channels)
 * @param raw_channels : Roh-Daten des Receivers
 * @param new_channels : relative Daten insbesondere für RPYT
 * @param prevent_armin: Default true, Armen ist nicht möglich. False: Armen ist möglich
 */
typedef struct __attribute__((packed))
{
  uint16_t raw_channels[MIN_NUMBER_OF_CHANNELS] = {0};
  uint16_t new_channels[MIN_NUMBER_OF_CHANNELS] = {0};
  bool prevent_arming = true;

} TDataHover;

typedef struct __attribute__((packed))
{
  uint16_t channels[MIN_NUMBER_OF_CHANNELS] = {0};
  bool prevent_arming = true;

} TDataSteering;

/**
 * @brief
 * @param
 * @param bool prevent_arming = true;
 *
 */
typedef struct __attribute__((packed))
{
  long start_millis; // Starting
  long end_millis;   // End (only used to measure timings)
  // bool prevent_ariming = true;
  char fwversion[10];
} TDataGlobal;

/**
 * @brief
 * @param
 * @param bool prevent_arming = true;
 *
 */
typedef struct __attribute__((packed))
{
  char domain[5];    // normaly used as short name for task
  char subdomain[5]; // sub category inside a Task
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
typedef struct __attribute__((packed))
{
  uint16_t start = 0xFFEE;
  TDataGlobal global;
  TDataStatus status;
  TDataGroup group;
  TDataRC rc;
  TDataSurface sdist;
  TDataOFlow oflow;
  TDataHover hover;
  TDataSteering steering;
  uint16_t end = 0xEEFF;
} TDataAll;

typedef struct __attribute__((packed))
{
  size_t freeHeap = 0;
  size_t minFreeHeap = 0;
} TESP32Memory;

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

extern TDataGlobal dataGlobal;
extern TDataRC dataRC;
extern TDataSurface dataSDist;
extern TDataOFlow dataOFlow;
extern TESP32Memory espMemory;
extern TSBUSGlobalDefaultValues gSBUSDefaultValues;

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
extern QueueHandle_t queueHoverRcv;
extern QueueHandle_t queueHoverSend;
extern QueueHandle_t queueSteering;

extern TDataAll globalData;
extern SemaphoreHandle_t xTDataAllMutex;
extern EventGroupHandle_t xEventGroup;

#endif