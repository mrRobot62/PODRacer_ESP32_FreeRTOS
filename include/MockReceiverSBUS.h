#ifndef _MOCK_RECEIVERSBUS_H_
#define _MOCK_RECEIVERSBUS_H_

#include "IReceiver.h"
#include <math.h> // Für die Sinusfunktion

class MockReceiverSBUS : public IReceiver
{
public:
    MockReceiverSBUS(uint8_t mock_pattern)
    {
        this->currentMillis = millis();
        this->mock_pattern = mock_pattern;
    };

    void read(TDataRC *data);

    void write(TDataRC *data);

private:
    uint8_t mock_pattern;
    int step = 10;
    // Parameter für die Sinuskurve
    const float amplitude = 500.0; // Differenz zur Mitte (2000 - 1500 oder 1500 - 1000)
    const float midValue = 1500.0; // Mittelwert
    const float frequency = 0.1;   // Frequenz für 10 Datenpunkte pro Sekunde

    const unsigned long interval = 100; // Intervall in Millisekunden (100ms = 10x pro Sekunde)
    unsigned long currentMillis = 0;

    unsigned long previousMillis = 0; // Zur Speicherung des letzten Aktualisierungszeitpunkts
    float timeStep = 0.0;             // Zeitvariable für die Sinusberechnung

    void _simulateDataBit0(TDataRC *data) {
        // Serial.printf("MOCK set BIT0 :c'%d', '%d', '%d'", data->gimbal_max, data->gimbal_mid, data->gimbal_min);
    };

    void _simulateDataBit1(TDataRC *data);

    void _simulateDataBit2(TDataRC *data);

    void _simulateDataBit3(TDataRC *data);

    void _simulateDataBit4(TDataRC *data);

    void _simulateDataBit5(TDataRC *data);

    void _simulateDataBit6(TDataRC *data);

    void _simulateDataBit7(TDataRC *data);

    // Funktion zur Berechnung des nächsten Wertes
    float _calculateNextValue(float lastValue, float timeStep);
};

#endif