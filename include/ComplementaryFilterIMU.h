#ifndef COMPLEMENTARY_FILTER_IMU_H
#define COMPLEMENTARY_FILTER_IMU_H

#include "ComplementaryFilterBase.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#
class ComplementaryFilterIMU : public ComplementaryFilterBase
{
public:
    ComplementaryFilterIMU(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TSensorCFG *cfg);
    void update(TDataSensors *filterData) override;
    void begin(TDataSensors *filterData);

protected:
    void calibrateGyro(TDataSensors *filterData);
    void calculateOrientation(TDataSensors *filterData);

private:
    Adafruit_MPU6050 *mpu;
    const float dt = 0.01; // 10 ms Zykluszeit

    float pitch, roll;
    float gyroOffsetX, gyroOffsetY, gyroOffsetZ;
};

#endif // COMPLEMENTARY_FILTER_NO_IMU_H
