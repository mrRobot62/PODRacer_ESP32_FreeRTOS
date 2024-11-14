#include "MockReceiverSBUS.h"

void MockReceiverSBUS::read(TDataRC *data)
{
    // mock_pattern - Bit0 - default
    data->raw_channels[ch_map[ROLL]] = data->gimbal_mid;     // center
    data->raw_channels[ch_map[PITCH]] = data->gimbal_mid;    // center
    data->raw_channels[ch_map[YAW]] = data->gimbal_mid;      // center
    data->raw_channels[ch_map[THROTTLE]] = data->gimbal_min; // min
    data->raw_channels[ch_map[HOVER]] = data->gimbal_min;    // min
    data->raw_channels[ch_map[ARM]] = data->gimbal_min;      // disarmed

    // disarmed
    if (CHECK_BIT(mock_pattern, BIT0))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT0 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit0(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT1))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT1 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit1(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT2))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT2 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit2(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT3))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT3 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit3(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT4))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT4 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit4(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT5))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT5 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit5(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT6))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT6 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit6(data);
    }
    else if (CHECK_BIT(mock_pattern, BIT7))
    {
        Serial.println("-----------------------------------");
        Serial.println("MOCK - BIT7 - set");
        Serial.println("-----------------------------------");
        _simulateDataBit7(data);
    }
    else
    {
        Serial.println("MockReceiver - ungültiges MOCK_PATTERN");
    }

    // data->is_armed = data->raw_channels[ARMING];

    // if (data->is_armed)
    //     SET_BIT(data->recvStatus, ARMING_BIT);
    // else
    //     CLEAR_BIT(data->recvStatus, ARMING_BIT);
};

void MockReceiverSBUS::write(TDataRC *data)
{
    // Simulation, das das schreiben etwas dauert
    vTaskDelay(2 / portTICK_PERIOD_MS);
};

void MockReceiverSBUS::_simulateDataBit1(TDataRC *data)
{
    // data->raw_channels[ROLL] = data->gimbal_mid;
    // data->raw_channels[PITCH] = data->gimbal_mid;
    // data->raw_channels[YAW] = data->gimbal_mid;
    // data->raw_channels[THROTTLE] = data->gimbal_min;
    // data->raw_channels[THRUST] = data->gimbal_min;
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

void MockReceiverSBUS::_simulateDataBit2(TDataRC *data)
{
    // Simulation: PreventArming, weil R <> mid ist
    data->raw_channels[ch_map[ROLL]] = data->gimbal_min;
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

void MockReceiverSBUS::_simulateDataBit3(TDataRC *data)
{
    // Simulation: PreventArming, weil THR/TRU <> min ist
    data->raw_channels[ch_map[THROTTLE]] = data->gimbal_mid;
    data->raw_channels[ch_map[HOVER]] = data->gimbal_mid;
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

void MockReceiverSBUS::_simulateDataBit4(TDataRC *data)
{
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

void MockReceiverSBUS::_simulateDataBit5(TDataRC *data)
{
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

void MockReceiverSBUS::_simulateDataBit6(TDataRC *data)
{
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

void MockReceiverSBUS::_simulateDataBit7(TDataRC *data)
{
    data->raw_channels[ch_map[ARM]] = data->gimbal_max; // armed
};

// Funktion zur Berechnung des nächsten Wertes
float MockReceiverSBUS::_calculateNextValue(float lastValue, float timeStep)
{
    // Berechne den neuen Wert basierend auf der Sinusfunktion
    return midValue + amplitude * sin(2 * PI * frequency * timeStep);
};