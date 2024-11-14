#include "ReceiverSBUS.h"
#include "Utils.h"

#define CALIB_RAW_MIN 0  // sbus_calibration_data index 0, raw min
#define CALIB_RAW_MID 1  // sbus_calibration_data index 1, raw mid
#define CALIB_RAW_MAX 2  // sbus_calibration_data index 2, raw max
#define CALIB_GIMB_MIN 3 // sbus_calibration_data index 3, gimbal min
#define CALIB_GIMB_MID 4 // sbus_calibration_data index 4, gimbal mid
#define CALIB_GIMB_MAX 5 // sbus_calibration_data index 5, gimbal max

// Konstruktor
ReceiverSBUS::ReceiverSBUS(uint8_t rxPin, uint8_t txPin)
{
    this->rxPin = rxPin;
    this->txPin = txPin;

    this->sbus_rx = new bfs::SbusRx(&Serial2, rxPin, txPin, true, false);
    this->sbus_tx = new bfs::SbusTx(&Serial2, rxPin, txPin, true, false);
    sbus_rx->Begin();
    sbus_tx->Begin();

    Serial.println("SBUS initialized....");
}

// Methode zum Auslesen der SBUS-Daten
void ReceiverSBUS::read(TDataRC *data)
{
    if (sbus_rx->Read())
    {
        sbus_raw_data = sbus_rx->data();
        data->fail_safe = sbus_raw_data.failsafe;
        data->lost_frame = sbus_raw_data.lost_frame;
        memcpy(data->raw_channels, sbus_raw_data.ch, sizeof(sbus_raw_data.ch));
        //logger->info(*data, millis(), "SBUS", "RAW");

        for (uint8_t ch = 0; ch < MIN_NUMBER_OF_CHANNELS; ch++)
        {
            // ermittlung des MIN/MAX Raw-Wertes
            sbus_calibration_data[ch][CALIB_RAW_MIN] = (data->raw_channels[ch] < sbus_calibration_data[ch][CALIB_RAW_MIN] || sbus_calibration_data[ch][CALIB_RAW_MIN] == 0 ? data->raw_channels[ch] : sbus_calibration_data[ch][CALIB_RAW_MIN]);
            sbus_calibration_data[ch][CALIB_RAW_MAX] = (data->raw_channels[ch] > sbus_calibration_data[ch][CALIB_RAW_MAX] || sbus_calibration_data[ch][CALIB_RAW_MAX] == 0 ? data->raw_channels[ch] : sbus_calibration_data[ch][CALIB_RAW_MAX]);
            //sbus_calibration_data[ch][CALIB_RAW_MID] = sbus_calibration_data[ch][CALIB_RAW_MIN] + (sbus_calibration_data[ch][CALIB_RAW_MAX] - sbus_calibration_data[ch][CALIB_RAW_MIN]) >> 1;
            // Mapping auf einen normierten Wertebereich zwischen 1000 und 2000
            data->raw_channels[ch] = map(
                sbus_raw_data.ch[ch],
                sbus_calibration_data[ch][CALIB_RAW_MIN],
                sbus_calibration_data[ch][CALIB_RAW_MAX],
                sbus_calibration_data[ch][CALIB_GIMB_MIN],
                sbus_calibration_data[ch][CALIB_GIMB_MAX]);
        }
        //
        //  relevante SBUS-Flags setzen
        // logger->printBinary8("SBUS","preRst ", data->armingMask);
        resetSBUSFlags(&data->armingMask, data);
        // logger->printBinary8("SBUS","postRst", data->armingMask);

        /***************************************************************************/
        /* Ob tatsächlich ein Armen möglich ist, wird im ReceiverTask festgestellt */
        /***************************************************************************/
    }
    else
    {
        Serial.println("!!!!! -------------------- ERROR   ----------------------- !!!!!");
        Serial.println("no sbus->read() data available...");
        Serial.println("!!!!! ---------------------------------------------------- !!!!!");
    }
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
