#ifndef _BLINK_CONTROLLER_H
#define _BLINK_CONTROLLER_H

#include <Arduino.h>

#define LED1 2              // blau (RUNNING, Armed, Disarmed)
#define LED2 15             // rot (Fehler)

  // status pattern
  // 0b0000 0000
  //           |____ 0=starting mode, 1=ready
  //          |_____ 0=disarmed, 1=armed
  //
  // bit 2-7 unused


  // Mask 
  //
  //   |_____________ Bit 7 : 1 = System running, 0=System off
  //   -|____________ Bit 6 : 1 = System armed, 0=System disarmed
  //                  Bit 5,4   = 0 (free)
  // 0b0000 0000
  //           |____ Error in receiver task     (bit 0)     = (1)
  //          |_____ Error in mixer task        (bit 1)     = (2)
  //          ||____ Error in SurfaceTask       (bit 0+1)   = (3)
  //         |______ Error in OpticalFlow Task  (bit2)      = (4)
  //         ||_____ Error in HoverTask         (bit 2+1)   = (6)
  //         |||____ Error in SteeringTask      (bit 3,2,   = (7)
  //        |_______ free 0
  //        ||______ free 0
  //        |||_____ free 0
  //        ||||____ free 0
  //

class BlinkController {
    public:
        /**
         * @brief Controller for blinking outputs. One StatusLED and one ErrorLED
         */
        BlinkController(uint8_t statusLEDPin, uint8_t errorLEDPin);

        /**
         * @brief set a blinking pattern based on parameter
         * 
         * @param pattern   set the blink pattern. 
         * Mask 
         *
         *   |_____________ Bit 7 : 1 = System running, 0=System off
         *   -|____________ Bit 6 : 1 = System armed, 0=System disarmed
         *                  Bit 5,4   = 0 (free)
         * 0b0000 0000
         *           |____ Error in receiver task     (bit 0)     = (1)
         *          |_____ Error in mixer task        (bit 1)     = (2)
         *          ||____ Error in SurfaceTask       (bit 0+1)   = (3)
         *         |______ Error in OpticalFlow Task  (bit2)      = (4)
         *         ||_____ Error in HoverTask         (bit 2+1)   = (6)
         *         |||____ Error in SteeringTask      (bit 3,2,   = (7)
         *        |_______ free 0
         *        ||______ free 0
         *        |||_____ free 0
         *        ||||____ free 0
         */
        void blink(uint8_t statusPattern, uint8_t errorPattern);
    private:
        uint8_t statusLEDPin, errorLEDPin;
};

#endif