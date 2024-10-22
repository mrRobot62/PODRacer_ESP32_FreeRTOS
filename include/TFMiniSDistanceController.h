#ifndef TFMINISDISTANCECONTROLLER_H
#define TFMINISDISTANCECONTROLLER_H

#include "SensorBase.h" // Basisklasse
#include <Arduino.h>
#include <PID_v1.h>
#include <TFMini.h>

class TFMiniSDistanceController : public SensorBase {
public:
    TFMiniSDistanceController(double kp, double ki, double kd, double maxDistance, HardwareSerial* serial);

    void init() override; // Initialisierung des Sensors
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

    PID pidDistance;

    double kp, ki, kd;

    double normalizeDistance(double distance); // Normierung der Distanz
};

#endif
