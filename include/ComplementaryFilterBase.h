#ifndef COMPLEMENTARY_FILTER_BASE_H
#define COMPLEMENTARY_FILTER_BASE_H

#include <Wire.h>
#include <TFMPlus.h>
#include <VL53L1X.h>
#include <Bitcraze_PMW3901.h>
#include <PID_v1.h>

#include "ConfigManager.h"
#include "data_struct.h"
#include "globals.h"

/**
 * @brief Basis-Klasse für ComplementaryFilter Klassen - es wird eine ComplementaryFilterWithIMU und ComplementaryFilterNoIMU geben
 * je nach Anwendungsfall
 *
 */

class ComplementaryFilterBase
{
public:
    ComplementaryFilterBase(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TSensorCFG *cfg);
    virtual ~ComplementaryFilterBase();
    void begin(TDataSensors *filterData);
    virtual void update(TDataSensors *filterData) = 0; // Reine virtuelle Methode
    void setTargetHeight(uint16_t height);
    float getYaw() const;
    float getHeight() const;
    bool isFailsafeActive() const;

protected:
    HardwareSerial *lidarBus;
    uint8_t pinCSPMW3901;

    // Sensoren (Pointer)
    TFMPlus *sensorRear;
    VL53L1X *sensorFront;
    Bitcraze_PMW3901 *sensorOptFlow;

    TSensorCFG *cfg;
    TDataSensors *lokalFilterData;

    void updateOpticalFlow(TDataSensors *filterData);
    void updateYawCompensation(TDataSensors *filterData);
    void updateDriftCompensation(TDataSensors *filterData);

    // PID-Controller
    // double targetHeight;
    // double pidInput, pidOutput, pidSetpoint;
    PID *heightPID;
    PID *yawPID;
    PID *driftXPID;
    PID *driftYPID;

    // Failsafe
    // bool failsafeActive;
    unsigned long lastValidSensorRead;

    bool checkFailsafe();
};

#endif // COMPLEMENTARY_FILTER_BASE_H
