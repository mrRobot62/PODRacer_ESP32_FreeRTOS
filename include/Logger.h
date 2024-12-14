#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <HardwareSerial.h>
#include "data_struct.h"
#include <FastCRC.h>

#define BYTE_TO_BINARY(d)         \
    ((d) & 0x80 ? '1' : '0'),     \
        ((d) & 0x40 ? '1' : '0'), \
        ((d) & 0x20 ? '1' : '0'), \
        ((d) & 0x10 ? '1' : '0'), \
        ((d) & 0x08 ? '1' : '0'), \
        ((d) & 0x04 ? '1' : '0'), \
        ((d) & 0x02 ? '1' : '0'), \
        ((d) & 0x01 ? '1' : '0')

#define BYTE_TO_BINARY_LSB(d)     \
    ((d) & 0x08 ? '1' : '0'),     \
        ((d) & 0x04 ? '1' : '0'), \
        ((d) & 0x02 ? '1' : '0'), \
        ((d) & 0x01 ? '1' : '0')

#define BYTE_TO_BINARY_MSB(d)     \
    ((d) & 0x80 ? '1' : '0'),     \
        ((d) & 0x40 ? '1' : '0'), \
        ((d) & 0x20 ? '1' : '0'), \
        ((d) & 0x10 ? '1' : '0')

#define ERROR 1
#define WARN 2
#define INFO 3
#define DEBUG 4

class Logger
{
public:
    Logger(HardwareSerial *bus, int baud = 115200, uint8_t level = 3)
    {
        this->_useUDP = true;
    };

    void setUDPLog(bool useUDP = true)
    {
        this->_useUDP = useUDP;
    }

    /**
     * @brief generic info method, print all stuff used by all data structs
     * @param data : data struct to log, TDataAll, TDataRC, TDataHover, TDataSurface, TDataStatus,
     * @param useUDP: default false, if true, send bytestream to host computer
     * @param cr: default true, if set, a carriage return is explizit added to Serial.print statement. Ignored for UDP
     */
    template <typename T>
    void info(const T &data, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true)
    {
        // call the overloaded method based on parameter
        logSpecificData(data, 3, ms, domain, subdomain, useUDP, cr);
    }

    template <typename T>
    void warn(const T &data, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true)
    {
        // call the overloaded method based on parameter
        logSpecificData(data, 2, ms, domain, subdomain, useUDP, cr);
    }

    template <typename T>
    void error(const T &data, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true)
    {
        // call the overloaded method based on parameter
        logSpecificData(data, 1, ms, domain, subdomain, useUDP, cr);
    }

    /**
     * @brief Override info-methode zur ausgabe von Strukturdaten
     * @param data Adresse einer TDataGlobal, TDataStatus, TESP32Memory Struktur
     * @param ms Angabe der Zeit in Millisekunden
     * @param cr Default True, zeilenumbruch wird eingefügt
     */
    template <typename T>
    void info(const T &data, unsigned long ms, bool cr = true)
    {
        logSpecificData2(data, ms, cr);
    }

    /**
     * @brief Logging-Methode die lediglich eine Message ausgibt. Zusätzlich können domain und subdomain verwendet werden
     * @param message Ausgabe Nachricht
     * @param level 1-4 1=ERROR,2=WARN,3=INFO (default),4=DEBUG
     * @param ms Angabe der Zeit in Millisekunden
     * @param domain Zeiger auf einen String der die Domäne repräsentiert (z.b RECV)
     * @param subdomain Zeiger auf einen String der Subdomäne (z.B. WRITE)
     * @param cr Default True, zeilenumbruch wird eingefügt
     */
    void message(const char *message, uint8_t level = 3, unsigned long ms = millis(), const char *domain = "", const char *subdomain = "", bool cr = true);

    /**
     * @brief Override info-methode zur ausgabe des Channel-Mappings
     * @param chMap, Zeiger auf uint8_t Channel-Mapping Array
     * @param ms Angabe der Zeit in Millisekunden
     * @param cr Default True, zeilenumbruch wird eingefügt
     */
    void info(const uint8_t *chMap, unsigned long ms = millis(), bool cr = true)
    {
        logCHMapping(chMap, ms, cr);
    }

    /**
     * @brief Erzeugt ein Bit-Pattern und schriebt diesen in buffer
     * @param buffer Ausgabe puffer
     * @param v Wert der in ein Bit-Muster umgewandelt wird
     * @param pattern Angabe wie das Bit-Muster dargestellt werden soll.
     * "<" nur LSB (Lower Nibble)
     * ">" nur MSB (Higer Nibble)
     * " " MSB<blank>LSB
     * "-" MSB-LSB
     * oder eigenes Pattern
     */
    static void getBinary(char *buffer, const uint8_t v, const char *pattern = " %c%c%c%c %c%c%c%c");

    void printBinary8(const char *domain, const char *subdomain, const uint8_t v, const char *pattern = " %c%c%c%c %c%c%c%c");

private:
    void logSpecificData(const TDataAll tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);
    void logSpecificData(const TDataRC tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);
    void logSpecificData(const TDataHover tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);
    void logSpecificData(const TDataSurface tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);
    void logSpecificData(const TDataStatus tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);
    void logSpecificData(const TESP32Memory tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);

    void logSpecificData(const TDataSensors tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);
    void logSpecificData(const TSensorCFG tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP = false, bool cr = true);

    void logSpecificData2(const TDataGlobal, unsigned long ms, bool cr = true);
    void logSpecificData2(const TDataStatus, unsigned long ms, bool cr = true);
    void logSpecificData2(const TESP32Memory, unsigned long ms, bool cr = true);

    void logCHMapping(const uint8_t *chMap, unsigned long ms, bool cr);

private:
    void getLogLevelString(char *level_str, uint8_t level);
    void getBoolString(char *arm_str, uint8_t level, const char *TRUE = "ARM", const char *FALSE = "DIS");

    /**
     * @brief Konvertieren eines Bytes (0-255) in eine Binäre-Darstellung (für Status-Anzeigen)
     * @param buffer: Zielpuffer in dem das Binär-Muster geschrieben wird
     * @param v: uint8_t Wert
     * @param pattern: Das erste Zeichen des Patterns kann zur Steuerung bestimmter Anzeigen verwendet werden
     * '<' zeigt nur 4-Bit (LSB) des Wertes (v)
     * '>' zeigt 4-Bit (MSB) des Wertes (v)
     * ' ' zeigt 8-Bit MSB und LSB mit einem Blank getrennt (default)
     * '-' zeigt 8-Bit MSB und LSB mit einem - getrennt
     * wenn keins der obigen Zeichen verwendet wird, wird das übergebene Pattern als Ausgabe-Pattern verwendet

    */

private:
    char buffer[200];
    bool _useUDP = true;
};

extern Logger *logger;

#endif