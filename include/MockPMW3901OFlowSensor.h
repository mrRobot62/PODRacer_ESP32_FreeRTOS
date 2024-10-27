#ifndef _MockPMW3901OFlowSensor_H_
#define _MockPMW3901OFlowSensor_H_

#include "ISensor.h"

class PMW3901OFlowSensor : public ISensor {
    PMW3901OFlowSensor();
    ~PMW3901OFlowSensor() override {};

    void sensorInit() override; // Initialisierung des Sensors
    void update() override; // Aktualisiert die Sensordaten

    double getDistance() override; // Gibt die tatsächliche gemessene Distanz zurück
    double getNormalizedDistance() override; // Gibt die normierte Distanz zurück

    void setTargetDistance(double targetDistance) override; // Setzt die Soll-Distanz
};

#endif