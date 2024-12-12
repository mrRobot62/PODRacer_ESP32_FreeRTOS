#ifndef SENSORWRAPPER_H_
#define SENSORWRAPPER_H_

#include <Wire.h>
#include <TFMPlus.h>
#include <VL53L1X.h>
#include <Bitcraze_PMW3901.h>
#include <PID_v1.h>

#include "ConfigManager.h"
#include "data_struct.h"
#include "globals.h"

/**
 * @brief SensorWrapperSingleton ist eine Singleton-Klasse die nur 1x instanziiert werden kann und verwaltet alle
 * Sensoren die verwendet werden. Das heißt wenn mehrere Tasks mit den Sensoren arbeiten, wird physisch aber nur einmalig
 * der entsprechende Sensor bereitgestellt
 *
 * Der Zugriff auf einen Sensor wird übereinen Semaphor geschützt um konkurrierende Zugriffe zu vermeiden.
 *
 */
class SensorWrapperSingleton
{
private:
    SensorWrapperSingleton(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TDataComplementaryFilterCfg *cfg);
    static SensorWrapperSingleton *instance;

    SensorWrapperSingleton(const SensorWrapperSingleton &) = delete;
    SensorWrapperSingleton &operator=(const SensorWrapperSingleton &) = delete;

    void init();

public:
    /**
     * @brief gibt eine neue oder die aktuelle Instanz des SensorWrapperSingleton zurück
     *
     */
    static SensorWrapperSingleton *getInstance(HardwareSerial *lidarBus, uint8_t pinCSPMW3901, TDataComplementaryFilterCfg *cfg)
    {
        if (instance == nullptr)
        {
            instance = new SensorWrapperSingleton(lidarBus, pinCSPMW3901, cfg);
        }
        return instance;
    }

    /**
     * @brief Es wird nur eine Instanz erzeugt. Diese Instanz verarbeitet alle Sensor-Signale
     * Alle die auf diese Instanz zugreifen nutzen die gleichen Sensorsignale. Die Ergebnisse
     * der Sensoren liegt in der lokalen Struktur und wir hier als zeiger zurück gegeben
     * @return TDataComplementaryFilter Zeiger auf die Struktur
     */
    TDataComplementaryFilter *getData() { return this->lokalFilterData; };

    /**
     * @brief Liest vom OpticalFlowSensor die Bewegung und gibt die Werte in deltaX und deltaY zurück
     * um konkurrierende Zugriff zu vermeiden wird ein oflowSemaphor verwendet
     * @param deltaX Bewegung +/- in X Richtung
     * @param deltaY Bewegung +/- in Y Richtung
     */
    void OFlow(int16_t *deltaX, int16_t *deltaY);

private:
    HardwareSerial *lidarBus;
    uint8_t pinCSPMW3901;

    TFMPlus *sensorLidar;
    VL53L1X *sensorToF;
    Bitcraze_PMW3901 *sensorOptFlow;

    TDataComplementaryFilterCfg *cfg;
    TDataComplementaryFilter *lokalFilterData;

    PID *heightPID, *yawPID, *driftXPID, *driftYPID;
};

extern SensorWrapperSingleton *sensorWrapper;

#endif