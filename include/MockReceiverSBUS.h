#ifndef _MOCK_RECEIVERSBUS_H_
#define _MOCK_RECEIVERSBUS_H_

#include "IReceiver.h"
#include <math.h>  // Für die Sinusfunktion

class MockReceiverSBUS : public IReceiver {
    public:
        MockReceiverSBUS(uint8_t mock_pattern) {
            this->currentMillis = millis();
            this->mock_pattern = mock_pattern;
        };

        void read(TDataRC *data) {
            // mock_pattern - Bit0 - default
            data->raw_channels[ROLL]        = data->gimbal_mid; // center
            data->raw_channels[PITCH]       = data->gimbal_mid; // center
            data->raw_channels[YAW]         = data->gimbal_mid; // center
            data->raw_channels[THROTTLE]    = data->gimbal_min; // min
            data->raw_channels[THRUST]      = data->gimbal_min; // min
            data->raw_channels[ARMING]      = data->gimbal_min; // disarmed
            
            //disarmed
            if (mock_pattern & 0b00000001) {
                //Serial.println("MOCK_REAK Bit0");
                _simulateDataBit0(data);
            }
            else if (mock_pattern & 0b00000010) {
                //Serial.println("----- PATTERN BIT 1");
                _simulateDataBit1(data);
            }
            else if (mock_pattern & 0b00000100) {
                _simulateDataBit2(data);
            }
            else if (mock_pattern & 0b00001000) {
                _simulateDataBit3(data);
            }
            else if (mock_pattern & 0b00010000) {
                _simulateDataBit4(data);
            }
            else if (mock_pattern & 0b00100000) {
                _simulateDataBit5(data);
            }
            else if (mock_pattern & 0b01000000) {
                _simulateDataBit6(data);
            }
            else if (mock_pattern & 0b10000000) {
                _simulateDataBit7(data);
            }
            else {
                Serial.println("MockReceiver - ungültiges MOCK_PATTERN");
            }

            data->is_armed = data->raw_channels[ARMING];
            data->prevent_arming = !data->is_armed;

        };

        void write(TDataRC *data){
            // Simulation, das das schreiben etwas dauert
            vTaskDelay(2 / portTICK_PERIOD_MS);
        };


    private:
        uint8_t mock_pattern;
        int step = 10;
        // Parameter für die Sinuskurve
        const float amplitude = 500.0;  // Differenz zur Mitte (2000 - 1500 oder 1500 - 1000)
        const float midValue = 1500.0;  // Mittelwert
        const float frequency = 0.1;    // Frequenz für 10 Datenpunkte pro Sekunde

        const unsigned long interval = 100; // Intervall in Millisekunden (100ms = 10x pro Sekunde)
        unsigned long currentMillis = 0;

        unsigned long previousMillis = 0;   // Zur Speicherung des letzten Aktualisierungszeitpunkts
        float timeStep = 0.0;               // Zeitvariable für die Sinusberechnung

        void _simulateDataBit0(TDataRC *data) {
            //Serial.printf("MOCK set BIT0 :c'%d', '%d', '%d'", data->gimbal_max, data->gimbal_mid, data->gimbal_min);
        };

        void _simulateDataBit1(TDataRC *data) {
            // data->raw_channels[ROLL] = data->gimbal_mid;
            // data->raw_channels[PITCH] = data->gimbal_mid;
            // data->raw_channels[YAW] = data->gimbal_mid;
            // data->raw_channels[THROTTLE] = data->gimbal_min;
            // data->raw_channels[THRUST] = data->gimbal_min;
            data->raw_channels[ARMING] = data->gimbal_max;      //armed
            
        };

        void _simulateDataBit2(TDataRC *data) {
            data->raw_channels[ROLL] = data->gimbal_min;        
            data->raw_channels[ARMING] = data->gimbal_max;      //armed

        };

        void _simulateDataBit3(TDataRC *data) {
            data->raw_channels[ARMING] = data->gimbal_max;      //armed
            
        };

        void _simulateDataBit4(TDataRC *data) {
            data->raw_channels[ARMING] = data->gimbal_max;      //armed
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
                float v = _calculateNextValue(this->midValue, (data->raw_channels[15] / 1000.0));
                for (uint8_t i=0; i < 4; i++) {
                    data->raw_channels[i] = constrain(uint16_t(v) + (i*15), 1000, 2000);
                }
                data->raw_channels[15] += uint16_t(0.1 * 1000);
                data->fail_safe = false;
                data->lost_frame = false;
                data->is_armed = true;
                
            }            
        };

        void _simulateDataBit5(TDataRC *data) {
            data->raw_channels[ARMING] = data->gimbal_max;      //armed
            
        };

        void _simulateDataBit6(TDataRC *data) {
            data->raw_channels[ARMING] = data->gimbal_max;      //armed
            
        };

        void _simulateDataBit7(TDataRC *data) {
            data->raw_channels[ARMING] = data->gimbal_max;      //armed
            
        };



        // Funktion zur Berechnung des nächsten Wertes
        float _calculateNextValue(float lastValue, float timeStep) {
            // Berechne den neuen Wert basierend auf der Sinusfunktion
            return midValue + amplitude * sin(2 * PI * frequency * timeStep);
        };
};


#endif