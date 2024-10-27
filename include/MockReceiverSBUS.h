#ifndef _MOCK_RECEIVERSBUS_H_
#define _MOCK_RECEIVERSBUS_H_

#include "IReceiver.h"
#include <math.h>  // Für die Sinusfunktion

class MockReceiverSBUS : public IReceiver {
    public:
        MockReceiverSBUS() {
            this->currentMillis = millis();
        }

        void read(TDataRC *data) {
            currentMillis = millis();
            // fülle Kanäle 0-3 (RPTY)
                // Serial.printf("Mock:read() cms: %d, pms: %d, interv: %d \n",
                //     currentMillis,
                //     previousMillis,
                //     interval
                // );

            // Prüfen, ob das Intervall seit dem letzten Update vergangen ist
            if (currentMillis - previousMillis >= interval) {
                previousMillis = currentMillis;
                float v = calculateNextValue(this->midValue, (data->raw_channels[15] / 1000.0));
                for (uint8_t i=0; i < 4; i++) {
                    data->raw_channels[i] = constrain(uint16_t(v) + (i*15), 1000, 2000);
                }
                data->raw_channels[15] += uint16_t(0.1 * 1000);
                data->fail_safe = false;
                data->lost_frame = false;
                data->is_armed = true;
                
            }
    };


    void write(TDataRC *data){
        // Simulation, das das schreiben etwas dauert
        vTaskDelay(2 / portTICK_PERIOD_MS);
    };


    private:
        int step = 10;
        // Parameter für die Sinuskurve
        const float amplitude = 500.0;  // Differenz zur Mitte (2000 - 1500 oder 1500 - 1000)
        const float midValue = 1500.0;  // Mittelwert
        const float frequency = 0.1;    // Frequenz für 10 Datenpunkte pro Sekunde

        const unsigned long interval = 100; // Intervall in Millisekunden (100ms = 10x pro Sekunde)
        unsigned long currentMillis = 0;

        unsigned long previousMillis = 0;   // Zur Speicherung des letzten Aktualisierungszeitpunkts
        float timeStep = 0.0;               // Zeitvariable für die Sinusberechnung




        // Funktion zur Berechnung des nächsten Wertes
        float calculateNextValue(float lastValue, float timeStep) {
            // Berechne den neuen Wert basierend auf der Sinusfunktion
            return midValue + amplitude * sin(2 * PI * frequency * timeStep);
        }


};


#endif