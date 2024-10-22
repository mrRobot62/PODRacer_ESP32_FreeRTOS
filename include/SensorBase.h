#ifndef SENSORBASE_H
#define SENSORBASE_H

class SensorBase {
public:
    virtual void init() = 0; // Initialisierung des Sensors
    virtual void update() = 0; // Aktualisiert die Sensordaten
    virtual double getDistance() = 0; // Gibt die tatsächliche gemessene Distanz zurück
    virtual double getNormalizedDistance() = 0; // Gibt die normierte Distanz zurück
    virtual void setTargetDistance(double targetDistance) = 0; // Setzt die Soll-Distanz

    virtual ~SensorBase() {} // Virtueller Destruktor für Polymorphie
};

#endif
