#ifndef _BLINK_CONTROLLER_H
#define _BLINK_CONTROLLER_H

#include <Arduino.h>

//
// 0bxxxx 0000
//           |___ 1 = 100ms Delay (blink frequency)
//          |____ 1 = 200ms
//         |_____ 1 = 500ms
//        |______ 1 = 1000ms
// 0b0000 xxxx
//      |________ 1 = DISAMRED  LED 1
//     |_________ 1 = ARMED     LED 1
//    |__________ 1 = ERR 1     LED 2
//   |___________ 1 = ERR 2     LED 3

class BlinkController
{
public:
  /**
   * @brief Controller for blinking outputs. One StatusLED and one ErrorLED
   */
  BlinkController(uint8_t led1_pin, uint8_t led2_pin, uint8_t led3_pin);

  void blink(uint8_t mask);

private:
  uint8_t led1, led2, led3;
};

#endif