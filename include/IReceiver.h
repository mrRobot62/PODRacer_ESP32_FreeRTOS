#ifndef _IRECEIVER_H_
#define _IRECEIVER_H_

#include <Arduino.h>
#include "data_struct.h"


class IReceiver {
    public:
        //virtual IReceiver(HardwareSerial *serialPort, uint8_t rxPin, uint8_t txPin);
        virtual void read(TDataRC *data) = 0;
        virtual void write(TDataRC *data) = 0;
        virtual TDataRC getData() = 0;
};

#endif
