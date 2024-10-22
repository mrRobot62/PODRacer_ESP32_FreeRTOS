#include "VL53L1XDistanceController.h"

VL53L1XDistanceController::VL53L1XDistanceController(double kp, double ki, double kd, double maxDistance)
    : kp(kp), ki(ki), kd(kd), maxDistance(maxDistance),
      pidDistance(&inputDistance, &outputDistance, &setpointDistance, kp, ki, kd, DIRECT)
{
    distance = 0;
    setpointDistance = 0;
}

void VL53L1XDistanceController::init() {
    Wire.begin();
    sensor.setTimeout(500);
    if (!sensor.init()) {
        Serial.println("Failed to detect and initialize sensor!");
        while (1);
    }
    sensor.setDistanceMode(VL53L1X::Long);
    sensor.startContinuous(50); // Startet den Sensor im kontinuierlichen Modus

    // PID-Controller initialisieren
    pidDistance.SetMode(AUTOMATIC);
}

void VL53L1XDistanceController::update() {
    // Sensor-Daten lesen
    distance = sensor.read();

    // Aktuelle Ist-Werte setzen
    inputDistance = distance;

    // PID-Regelung durchführen
    pidDistance.Compute();

    // Hier könntest du die Ausgaben (outputDistance) verwenden,
    // um Motoren oder andere Steuerungen anzupassen
}

double VL53L1XDistanceController::getDistance() {
    return outputDistance;
}

double VL53L1XDistanceController::getNormalizedDistance() {
    return normalizeDistance(distance);
}

void VL53L1XDistanceController::setTargetDistance(double targetDistance) {
    setpointDistance = targetDistance;
}

double VL53L1XDistanceController::normalizeDistance(double distance) {
    // Normiert die Distanz in einen Bereich von 0.0 bis 1.0
    double normalized = abs(distance) / maxDistance;
    return constrain(normalized, 0.0, 1.0); // Begrenzung auf den Bereich 0.0 - 1.0
}
