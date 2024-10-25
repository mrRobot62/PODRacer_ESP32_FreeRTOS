#ifndef SENSORBASE_H
#define SENSORBASE_H

#include "ConfigManager.h"

class SensorBase{
public:
    virtual ~SensorBase() {}
    /**
     * allgemeingültige Initialisierung, gilt für alle Sensoren
     */
    void init(ConfigManager *cfg) {
        this->cfg = cfg;
    } 

    // virtual void init(ConfigManager *cfg) = 0; // Initialisierung des Sensors
    // virtual void update() = 0; // Aktualisiert die Sensordaten
    // virtual double getDistance() = 0; // Gibt die tatsächliche gemessene Distanz zurück
    // virtual double getNormalizedDistance() = 0; // Gibt die normierte Distanz zurück
    // virtual void setTargetDistance(double targetDistance) = 0; // Setzt die Soll-Distanz

    //virtual ~SensorBase() {} // Virtueller Destruktor für Polymorphie

protected:
    ConfigManager *cfg;
};

#endif
