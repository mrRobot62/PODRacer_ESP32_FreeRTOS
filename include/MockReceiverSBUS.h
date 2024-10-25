#ifndef _MOCK_RECEIVERSBUS_H_
#define _MOCK_RECEIVERSBUS_H_

#include "IReceiver.h"
#include <cmath>  // Für die Sinusfunktion

class MockReceiverSBUS : public IReceiver {
    void read(TDataRC *data) {
        // fülle Kanäle 0-3 (RPTY)
        for (uint8_t i = 0; i < 4; i++) {
            data->channels[i] = constrain(
                uint16_t(
                    calculateNextValue(data->channels[i], (data->channels[15] / step) )
                ), 
                1000, 2000);
        }

        data->channels[15] += step;
    };
    void write(TDataRC *data){

    };


    private:
        int step = 10;
        // Parameter für die Sinuskurve
        const float amplitude = 500.0;  // Differenz zur Mitte (2000 - 1500 oder 1500 - 1000)
        const float midValue = 1500.0;  // Mittelwert
        const float frequency = 0.1;    // Frequenz für 10 Datenpunkte pro Sekunde

        // Funktion zur Berechnung des nächsten Wertes
        float calculateNextValue(float lastValue, float timeStep) {
            // Berechne den neuen Wert basierend auf der Sinusfunktion
            return midValue + amplitude * std::sin(2 * M_PI * frequency * timeStep);
        }

};


#endif