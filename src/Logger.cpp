#include "Logger.h"

void Logger::getBinary(char *buffer, const uint8_t v, const char *pattern)
{

    if (pattern[0] == '>')
    {
        sprintf(buffer, "%c%c%c%c", BYTE_TO_BINARY_MSB(v));
        return;
    }
    if (pattern[0] == '<')
    {
        sprintf(buffer, "%c%c%c%c", BYTE_TO_BINARY_LSB(v));
        return;
    }
    if (pattern[0] == ' ')
    {
        sprintf(buffer, "%c%c%c%c %c%c%c%c", BYTE_TO_BINARY(v));
        return;
    }
    if (pattern[0] == '-')
    {
        sprintf(buffer, "%c%c%c%c-%c%c%c%c", BYTE_TO_BINARY(v));
        return;
    }
    sprintf(buffer, pattern, BYTE_TO_BINARY(v));
}

void Logger::getLogLevelString(char *level_str, uint8_t level)
{
    // Initialisierung der Zeiger auf konstante Log-Level-Strings
    const char *logLevel;
    const size_t len = 5; // Feste Länge von 5 Zeichen

    switch (level)
    {
    case 1:
        logLevel = "ERROR";
        break;
    case 2:
        logLevel = "WARN "; // Leerzeichen hinzufügen, um Länge 5 zu erreichen
        break;
    case 3:
        logLevel = "INFO "; // Leerzeichen hinzufügen, um Länge 5 zu erreichen
        break;
    case 4:
        logLevel = "DEBUG";
        break;
    default:
        logLevel = "?????";
        break;
    }

    // Kopiert genau 5 Zeichen in den Zielpuffer
    memcpy(level_str, logLevel, len);
    level_str[len] = '\0'; // Null-Terminierung
}

void Logger::getBoolString(char *arm_str, uint8_t level, const char *TRUE, const char *FALSE)
{
    // Initialisierung der Zeiger auf konstante Log-Level-Strings
    const char *str;
    const size_t len = 3; // Feste Länge von 3 Zeichen

    switch (level)
    {
    case 0:
        str = FALSE;
        break;
    case 1:
        str = TRUE;
        break;
    default:
        str = "---";
        break;
    }

    // Kopiert genau 5 Zeichen in den Zielpuffer
    memcpy(arm_str, str, len);
    arm_str[len] = '\0'; // Null-Terminierung
}

void Logger::logSpecificData(const TDataAll tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP, bool cr)
{
}

void Logger::logSpecificData(const TDataRC tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP, bool cr)
{
    char lString[5 + 1];
    char arm[3 + 1];
    char prevent[3 + 1];
    char crlf = ' ';
    char binBuffer[10];
    ;

    this->getLogLevelString(lString, level);
    this->getBoolString(arm, tdata.is_armed);

    crlf = ' ';
    if (cr)
        crlf = '\n';
    //  <ms>|<level>|<domain>|<subd>|<armed>|[status_pattern]|RAW: 6values|NEW: 6values...|lost_frame|fail_safe
    // getBinary(binBuffer, tdata.recvStatus, "<");
    getBinary(binBuffer, tdata.armingMask, "%c%c%c%c%c %c%c%c");
    // getBinary(binBuffer, 0b10001111, ">");
    //    sprintf(buffer, "%8d|%5s|%5s|%5s|%3s|[%s]|Raw:%04d %04d %04d %04d %04d %04d %04d %04d|NEW: %04d %04d %04d %04d %04d %04d %04d %04d|%1d|%1d|%c",
    sprintf(buffer, "%8d|%5s|%5s|%5s|%3s|[%s]|Raw:%04d %04d %04d %04d %04d x x %04d|NEW: %04d %04d %04d %04d %04d x x %04d|%1d|%1d|%c",
            ms, lString, domain, subdomain, arm, binBuffer,
            // tdata.raw_channels[ch_map[ROLL]], tdata.raw_channels[ch_map[PITCH]], tdata.raw_channels[ch_map[THROTTLE]], tdata.raw_channels[ch_map[YAW]], tdata.raw_channels[ch_map[ARM]], tdata.raw_channels[ch_map[AUX2]], tdata.raw_channels[ch_map[AUX3]], tdata.raw_channels[ch_map[HOVER]],
            // tdata.new_channels[ch_map[ROLL]], tdata.new_channels[ch_map[PITCH]], tdata.new_channels[ch_map[THROTTLE]], tdata.new_channels[ch_map[YAW]], tdata.new_channels[ch_map[ARM]], tdata.new_channels[ch_map[AUX2]], tdata.new_channels[ch_map[AUX3]], tdata.new_channels[ch_map[HOVER]],
            tdata.raw_channels[ch_map[ROLL]], tdata.raw_channels[ch_map[PITCH]], tdata.raw_channels[ch_map[THROTTLE]], tdata.raw_channels[ch_map[YAW]], tdata.raw_channels[ch_map[ARM]], tdata.raw_channels[ch_map[HOVER]],
            tdata.new_channels[ch_map[ROLL]], tdata.new_channels[ch_map[PITCH]], tdata.new_channels[ch_map[THROTTLE]], tdata.new_channels[ch_map[YAW]], tdata.new_channels[ch_map[ARM]], tdata.new_channels[ch_map[HOVER]],
            tdata.lost_frame, tdata.fail_safe,
            crlf);

    Serial.print(buffer);
}

