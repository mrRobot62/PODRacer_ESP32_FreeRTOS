#include "ComplementaryFilterNoIMU.h"

ComplementaryFilterNoIMU::ComplementaryFilterNoIMU(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TDataComplementaryFilterCfg *cfg)
    : ComplementaryFilterBase(lidarBus, pinCSPMW3901, cfg) {}

void ComplementaryFilterNoIMU::update(TDataComplementaryFilter *filterData)
{
    if (checkFailsafe())
    {
        filterData->isFailsafeActive = true;
        filterData->pidOutput = 0.0;
        return;
    }
    filterData->isFailsafeActive = false;

    //--------------------------------------------------------------------------------------
    // Lese LiDAR-Daten
    //--------------------------------------------------------------------------------------

    // Zur Erinnerung. sensorRear = TFPlus (lidar), sensorFront=VL53L1 (ToF)

    // auslesen des Lidar-Sensors
    int16_t heightRear;
    if (sensorRear->getData(heightRear))
    {
        lastValidSensorRead = millis();
    }
    filterData->rawHeightRear = double(heightRear);

    //--------------------------------------------------------------------------------------
    // auslesen des ToF-Sensors
    //--------------------------------------------------------------------------------------
    filterData->rawHeightFront = sensorFront->readRangeContinuousMillimeters();

    if (!sensorFront->timeoutOccurred())
    {
        lastValidSensorRead = millis();
    }

    //
    // es wird hier lediglich die durchschnittliche Höhe beider Distanzsensoren berechnet
    filterData->rawHeight = (filterData->rawHeightRear + filterData->rawHeightFront) / 2.0;

    // PID-Regelung
    filterData->pidInput = filterData->rawHeight;
    heightPID->Compute();

    // dieser Status wird benötigt um durch Methoden wie getHeight() die aktuelle Höhe abzufragen
    lokalFilterData = filterData;
    // Motorsteuerung hier implementieren

    // Motorsteuerung
    // Beispiele:
    // motor1Output = pidOutput + yawOutput - driftXOutput - driftYOutput;
    // motor2Output = pidOutput - yawOutput + driftXOutput - driftYOutput;
}
