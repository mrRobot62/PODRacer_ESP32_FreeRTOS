#include "Utils.h"

bool isWithinRange(uint16_t current, uint16_t target, uint16_t range) {
    return (current >= (target - range)) && (current <= (target + range));
}

bool isLowerAs(uint16_t current, uint16_t target, uint8_t range) {
    return (current <= (target + range));
}

bool isHigherAs(uint16_t current, uint16_t target, uint8_t range) {
    return (current >= (target - range));
}

