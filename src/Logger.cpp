#include "Logger.h"


void Logger::logSpecificData(const TDataAll tdata, uint8_t level, const char* domain, const char* subdomain,bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataRC tdata, uint8_t level, const char* domain, const char* subdomain,bool useUDP, bool cr) {
    char lString[5+1];
    char state[3+1];
    char crlf = ' ';

    switch (level) {
        case 1 : // error
            strcpy (lString, "ERROR");
            break;
        case 2 : // warnings
            strcpy (lString, "WARN");
            break;
        case 3 : // Info
            strcpy (lString, "INFO");
            break;
        case 4 : // debugging
            strcpy (lString, "INFO");
            break;
        default:
            strcpy (lString, "?????");        
    };
    // <ms>|<level>|<domain>|<armed>|ch0-ch7|<lost_frame|<faile_safe|<cr>
    // 000000|INFO|10s|0/1|0000,0000,0000,...|0/1|0/1|
    if (tdata.is_armed) {
        strcpy(state,"ARM");
    }
    else {
        strcpy(state,"DIS");
    }
    crlf = ' ';
    if (cr) crlf = '\n';
    //  <ms>|<level>|<domain>|<subd>|<armed>|RAW: 6values|NEW: 6values...
    // "%5s|%5s|%5s|%1d|...
    sprintf(buffer, "%8d|%5s|%5s|%5s|%1d|RAW: %04d %04d %04d %04d %04d %04d|NEW: %04d %04d %04d %04d %04d %04d|%1d|%1d|%c",
        this->current_millis, lString, domain, subdomain, tdata.is_armed,
        tdata.raw_channels[0],tdata.raw_channels[1],tdata.raw_channels[2],tdata.raw_channels[3],tdata.raw_channels[4],tdata.raw_channels[5],
        tdata.new_channels[0],tdata.new_channels[1],tdata.new_channels[2],tdata.new_channels[3],tdata.new_channels[4],tdata.new_channels[5],
        tdata.lost_frame, tdata.fail_safe,
        crlf
    );

    Serial.print(buffer);
}

void Logger::logSpecificData(const TDataOFlow tdata, uint8_t level, const char* domain, const char*subdomain,bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataSurface tdata, uint8_t level, const char* domain, const char*subdomain,bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataStatus tdata, uint8_t level, const char* domain, const char*subdomain,bool useUDP, bool cr) {

}

