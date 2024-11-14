#ifndef _IRECEIVER_H_
#define _IRECEIVER_H_

#include <Arduino.h>
#include "data_struct.h"
#include "Logger.h"
#include "Utils.h"

class IReceiver
{
public:
    // virtual IReceiver(HardwareSerial *serialPort, uint8_t rxPin, uint8_t txPin);
    virtual void read(TDataRC *data) = 0;
    virtual void write(TDataRC *data) = 0;

protected:
    /**
     * @brief führt basierend auf SBUS-Signalen ein setzen der armingFlags
     * für die Kanäle RPY, Throttle, Hover, ArmSwitch. Setzt Bit0 auf 0
     * @param mask BitMaske die gesetzt wird
     * @param data Data-Struct, hier werden die Channels ausgelesen
     */
    void resetSBUSFlags(uint8_t *mask, TDataRC *data)
    {
        // Bit 7,6,5    = RPY,      1=wenn CP, 0= !CP
        // Bit 4,3      = HOVER/THROTTLE 1=wenn MIN, 0= <> MIN
        // Bit 2        = wird gesetzt, wenn ArmSwitch = ON
        // Bit 1        = wird ignoriert
        // Bit 0        = wird auf 0 gesetzt

        //
        // Grundeinstellung, alle SBUS (außer ARM-SW) werden resettet
        // ARM-SW, ARMING, PREVENT behalten ihren aktuellen Stand
        //
        *mask &= 0b00000111;
        if (isCenterPos(data->raw_channels[ch_map[ROLL]], GIMBAL_NOISE, 5))
        {
            SET_BIT(*mask, BIT7);
        }
        if (isCenterPos(data->raw_channels[ch_map[PITCH]], GIMBAL_NOISE, 5))
        {
            SET_BIT(*mask, BIT6);
        }
        if (isCenterPos(data->raw_channels[ch_map[YAW]], GIMBAL_NOISE, 5))
        {
            SET_BIT(*mask, BIT5);
        }
        if (isMin(data->raw_channels[ch_map[THROTTLE]], GIMBAL_NOISE))
        {
            SET_BIT(*mask, BIT4);
        }
        if (isMin(data->raw_channels[ch_map[HOVER]], GIMBAL_NOISE))
        {
            SET_BIT(*mask, BIT3);
        }
        if (isMax(data->raw_channels[ch_map[ARM]], GIMBAL_NOISE))
        {
            SET_BIT(*mask, BIT2);
        }
        else
        {
            CLEAR_BIT(*mask, BIT2);
        }
    }

    /**
     * @brief Prüft ob ein ARMING möglich ist
     * BIT7-3   = Gimbals Potis
     * BIT2     = ARM-Switch
     * BIT1     = ARMING möglich
     * BIT0     = PreventArming Bit, 1=Arming nicht möglich
     */
    bool calculateIfArmingPossible(uint8_t *mask, TDataRC *data)
    {
        uint8_t rc = false;

        if (CHECK_BIT_PATTERN(*mask, 0b11111000) == true && CHECK_BIT(*mask, BIT2) && CHECK_BIT_ZERO(*mask, BIT1))
        {
            //
            // Normales Armen möglich.
            // PODRacer isarmed, default BitMaske, Alle Gimbals in Grundeinstellung
            //
            SET_BIT(*mask, BIT1);
            CLEAR_BIT(*mask, BIT0);
            // Serial.println("ARM 1");
            return true;
        }
        else if (CHECK_BIT(*mask, BIT2) && CHECK_BIT(*mask, BIT1) && CHECK_BIT_ZERO(*mask, BIT0))
        {
            // PODRacer ist war schon im gearmed
            // Serial.println("ARM 2");
            return true;
        }
        else if (CHECK_BIT_PATTERN(*mask, 0b11111000) == false && CHECK_BIT(*mask, BIT2) && CHECK_BIT_ZERO(*mask, BIT1))
        {
            // Grundeinstellungen fehlerhaft,
            // BIT2:ARM-SW= ON, BIT1:ARMING=OFF
            // CLEAR_BIT(*mask, BIT1);
            // CLEAR_BIT(*mask, BIT0);
            // Serial.println("DIS 1");
            return false;
        }

        // --------------------------------------------------------------------
        //  PODRacer gearmed, Prevent=OFF, Normales Flug-Situation
        // --------------------------------------------------------------------
        // if (CHECK_BIT(*mask, BIT2) && CHECK_BIT_ZERO(*mask, BIT0))
        // {
        //     SET_BIT(*mask, BIT1);
        //     CLEAR_BIT(*mask, BIT0);
        //     Serial.println("ARM 3");
        //     return true;
        // }
        // Serial.println("unknown");
        return rc;
    }

protected:
    // SBUS liefert für min/mid/max Werte, die später in einen Wertebereich
    // zw. 1000-2000 gemapped werden.
    // raw_MIN und raw_MAX sollte pro Receiver 1malig eingestellt werden
    //
    // idx 0-2 uncalibratd values
    // idx 0 = SBUS min, idx 1 = SBUS mid, idx 2 = SBUS max,
    // idx 3-5 calibratied values
    // idx 3 = min, 4 = mid, 5 = max
    //
    // * Default ROLL = CH0, PITCH=CH1, THR=CH7(POTI), YAW=CH3, HOVER=CH2, ARM=CH4, AUX2=CH5, AUX3=CH6

    uint16_t sbus_calibration_data[NUMBER_OF_CHANNELS][6] = {
        {197, 0, 1794, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // ROLL
        {193, 0, 1794, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // PITCH
        {177, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // HOVER
        {193, 0, 1794, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // YAW
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // ARM
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // AUX2
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // AUX3
        {173, 0, 1794, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}, // THROTTLE (Poti)
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
        {173, 0, 1810, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}};

    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX},
    // {0, 0, 0, GIMBAL_MIN, GIMBAL_MID, GIMBAL_MAX}};

    uint8_t lastArmingMask = 0b00000000;
    uint8_t sbusBitMask = 0b11111100; // Bit7,6,5,4,3,2
};

#endif
