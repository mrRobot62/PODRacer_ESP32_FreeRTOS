#ifndef _SENSORPMW3901_H_
#define _SENSORPMW3901_H_

#include <Arduino.h>
#include "ISensor.h"

class PMW3901OpticalFlow : public ISensor<TSensorPMW3901> {
    public:
        PMW3901OpticalFlow();
        ~PMW3901OpticalFlow() {} // Virtueller Destruktor für Polymorphie
        void update(TSensorPMW3901 &sensorData) override;
        void sensorInit(TSensorPMW3901 &sensorData) override;

};


#endif