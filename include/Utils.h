#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>
#include "data_struct.h"

/**
 * @brief Prüft ob der Wert in current im Bereich des Targets liegt (in einer Range von Target-range / Target+range)
 */
bool isWithinRange(uint16_t current, uint16_t target, uint16_t upper_range = GIMBAL_NOISE, uint16_t lower_range = GIMBAL_NOISE);

/**
 * @brief Prüft ob der Wert kleiner-gleich target ist. Target kann durch Range angepasst werden (target + range)
 */
bool isLowerAs(uint16_t current, uint16_t target, uint8_t range = 0);

/**
 * @brief Prüft ob der Wert größer-gleich target ist. Target kann durch Range angepasst werden (target - range)
 */
bool isHigherAs(uint16_t current, uint16_t target, uint8_t range);

bool isMax(uint16_t current);
bool isMax(uint16_t current, uint8_t range);

bool isMid(uint16_t current);

bool isCenterPos(uint16_t current, uint8_t upper_noise = GIMBAL_NOISE, uint8_t lower_noise = GIMBAL_NOISE);

bool isMin(uint16_t current);
bool isMin(uint16_t current, uint8_t range=GIMBAL_NOISE);

/**
 * @brief setzt ein neues Channel-Mapping und überschreibt somit die Default Einstellunge.
 * Wird später genutzt um über den WebServer ein Channel-Mapping auf die jeweiligen Umgebung anzupassen
 * @param *newMapping Zeiger auf ein uint8_t Array. Beachten: Das Array muss mindestens 8 Einträge haben.
 */
void setChannelMapping(uint8_t *newMapping);

#endif