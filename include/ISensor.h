/**
 * @brief Interface used by MOCK classes
 */


#ifndef _ISENSOR_H_
#define _ISENSOR_H_

/**
 * @brief OpticalFlow Sensor PMW3901
 * @param double raw_x: Abweichung X (+/-)
 * @param double raw_y: Abweichung Y (+/-)
 * @param double normalized_X: Normalisierter Wert zwischen -1.0 .. +1.0
 * @param double normalized_Y: Normalisierter Wert zwischen -1.0 .. +1.0
 */
 typedef struct {
    double raw_x = 0.0;
    double raw_y = 0.0;
    double normalized_X = 0.0;
    double normalized_Y = 0.0;
    double setPointX = 0.0;
    double setPointY = 0.0;
} TSensorPMW3901;

/**
 * @brief  
 * @param 
 *
 */
typedef struct {
    double minDistance = 0;
    double maxDistance = 0;
    double currentDistance = 0;
} TSensorDistance;

template <typename T>
class ISensor {
    public:
        virtual ~ISensor() {} // Virtueller Destruktor für Polymorphie
        virtual void update(T &sensorData) = 0; // Aktualisiert die Sensordaten
        virtual void sensorInit(T &sensorData) = 0;
};

#endif