#include "HoverController.h"

HoverController::HoverController() {
    #ifdef USE_MOCK_PMW3901
        this->pmw3901 = new MockPMW3901OFlowSensor();
    #else
        this->pmw3901 = new PMW3901OpticalFlow();
    #endif
}

void HoverController::read(TDataHover &data) {
    // prüfe CENTER_POSITION für ROLL/PITCH/YAW und THRUST auf MIN
    // wenn nicht Centered, dann prevent_arming = TRUE
    if (
        isWithinRange(data.raw_channels[ROLL], globalValues.midGimbal, globalValues.gimbalNoise) &&
        isWithinRange(data.raw_channels[PITCH], globalValues.midGimbal, globalValues.gimbalNoise) &&
        isWithinRange(data.raw_channels[YAW], globalValues.midGimbal, globalValues.gimbalNoise) &&
        isLowerAs(data.raw_channels[THRUST], globalValues.minGimbal, 0)                                 // kein Rauschen erlaubt

    ) {
        //
    }
    else {
        // sicher stellen, das Werte in new_channels auf 0 stehen
        // setze prevent_arming auf true setzen
        memset(data.new_channels, 0, sizeof(data.new_channels)); 
        data.prevent_arming = true;
        return;
    }

}