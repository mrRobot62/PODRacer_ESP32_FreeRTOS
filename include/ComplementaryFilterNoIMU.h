#ifndef COMPLEMENTARY_FILTER_NO_IMU_H
#define COMPLEMENTARY_FILTER_NO_IMU_H

#include "ComplementaryFilterBase.h"

class ComplementaryFilterNoIMU : public ComplementaryFilterBase
{
public:
    ComplementaryFilterNoIMU(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TSensorCFG *cfg);
    void update(TDataSensors *filterData) override;
};

#endif // COMPLEMENTARY_FILTER_NO_IMU_H
