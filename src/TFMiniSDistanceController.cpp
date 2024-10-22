#include "TFMiniSDistanceController.h"

TFMiniSDistanceController::TFMiniSDistanceController(double kp, double ki, double kd, double maxDistance, HardwareSerial* serial)
    : serialPort(serial), kp(kp), ki(ki), kd(kd), maxDistance(maxDistance),
      pidDistance(&inputDistance, &outputDistance, &setpointDistance, kp, ki, kd, DIRECT)
{
    distance = 0;
    setpointDistance = 0;
}

void TFMiniSDistanceController::init() {
    serialPort->begin(115200);
    tfmini.begin(serialPort);
    pidDistance.SetMode(AUTOMATIC);
}

void TFMiniSDistanceController::update() {
    distance = tfmini.getDistance();
    inputDistance = distance;
    pidDistance.Compute();
}

double TFMiniSDistanceController::getDistance() {
    return outputDistance;
}

double TFMiniSDistanceController::getNormalizedDistance() {
    return normalizeDistance(distance);
}

void TFMiniSDistanceController::setTargetDistance(double targetDistance) {
    setpointDistance = targetDistance;
}

double TFMiniSDistanceController::normalizeDistance(double distance) {
    double normalized = abs(distance) / maxDistance;
    return constrain(normalized, 0.0, 1.0);
}
