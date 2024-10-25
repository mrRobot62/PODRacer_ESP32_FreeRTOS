#ifndef _VL53L1XDISTANCECONTROLLER_H_
#define _VL53L1XDISTANCECONTROLLER_H_

#include "SensorBase.h" // Basisklasse
#include "ISensor.h"    // Interface
#include <Arduino.h>
#include <VL53L1X.h>
#include <PID_v1.h>

class VL53L1XDistanceSensor : public SensorBase, public ISensor {
public:
    VL53L1XDistanceSensor();
    ~VL53L1XDistanceSensor() override {};

    void sensorInit() override;
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


    double kp, ki, kd;

    double normalizeDistance(double distance); // Normierung der Distanz
};

#endif
