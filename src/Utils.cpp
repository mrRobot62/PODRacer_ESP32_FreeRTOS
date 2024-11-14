#include "Utils.h"

bool isWithinRange(uint16_t current, uint16_t target, uint16_t upper_range, uint16_t lower_range)
{
    return (current >= (target - lower_range)) && (current <= (target + upper_range));
}

bool isLowerAs(uint16_t current, uint16_t target, uint8_t range)
{
    return (current <= (target + range));
}

bool isHigherAs(uint16_t current, uint16_t target, uint8_t range)
{
    return (current >= (target - range));
}

bool isMax(uint16_t current)
{
    return isHigherAs(current, gSBUSDefaultValues.maxGimbal, 0);
}

bool isMax(uint16_t current, uint8_t range)
{
    return isHigherAs(current, gSBUSDefaultValues.maxGimbal, range);
}

bool isMid(uint16_t current, uint8_t noise = GIMBAL_NOISE)
{
    return isWithinRange(current, gSBUSDefaultValues.midGimbal, noise, noise);
}

bool isCenterPos(uint16_t current, uint8_t upper_noise, uint8_t lower_noise)
{
    return isWithinRange(current, gSBUSDefaultValues.midGimbal, upper_noise, lower_noise);
}

bool isMin(uint16_t current)
{
    return isLowerAs(current, gSBUSDefaultValues.minGimbal, 0);
}

bool isMin(uint16_t current, uint8_t range)
{
    return isLowerAs(current, gSBUSDefaultValues.minGimbal, range);
}

void setChannelMapping(uint8_t *newMapping)
{
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        ch_map[i] = newMapping[i];
    }
}