void Logger::logSpecificData(const TDataHover tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP, bool cr)
{
}

void Logger::logSpecificData(const TDataSurface tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP, bool cr)
{
}

void Logger::logSpecificData(const TDataStatus tdata, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool useUDP, bool cr)
{
    char lString[5 + 1];
    char crlf = ' ';
    char statusBuffer[10];
    ;
    char errorBuffer[10];
    ;

    crlf = ' ';
    if (cr)
        crlf = '\n';

    this->getLogLevelString(lString, level);

    getBinary(statusBuffer, tdata.status_pattern, " ");
    getBinary(errorBuffer, tdata.error_pattern, " ");

    //  <ms>|<level>|<domain>|<subd>|[status_pattern]|[status_pattern]|
    sprintf(buffer, "%8d|%5s|%5s|%5s|%3s|[%s]|%c",
            ms, lString, domain, subdomain,
            statusBuffer,
            errorBuffer,
            crlf);
    Serial.print(buffer);
}

// ----------------------------------------------------------------------------------------------------
// Logging von speziellen Strukturen (Global, Status, memory, ...)
// ----------------------------------------------------------------------------------------------------
void Logger::logSpecificData2(const TDataGlobal tdata, unsigned long ms, bool cr)
{
}

void Logger::logSpecificData2(const TDataStatus tdata, unsigned long ms, bool cr)
{
}

void Logger::logSpecificData2(const TESP32Memory tdata, unsigned long ms, bool cr)
{
    char crlf;
    crlf = ' ';
    if (cr)
        crlf = '\n';

    sprintf(buffer, "%8d| ESP-Heap | Free: %6d Bytes | MinFree: %6d Bytes|%c",
            ms,
            tdata.freeHeap,
            tdata.minFreeHeap,
            crlf);
    Serial.print(buffer);
}

// ----------------------------------------------------------------------------------------------------
// Logging ausschließlich des Channel-Mapping Arrays
// ----------------------------------------------------------------------------------------------------
void Logger::logCHMapping(const uint8_t *chMap, unsigned long ms, bool cr)
{
    char crlf;
    crlf = ' ';
    if (cr)
        crlf = '\n';
    char lString[5 + 1];
    this->getLogLevelString(lString, 2);
    sprintf(buffer, "%8d| %s | CH_MAPPING | ROLL=%02d | PITCH=%02d | THROTTLE=%02d | YAW=%02d | ARM=%02d | AUX2=%02d | AUX3=%02d | HOVER=%02d |%c",
            ms, lString,
            chMap[ROLL],
            chMap[PITCH],
            chMap[THROTTLE],
            chMap[YAW],
            chMap[ARM],
            chMap[AUX2],
            chMap[AUX3],
            chMap[HOVER],
            crlf);
    Serial.print(buffer);
}

// ----------------------------------------------------------------------------------------------------
// Logging einer einfachen message
// ----------------------------------------------------------------------------------------------------
void Logger::message(const char *message, uint8_t level, unsigned long ms, const char *domain, const char *subdomain, bool cr)
{
    char crlf;
    crlf = ' ';
    if (cr)
        crlf = '\n';

    char lString[5 + 1];
    this->getLogLevelString(lString, level);

    sprintf(buffer, "%8d|%s|%s|%s|%s|%c",
            ms,
            lString,
            domain,
            subdomain,
            message,
            crlf);
    Serial.print(buffer);
}

void Logger::printBinary8(const char *domain, const char *subdomain, const uint8_t v, const char *pattern)
{
    char binBuffer[20];
    getBinary(binBuffer, v, "%c%c%c%c%c %c%c %c");
    sprintf(buffer, "%s|%s|%s", domain, subdomain, binBuffer);
    Serial.println(buffer);
}