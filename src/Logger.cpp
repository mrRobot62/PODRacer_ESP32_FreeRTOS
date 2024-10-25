#include "Logger.h"


void Logger::logSpecificData(const TDataAll tdata, uint8_t level, bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataRC tdata, uint8_t level, bool useUDP, bool cr) {
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
    sprintf(buffer, "%5s|%5s|%1d|%04d, %04d, %04d, %04d, %04d, %04d, %04d|%1d|%1d|%c",
        lString, "RECV", 
        tdata.channels[0],tdata.channels[1],tdata.channels[2],tdata.channels[3],
        tdata.channels[4],tdata.channels[5],tdata.channels[6],tdata.channels[7],
        tdata.lost_frame, tdata.fail_safe,
        crlf
    );

    Serial.print(buffer);
}

void Logger::logSpecificData(const TDataOFlow tdata, uint8_t level, bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataSurface tdata, uint8_t level, bool useUDP, bool cr) {

}

void Logger::logSpecificData(const TDataStatus tdata, uint8_t level, bool useUDP, bool cr) {

}

