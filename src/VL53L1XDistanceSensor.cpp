#include "VL53L1XDistanceSensor.h"

VL53L1XDistanceSensor::VL53L1XDistanceSensor(){}


void VL53L1XDistanceSensor::sensorInit() {
//      pidDistance(&inputDistance, &outputDistance, &setpointDistance, kp, ki, kd, DIRECT)
}

void VL53L1XDistanceSensor::update() {
    // // Sensor-Daten lesen
    // distance = sensor.read();

    // // Aktuelle Ist-Werte setzen
    // inputDistance = distance;

    // // PID-Regelung durchführen
    // pidDistance.Compute();

    // Hier könntest du die Ausgaben (outputDistance) verwenden,
    // um Motoren oder andere Steuerungen anzupassen
}

double VL53L1XDistanceSensor::getDistance() {
    return 0.0;
}

double VL53L1XDistanceSensor::getNormalizedDistance() {
    return 0.0;
}

void VL53L1XDistanceSensor::setTargetDistance(double targetDistance) {
    
}

double VL53L1XDistanceSensor::normalizeDistance(double distance) {
    // map() vom echten Wert in 0.01 - 1.00
    return 0.0;
}
