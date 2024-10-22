#include "EMAFilter.h"

// Konstruktor: Initialisiert den Glättungsfaktor und den gefilterten Wert
EMAFilter::EMAFilter(float smoothingFactor) : alpha(smoothingFactor), filteredValue(0.0) {}

// Init-Methode: Setzt den Startwert des gefilterten Werts
void EMAFilter::init(float initialValue) {
    filteredValue = initialValue;
}

// Update-Methode: Berechnet den neuen gefilterten Wert
float EMAFilter::update(float newValue) {
    filteredValue = alpha * newValue + (1 - alpha) * filteredValue;
    return filteredValue;
}

// Get-Methode: Gibt den aktuellen gefilterten Wert zurück
float EMAFilter::getFilteredValue() const {
    return filteredValue;
}
