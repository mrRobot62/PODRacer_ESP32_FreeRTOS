#include "ReceiverSBUS.h"

// Konstruktor
ReceiverSBUS::ReceiverSBUS(HardwareSerial *serialPort, uint8_t rxPin, uint8_t txPin) {
    this->bus = serialPort;
    this->rxPin = rxPin;
    this->txPin = txPin;
}

// Methode zum Auslesen der SBUS-Daten
void ReceiverSBUS::read(TDataRC *data) {
    //sbus_read_data = sbus_rx->data();
    // if (sbus_rx->read()) {
    //     // Kanäle und Statusinformationen aus der SBUS-Library auslesen
    //     for (int i = 0; i < 16; i++) {
    //         data->channels[i] = sbus.channels[i];
    //     }
    //     data->failsafe = sbus.failsafe();
    //     data->frameLost = sbus.lostFrame();

    //     rcData = *data;  // Lokale Kopie der Daten
    //     return true;     // Erfolgreiches Auslesen
    // }
    // return false;        // Kein neues Frame verfügbar
}

// Methode zum Senden der SBUS-Daten
void ReceiverSBUS::write(TDataRC *data) {
    // Daten aus der TDataRC Struktur in das SBUS Format übertragen
    // for (int i = 0; i < 16; i++) {
    //     sbus.channels[i] = data->channels[i];
    // }

    // // Failsafe und Frame Lost setzen, wenn nötig
    // sbus.setFailsafe(data->failsafe);
    // sbus.setLostFrame(data->frameLost);

    // // SBUS-Daten über UART senden
    // sbus.write();
}
