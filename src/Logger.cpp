#include "Logger.h"


void Logger::getLogLevelString(char *level_str, uint8_t level) {
    // Initialisierung der Zeiger auf konstante Log-Level-Strings
    const char *logLevel;
    const size_t len = 5; // Feste Länge von 5 Zeichen

    switch (level) {
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


void Logger::getBoolString(char *arm_str, uint8_t level, const char* TRUE, const char* FALSE) {
    // Initialisierung der Zeiger auf konstante Log-Level-Strings
    const char *str;
    const size_t len = 3; // Feste Länge von 3 Zeichen

    switch (level) {
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

void Logger::logSpecificData(const TDataAll tdata, uint8_t level, const char* domain, const char* subdomain,bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataRC tdata, uint8_t level, const char* domain, const char* subdomain,bool useUDP, bool cr) {
    char lString[5+1];
    char arm[3+1];
    char prevent[3+1];
    char crlf = ' ';

    this->getLogLevelString(lString, level);
    this->getBoolString(arm, tdata.is_armed);
    this->getBoolString(prevent, tdata.prevent_arming, "YES","NO ");

    // <ms>|<level>|<domain>|<armed>|ch0-ch7|<lost_frame|<faile_safe|<cr>
    // 000000|INFO|10s|0/1|0000,0000,0000,...|0/1|0/1|

    crlf = ' ';
    if (cr) crlf = '\n';
    //  <ms>|<level>|<domain>|<subd>|<armed>|RAW: 6values|NEW: 6values...
    // "%5s|%5s|%5s|%1d|...
    sprintf(buffer, "%8d|%5s|%5s|%5s|%3s|%3s|RAW: %04d %04d %04d %04d %04d %04d|NEW: %04d %04d %04d %04d %04d %04d|%1d|%1d|%c",
        this->current_millis, lString, domain, subdomain, arm,prevent,
        tdata.raw_channels[ROLL],tdata.raw_channels[PITCH],tdata.raw_channels[YAW],tdata.raw_channels[THROTTLE],tdata.raw_channels[THRUST],tdata.raw_channels[ARMING],
        tdata.new_channels[ROLL],tdata.new_channels[PITCH],tdata.new_channels[YAW],tdata.new_channels[THROTTLE],tdata.new_channels[THRUST],tdata.new_channels[ARMING],
        tdata.lost_frame, tdata.fail_safe,
        crlf
    );

    Serial.print(buffer);
}

void Logger::logSpecificData(const TDataHover tdata, uint8_t level, const char* domain, const char*subdomain,bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataSurface tdata, uint8_t level, const char* domain, const char*subdomain,bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataStatus tdata, uint8_t level, const char* domain, const char*subdomain,bool useUDP, bool cr) {

}

