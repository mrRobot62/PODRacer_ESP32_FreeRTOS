#include "HoverController.h"

HoverController::HoverController()
{
#ifdef USE_MOCK_PMW3901
    this->pmw3901 = new MockPMW3901OFlowSensor();
#else
    this->pmw3901 = new PMW3901OpticalFlow();
#endif
}

void HoverController::read(TDataHover &data)
{
    // prüfe CENTER_POSITION für ROLL/PITCH/YAW und THRUST auf MIN
    // wenn nicht Centered, dann prevent_arming = TRUE
    if (
        isWithinRange(data.raw_channels[ch_map[ROLL]], GIMBAL_MID, GIMBAL_NOISE) &&
        isWithinRange(data.raw_channels[ch_map[PITCH]], GIMBAL_MID, GIMBAL_NOISE) &&
        isWithinRange(data.raw_channels[ch_map[YAW]], GIMBAL_MID, GIMBAL_NOISE) &&
        isLowerAs(data.raw_channels[ch_map[THROTTLE]], GIMBAL_MIN, 0) && // kein Rauschen erlaubt
        isLowerAs(data.raw_channels[ch_map[HOVER]], GIMBAL_MIN, 0)      // kein Rauschen erlaubt

    )
    {
        Serial.println(">>>> HOVER - RPY=mid, THR/HOVER=min");
    }
    else
    {
        // sicher stellen, das Werte in new_channels auf 0 stehen
        // setze prevent_arming auf true setzen
        Serial.println(">>>> HOVER - PREVENT_ARM");
        memset(data.new_channels, 0, sizeof(data.new_channels));
        data.prevent_arming = true;
        return;
    }
}