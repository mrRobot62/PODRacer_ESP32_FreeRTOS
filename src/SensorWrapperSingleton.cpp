#include "SensorWrapperSingleton.h"

SensorWrapperSingleton::SensorWrapperSingleton(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TSensorCFG *cfg)
{
    sensorOptFlow = new Bitcraze_PMW3901(pinCSPMW3901);
    sensorToF = new VL53L1X();
    sensorLidar = new TFMPlus();
    this->cfg = cfg;
    this->init();
}

void SensorWrapperSingleton::init()
{
    if (!sensorLidar->begin(this->lidarBus))
    {
        // <tbd> - Lidar-error
    }

    if (!sensorToF->init())
    {
        // <tbd> - ToF-error
    }

    sensorToF->setTimeout(500);
    sensorToF->startContinuous(50);

    if (!sensorOptFlow->begin())
    {
        // <tbd> OpticalFlow-error
    }

    //--------------------------------------------------
    // PID controller für die Sensoren initiieren
    //--------------------------------------------------
    heightPID = new PID(
        &lokalFilterData->pidInput,
        &lokalFilterData->pidOutput,
        &lokalFilterData->pidSetpoint,
        double(cfg->pidKValues[0] / 100.0),
        double(cfg->pidKValues[1] / 100.0),
        double(cfg->pidKValues[2] / 100.0),
        DIRECT);

    yawPID = new PID(
        &lokalFilterData->pidYawInput,
        &lokalFilterData->pidYawOutput,
        &lokalFilterData->pidYawSetpoint,
        double(cfg->pidYawValues[0] / 100.0),
        double(cfg->pidYawValues[1] / 100.0),
        double(cfg->pidYawValues[2] / 100.0),
        DIRECT);

    driftXPID = new PID(
        &lokalFilterData->pidDriftXInput,
        &lokalFilterData->pidDriftXOutput,
        &lokalFilterData->pidDriftSetpoint,
        double(cfg->pidDriftXValues[0] / 100.0),
        double(cfg->pidDriftXValues[1] / 100.0),
        double(cfg->pidDriftXValues[2] / 100.0),
        DIRECT);

    driftYPID = new PID(
        &lokalFilterData->pidDriftYInput,
        &lokalFilterData->pidDriftYOutput,
        &lokalFilterData->pidDriftSetpoint,
        double(cfg->pidDriftYValues[0] / 100.0),
        double(cfg->pidDriftYValues[1] / 100.0),
        double(cfg->pidDriftYValues[2] / 100.0),
        DIRECT);

    heightPID->SetMode(AUTOMATIC);
    heightPID->SetOutputLimits(
        cfg->pidKLimit[0],
        cfg->pidKLimit[1]);
}

void SensorWrapperSingleton::OFlow(int16_t *deltaX, int16_t *deltaY)
{
    //
    // wenn Ressource frei ist, dann kann gelesen werden
    if (xSemaphoreTake(xSemaSensorOFlow, (TickType_t)10) == pdTRUE)
    {

        xSemaphoreGive(xSemaSensorOFlow);
    }
}
