#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>
#include "data_struct.h"



/**
 * @brief Prüft ob der Wert in current im Bereich des Targets liegt (in einer Range von Target-range / Target+range)
 */
bool isWithinRange(uint16_t current, uint16_t target, uint16_t range);

/**
 * @brief Prüft ob der Wert kleiner-gleich target ist. Target kann durch Range angepasst werden (target + range)
 */
bool isLowerAs(uint16_t current, uint16_t target, uint8_t range=0);

/**
 * @brief Prüft ob der Wert größer-gleich target ist. Target kann durch Range angepasst werden (target - range)
 */
bool isHigherAs(uint16_t current, uint16_t target, uint8_t range);

#endif