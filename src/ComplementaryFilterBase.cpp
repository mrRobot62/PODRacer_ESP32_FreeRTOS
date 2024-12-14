#include "ComplementaryFilterBase.h"

ComplementaryFilterBase::ComplementaryFilterBase(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TSensorCFG *cfg)
    : cfg(cfg),
      lastValidSensorRead(0),
      lidarBus(lidarBus), pinCSPMW3901(pinCSPMW3901)
{
    sensorRear = new TFMPlus();
    sensorFront = new VL53L1X();
    sensorOptFlow = new Bitcraze_PMW3901(5);
}

ComplementaryFilterBase::~ComplementaryFilterBase()
{
    delete sensorRear;
    delete sensorFront;
    delete sensorOptFlow;
    delete heightPID;
    delete yawPID;
    delete driftXPID;
    delete driftYPID;
}

void ComplementaryFilterBase::updateOpticalFlow(TDataSensors *filterData)
{
    int16_t deltaX = 0, deltaY = 0;

    if (sensorOptFlow != nullptr)
    {
        sensorOptFlow->readMotionCount(&deltaX, &deltaY);
        filterData->rawYaw += deltaX * 0.1; // Beispiel: Skalierung in Grad pro Pixelwert

        // Delta-Werte in Yaw-Änderung und Drift umrechnen
        filterData->rawYaw += deltaX * 0.1;        // Beispielskalierung: 0.1 Grad pro Pixel
        filterData->rawDrift[0] += -deltaX * 0.01; // Beispielskalierung: mm pro Pixel
        filterData->rawDrift[1] += deltaY * 0.01;  // Beispielskalierung: mm pro Pixel
    }
    else
    {
        // falls Objekt nicht verfügbar wird die Yaw-Drehrate auf 0 gesetzt
        lokalFilterData->rawYaw = 0.0;
    }
}

void ComplementaryFilterBase::updateYawCompensation(TDataSensors *filterData)
{
    filterData->pidYawInput = filterData->rawYaw; // Aktueller Yaw-Winkel
    yawPID->Compute();                            // Berechnung des Yaw-Outputs
}

void ComplementaryFilterBase::updateDriftCompensation(TDataSensors *filterData)
{
    filterData->pidDriftXInput = filterData->rawDrift[0]; // Aktueller Drift in X-Richtung
    filterData->pidDriftYInput = filterData->rawDrift[1]; // Aktueller Drift in Y-Richtung

    driftXPID->Compute(); // Berechnung der Drift-X-Kompensation
    driftYPID->Compute(); // Berechnung der Drift-Y-Kompensation
}

void ComplementaryFilterBase::begin(TDataSensors *filterData)
{
    //
    // Konfiguration des PID-Controllers. Die Ein- und Ausgabe wird in die Datenstruktur direkt geschrieben
    // konstante Konfigurationsparameter werden aus der CFG-Struktur gelesen
    this->lokalFilterData = filterData;
    filterData->pidSetpoint = cfg->targetHeight;

    heightPID = new PID(
        &filterData->pidInput,
        &filterData->pidOutput,
        &filterData->pidSetpoint,
        double(cfg->pidKValues[0] / 100.0),
        double(cfg->pidKValues[1] / 100.0),
        double(cfg->pidKValues[2] / 100.0),
        DIRECT);

    yawPID = new PID(
        &filterData->pidYawInput,
        &filterData->pidYawOutput,
        &filterData->pidYawSetpoint,
        double(cfg->pidYawValues[0] / 100.0),
        double(cfg->pidYawValues[1] / 100.0),
        double(cfg->pidYawValues[2] / 100.0),
        DIRECT);

    driftXPID = new PID(
        &filterData->pidDriftXInput,
        &filterData->pidDriftXOutput,
        &filterData->pidDriftSetpoint,
        double(cfg->pidDriftXValues[0] / 100.0),
        double(cfg->pidDriftXValues[1] / 100.0),
        double(cfg->pidDriftXValues[2] / 100.0),
        DIRECT);

    driftYPID = new PID(
        &filterData->pidDriftYInput,
        &filterData->pidDriftYOutput,
        &filterData->pidDriftSetpoint,
        double(cfg->pidDriftYValues[0] / 100.0),
        double(cfg->pidDriftYValues[1] / 100.0),
        double(cfg->pidDriftYValues[2] / 100.0),
        DIRECT);

    // TFMPlus initialisieren
    Serial2.begin(115200);
    if (!sensorRear->begin(this->lidarBus))
    {
        Serial.println("TFMPlus nicht verbunden!");
        while (1)
            ;
    }

    // VL53L1X initialisieren
    if (!sensorFront->init())
    {
        Serial.println("VL53L1X nicht verbunden!");
        while (1)
            ;
    }
    sensorFront->setTimeout(500);
    sensorFront->startContinuous(50);

    // PMW3901 initialisieren
    if (!sensorOptFlow->begin())
    {
        Serial.println("PMW3901 nicht verbunden!");
        while (1)
            ;
    }

    // PID initialisieren
    heightPID->SetMode(AUTOMATIC);

    // PID Maximum definieren (negativ/positiv)
    heightPID->SetOutputLimits(
        cfg->pidKLimit[0],
        cfg->pidKLimit[1]);
}

void ComplementaryFilterBase::setTargetHeight(uint16_t height)
{
    if (cfg != nullptr)
    {
        cfg->targetHeight = height;
    }
}

float ComplementaryFilterBase::getYaw() const
{
    return lokalFilterData->rawYaw;
}

float ComplementaryFilterBase::getHeight() const
{
    return lokalFilterData->rawHeight;
}

bool ComplementaryFilterBase::isFailsafeActive() const
{
    return lokalFilterData->isFailsafeActive;
}

bool ComplementaryFilterBase::checkFailsafe()
{
    unsigned long currentTime = millis();
    return (currentTime - lastValidSensorRead > cfg->failsafeTimeout); // Timeout 1 Sekunde
}
