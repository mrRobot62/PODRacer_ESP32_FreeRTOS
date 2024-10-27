#include "HoverController.h"

HoverController::HoverController() {
    #ifdef USE_MOCK_PMW3901
        this->pmw3901 = new MockPMW3901OFlowSensor();
    #else
        this->pmw3901 = new PMW3901OpticalFlow();
    #endif
}

void HoverController::read(TDataHover &data) {
    

}