/**
 * @brief Interface used by MOCK classes
 */


#ifndef _ISENSOR_H_
#define _ISENSOR_H_

class ISensor {
    public:
        virtual ~ISensor() {} // Virtueller Destruktor für Polymorphie
        virtual void update() = 0; // Aktualisiert die Sensordaten
        virtual void sensorInit() = 0;
        virtual double getDistance() = 0; // Gibt die tatsächliche gemessene Distanz zurück
        virtual double getNormalizedDistance() = 0; // Gibt die normierte Distanz zurück
        virtual void setTargetDistance(double targetDistance) = 0; // Setzt die Soll-Distanz


};

#endif