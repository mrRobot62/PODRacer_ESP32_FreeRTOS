#ifndef _MockPMW3901OFlowSensor_H_
#define _MockPMW3901OFlowSensor_H_

#include "ISensor.h"

class MockPMW3901OFlowSensor : public ISensor<TSensorPMW3901>
{
public:
    MockPMW3901OFlowSensor() {};
    ~MockPMW3901OFlowSensor() override {};

    virtual void update(TSensorPMW3901 *sensorData) override
    {
    }

    virtual bool sensorInit(TSensorPMW3901 *sensorData) override
    {
    }

private:
};

#endif