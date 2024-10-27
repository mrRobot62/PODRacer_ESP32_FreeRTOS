#ifndef _TFMINISDISTANCECONTROLLER_H_
#define _TFMINISDISTANCECONTROLLER_H_

#include "SensorBase.h" // Basisklasse
#include "ISensor.h"    // Interface
#include <Arduino.h>
#include <PID_v1.h>
#include <TFMini.h>

class TFMiniSDistanceSensor : public SensorBase, public ISensor {
public:
    TFMiniSDistanceSensor();
    ~TFMiniSDistanceSensor() override {};

    void sensorInit() override; // Initialisierung des Sensors
    void update() override; // Aktualisiert die Sensordaten

    double getDistance() override; // Gibt die tatsächliche gemessene Distanz zurück
    double getNormalizedDistance() override; // Gibt die normierte Distanz zurück

    void setTargetDistance(double targetDistance) override; // Setzt die Soll-Distanz

private:
    TFMini tfmini;
    HardwareSerial* serialPort;
    double distance;
    double setpointDistance;
    double inputDistance;
    double outputDistance;
    double maxDistance;

    double kp, ki, kd;

    double normalizeDistance(double distance); // Normierung der Distanz
};

#endif
