#ifndef _MockVL53L1XDistanceSensor_H_
#define _MockVL53L1XDistanceSensor_H_

#include "ISensor.h"

class VL53L1XDistanceSensor : public ISensor {
    VL53L1XDistanceSensor() {}
    void update() override {

    }

    void sensorInit() override {

    }
    
    double getDistance() override {

    }
    
    double getNormalizedDistance() override {

    }
    
    void setTargetDistance(double targetDistance) override {

    }
    
};

#endif