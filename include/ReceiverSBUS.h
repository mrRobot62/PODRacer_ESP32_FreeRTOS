#ifndef RECEIVERSBUS_H
#define RECEIVERSBUS_H

#include <Arduino.h>
//#include <sbus.h>
#include "IReceiver.h"
#include "data_struct.h"

// ReceiverSBUS Klasse
class ReceiverSBUS : public IReceiver {
private:
//    bfs::SbusRx *sbus_rx;
//    bfs::SbusTx *sbus_tx;
//    bfs::SbusData sbus_read_data, sbus_write_data, sbus_last_data;
    uint8_t rxPin, txPin;
    HardwareSerial *bus;
    TDataRC rcData;

public:
    // Konstruktor
    ReceiverSBUS(HardwareSerial *serialPort, uint8_t rxPin, uint8_t txPin);

    // Methode zum Auslesen der SBUS-Daten
    void read(TDataRC *data) override ;

    // Methode zum Senden der SBUS-Daten
    void write(TDataRC *data) override ;

    // Zugriff auf Daten
    TDataRC getData() override { return rcData; }
};

#endif // RECEIVERSBUS_H
