#ifndef RECEIVERSBUS_H
#define RECEIVERSBUS_H

#include <Arduino.h>
#include <sbus.h>
#include "IReceiver.h"
#include "data_struct.h"

// ReceiverSBUS Klasse
class ReceiverSBUS : public IReceiver
{
public:
    // Konstruktor
    ReceiverSBUS(uint8_t rxPin = 16, uint8_t txPin = 17);

    // Methode zum Auslesen der SBUS-Daten
    void read(TDataRC *data) override;

    // Methode zum Senden der SBUS-Daten
    void write(TDataRC *data) override;

private:
    bfs::SbusRx *sbus_rx;
    bfs::SbusTx *sbus_tx;
    bfs::SbusData sbus_raw_data;

    uint8_t rxPin, txPin;
    TDataRC rcData;
};

#endif // RECEIVERSBUS_H
