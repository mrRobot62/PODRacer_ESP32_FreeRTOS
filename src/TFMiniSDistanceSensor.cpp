#include "TFMiniSDistanceSensor.h"

TFMiniSDistanceSensor::TFMiniSDistanceSensor()
{
    //double kp, double ki, double kd, double maxDistance, HardwareSerial* serial)
    //pidDistance(&inputDistance, &outputDistance, &setpointDistance, kp, ki, kd, DIRECT
    // : serialPort(serial), kp(kp), ki(ki), kd(kd), maxDistance(maxDistance)
    distance = 0;
    setpointDistance = 0;
}

void TFMiniSDistanceSensor::sensorInit() {
    
}

void TFMiniSDistanceSensor::update() {
}

double TFMiniSDistanceSensor::getDistance() {
    return outputDistance;
}

double TFMiniSDistanceSensor::getNormalizedDistance() {
    return 0.0;
}

void TFMiniSDistanceSensor::setTargetDistance(double targetDistance) {
}

double TFMiniSDistanceSensor::normalizeDistance(double distance) {
    return 0.0;
}
