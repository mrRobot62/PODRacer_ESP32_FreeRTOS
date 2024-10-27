#include "BlinkController.h"

BlinkController::BlinkController(uint8_t statusLEDPin, uint8_t errorLEDPin) {
    this->statusLEDPin = statusLEDPin;
    this->errorLEDPin = errorLEDPin;


}


void BlinkController::blink(uint8_t statusPattern, uint8_t errorPattern) {

}