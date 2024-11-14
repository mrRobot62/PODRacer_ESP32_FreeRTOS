#include "ReceiverSBUS.h"

#define CALIB_RAW_MIN 0  // sbus_calibration_data index 0, raw min
#define CALIB_RAW_MID 1  // sbus_calibration_data index 0, raw mid
#define CALIB_RAW_MAX 2  // sbus_calibration_data index 0, raw max
#define CALIB_GIMB_MIN 3 // sbus_calibration_data index 0, gimbal min
#define CALIB_GIMB_MID 4 // sbus_calibration_data index 0, gimbal mid
#define CALIB_GIMB_MAX 5 // sbus_calibration_data index 5, gimbal max

// Konstruktor
ReceiverSBUS::ReceiverSBUS(uint8_t rxPin, uint8_t txPin)
{
    // this->rxPin = rxPin;
    // this->txPin = txPin;

    // bfs::SbusRx srx(*serialPort);

    // sbus_rx = new bfs::SbusRx(serialPort);
    // sbus_tx = new bfs::SbusTx(serialPort);
}

// Methode zum Auslesen der SBUS-Daten
void ReceiverSBUS::read(TDataRC *data)
{

    sbus_raw_data = sbus_rx->data();
    data->fail_safe = sbus_raw_data.failsafe;
    data->lost_frame = sbus_raw_data.lost_frame;
    for (uint8_t ch = 0; ch < NUMBER_OF_CHANNELS; ch++)
    {
        // ermittlung des MIN/MAX Raw-Wertes
        sbus_calibration_data[ch][CALIB_RAW_MIN] = (sbus_raw_data.ch[ch] < sbus_calibration_data[ch][CALIB_RAW_MIN] ? sbus_raw_data.ch[ch] : sbus_calibration_data[ch][CALIB_RAW_MIN]);
        sbus_calibration_data[ch][CALIB_RAW_MAX] = (sbus_raw_data.ch[ch] < sbus_calibration_data[ch][CALIB_RAW_MAX] ? sbus_raw_data.ch[ch] : sbus_calibration_data[ch][CALIB_RAW_MAX]);

        // Mapping auf einen normierten Wertebereich zwischen 1000 und 2000
        data->raw_channels[ch] = map(
            sbus_raw_data.ch[ch],
            sbus_calibration_data[ch][CALIB_RAW_MIN],
            sbus_calibration_data[ch][CALIB_RAW_MAX],
            sbus_calibration_data[ch][CALIB_GIMB_MIN],
            sbus_calibration_data[ch][CALIB_GIMB_MAX]

        );
    }
    Serial.printf("SBUS-RAW CH0: %d, CH1: %d, CH2: %d \n", sbus_raw_data.ch[0], sbus_raw_data.ch[1], sbus_raw_data.ch[2]);
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
void ReceiverSBUS::write(TDataRC *data)
{
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
