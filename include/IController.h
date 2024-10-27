/**
 * @brief Interface used by MOCK classes
 */


#ifndef _ICONTROLLER_H_
#define _ICONTROLLER_H_

#include <Arduino.h>

template <typename T>
class IController {
    public:
        virtual ~IController() = default; // Virtueller Destruktor für Polymorphie
        virtual void read(T &data) = 0;

};

#endif