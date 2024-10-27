#ifndef _MockTFMiniSDistanceSensor_H_
#define _MockTFMiniSDistanceSensor_H_

#include "ISensor.h"

class TFMiniSDistanceSensor : public ISensor {
    TFMiniSDistanceSensor() {}
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