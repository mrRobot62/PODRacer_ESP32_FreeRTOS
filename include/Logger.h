#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <HardwareSerial.h>
#include "data_struct.h"
#include <FastCRC.h>

#define BYTE_TO_BINARY_PATTERN  "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY_PATTERN2 "%c%c%c%c %c%c%c%c"
#define BYTE_TO_BINARY(d)  \
  ((d) & 0x80 ? '1' : '0'), \
  ((d) & 0x40 ? '1' : '0'), \
  ((d) & 0x20 ? '1' : '0'), \
  ((d) & 0x10 ? '1' : '0'), \
  ((d) & 0x08 ? '1' : '0'), \
  ((d) & 0x04 ? '1' : '0'), \
  ((d) & 0x02 ? '1' : '0'), \
  ((d) & 0x01 ? '1' : '0') 

class Logger {
    public:
        Logger(HardwareSerial *bus, int baud=115200, uint8_t level=3) {
            this->_useUDP = true;
        };

        void setUDPLog(bool useUDP=true) {
            this->_useUDP = useUDP;
        }

        /**
         * @brief generic info method, print all stuff used by all data structs
         * @param data : data struct to log
         * @param useUDP: default false, if true, send bytestream to host computer
         * @param cr: default true, if set, a carriage return is explizit added to Serial.print statement. Ignored for UDP
         */
        template<typename T>
        void info(const T &data, const char* domain, const char* subdomain,bool useUDP=false, bool cr=true) {
            // call the overloaded method based on parameter
            logSpecificData(data, 3, domain, subdomain, useUDP, cr);
        }


    private:        
        void logSpecificData(const TDataAll tdata,      uint8_t level, const char* domain, const char* subdomain,bool useUDP=false, bool cr=true);
        void logSpecificData(const TDataRC tdata,       uint8_t level, const char* domain, const char* subdomain,bool useUDP=false, bool cr=true);
        void logSpecificData(const TDataOFlow tdata,    uint8_t level, const char* domain, const char* subdomain,bool useUDP=false, bool cr=true);
        void logSpecificData(const TDataSurface tdata,  uint8_t level, const char* domain, const char* subdomain,bool useUDP=false, bool cr=true);
        void logSpecificData(const TDataStatus tdata,   uint8_t level, const char* domain, const char* subdomain,bool useUDP=false, bool cr=true);

    private:
        char buffer[200];
        bool _useUDP = true;

};

extern Logger *logger;

#endif