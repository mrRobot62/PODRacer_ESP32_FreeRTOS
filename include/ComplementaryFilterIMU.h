#ifndef COMPLEMENTARY_FILTER_IMU_H
#define COMPLEMENTARY_FILTER_IMU_H

#include "ComplementaryFilterBase.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#
class ComplementaryFilterIMU : public ComplementaryFilterBase
{
public:
    ComplementaryFilterIMU(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TDataComplementaryFilterCfg *cfg);
    void update(TDataComplementaryFilter *filterData) override;
    void begin(TDataComplementaryFilter *filterData);

protected:
    void calibrateGyro(TDataComplementaryFilter *filterData);
    void calculateOrientation(TDataComplementaryFilter *filterData);

private:
    Adafruit_MPU6050 *mpu;
    const float dt = 0.01; // 10 ms Zykluszeit

    float pitch, roll;
    float gyroOffsetX, gyroOffsetY, gyroOffsetZ;
};

#endif // COMPLEMENTARY_FILTER_NO_IMU_H
