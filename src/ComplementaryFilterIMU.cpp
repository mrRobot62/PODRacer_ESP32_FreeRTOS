#include "ComplementaryFilterIMU.h"

ComplementaryFilterIMU::ComplementaryFilterIMU(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TSensorCFG *cfg)
    : ComplementaryFilterBase(lidarBus, pinCSPMW3901, cfg) {}

void ComplementaryFilterIMU::calibrateGyro(TDataSensors *filterData)
{
    const int samples = 500;
    float sumX = 0, sumY = 0, sumZ = 0;

    for (int i = 0; i < samples; i++)
    {
        sensors_event_t a, g, temp;
        mpu->getEvent(&a, &g, &temp);

        sumX += g.gyro.x;
        sumY += g.gyro.y;
        sumZ += g.gyro.z;

        delay(5);
    }

    gyroOffsetX = sumX / samples;
    gyroOffsetY = sumY / samples;
    gyroOffsetZ = sumZ / samples;

    Serial.println("Gyroskop kalibriert!");
}

void ComplementaryFilterIMU::begin(TDataSensors *filterData)
{

    // Initialisiere MPU6050
    if (!mpu->begin())
    {
        Serial.println("MPU6050 Verbindung fehlgeschlagen!");
        while (1)
            ; // Fehlerzustand
    }

    Serial.println("MPU6050 erfolgreich verbunden.");
    mpu->setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu->setGyroRange(MPU6050_RANGE_500_DEG);
    mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);

    // Gyroskop kalibrieren
    calibrateGyro(filterData);
}

void ComplementaryFilterIMU::calculateOrientation(TDataSensors *filterData)
{
    sensors_event_t a, g, temp;
    mpu->getEvent(&a, &g, &temp);

    // Gyroskopdaten (Offset entfernen)
    float gyroX = g.gyro.x - gyroOffsetX; // Roll
    float gyroY = g.gyro.y - gyroOffsetY; // Pitch
    float gyroZ = g.gyro.z - gyroOffsetZ; // Yaw

    // Beschleunigungsdaten
    float accelX = a.acceleration.x;
    float accelY = a.acceleration.y;
    float accelZ = a.acceleration.z;

    // Berechne Yaw durch Integration
    // Der GyroZ-Wert (Drehgeschwindigkeit um die Z-Achse) liefert die Winkelgeschwindigkeit für Yaw (Gieren).
    // Über Zeitintegration kann die aktuelle Yaw-Position berechnet werden:
    // der rawYaw Wert wird später für den PID-Controller als pidYawInput verwendet
    filterData->rawYaw += gyroZ * dt;

    // Berechne Pitch und Roll aus Beschleunigungssensor
    //
    float accelPitch = atan2(accelY, accelZ) * 180 / PI; // absoluter PITCH Winkel aus den AccelerometerDaten
    float accelRoll = atan2(accelX, accelZ) * 180 / PI;

    // Komplementärfilter anwenden
    pitch = cfg->alpha * (pitch + gyroX * dt) + cfg->beta * accelPitch;
    roll = cfg->alpha * (roll + gyroY * dt) + cfg->beta * accelRoll;
}

void ComplementaryFilterIMU::update(TDataSensors *filterData)
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

    // Lese MPU6500-Daten
    calculateOrientation(filterData);

    // Yaw-Korrektur
    filterData->pidYawInput = filterData->rawYaw;
    yawPID->Compute();

    // Motorsteuerung
    // Beispiele:
    // motor1Output = pidOutput + yawOutput - driftXOutput - driftYOutput;
    // motor2Output = pidOutput - yawOutput + driftXOutput - driftYOutput;
}
