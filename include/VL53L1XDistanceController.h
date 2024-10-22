#ifndef VL53L1XDISTANCECONTROLLER_H
#define VL53L1XDISTANCECONTROLLER_H

#include "SensorBase.h" // Basisklasse
#include <Arduino.h>
#include <VL53L1X.h>
#include <PID_v1.h>

class VL53L1XDistanceController : public SensorBase {
public:
    VL53L1XDistanceController(double kp, double ki, double kd, double maxDistance);

    void init() override;
    void update() override;

    double getDistance() override;
    double getNormalizedDistance() override;

    void setTargetDistance(double targetDistance) override;

private:
    VL53L1X sensor;
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
