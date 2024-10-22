#include "Logger.h"

Logger::Logger(HardwareSerial *bus, int baud, uint8_t level) {
    _bus = bus;
    _bus->begin(baud);
    _level = level;
    _visualizer_mode = 1;    
}

void Logger::convertValueToBinary(char* buffer, size_t const size, void const * const ptr) {
    unsigned char *b;
    unsigned char c;
    uint8_t idx;
    char x[64];

    for (size_t i = size; i > 0; i--) {
        b = ((unsigned char* )ptr+i);
        Serial.println(*b);
        for (int8_t j=7; j >= 0; j--) {
            c = (*b & 1);
            sprintf(x,"b(%d) i(%d) j(%d) idx(%d) c(%d)|", *b, i, j, idx, c);
            _bus->println(x);          
        }
    }
}

/************************************************************************/
/* once_xxxx methods print the statement only one time and set value    */
/* on address_value to 1. Up from this point, no additional loggings.   */
/* are possible. Nice feature inside loops in tasks                     */
/************************************************************************/

void Logger::once_info(uint16_t *once_mask, uint8_t maskBit, const char *text, const char *tname, bool cr) {
    (maskBit > 15)?maskBit=15:maskBit;
    if (bitRead(*once_mask, maskBit) == 0) {
    this->info(text, true, tname, cr);
    }
    bitSet(*once_mask, maskBit);
}

void Logger::once_warn(uint16_t *once_mask, uint8_t maskBit, const char *text, const char *tname, bool cr) {
    (maskBit > 15)?maskBit=15:maskBit;
    if (bitRead(*once_mask, maskBit) == 0) {
    this->warn(text, tname, cr);
    }
    bitSet(*once_mask, maskBit);
    
}

void Logger::once_error(uint16_t *once_mask, uint8_t maskBit, const char *text, const char *tname, bool cr) {
    (maskBit > 15)?maskBit=15:maskBit;
    if (bitRead(*once_mask, maskBit) == 0) {
    this->error(text, tname, cr);
    }
    bitSet(*once_mask, maskBit);
}

void Logger::once_data(uint16_t *once_mask, uint8_t maskBit,TaskData *td, const bool allowLog, const char *tname, const char *tgroup, bool printCH, bool printFData, bool printLData, bool pidData) {
    (maskBit > 15)?maskBit=15:maskBit;
    if (bitRead(*once_mask, maskBit) == 0) {
    this->data(td, allowLog, tname, tgroup, printCH, printFData, printLData, pidData);
    }
    bitSet(*once_mask, maskBit);
}

void Logger::once_binary(uint16_t *once_mask, uint8_t maskBit,const char *text, const char* tname, const uint8_t v, bool cr){
    (maskBit > 15)?maskBit=15:maskBit;
    if (bitRead(*once_mask, maskBit) == 0) {
        this->printBinary(text, true, tname, v, cr);
    }
    bitSet(*once_mask, maskBit);

}

void Logger::data(TaskData *data, const bool allowLog, const char *tname, const char *tgroup, bool printCH, bool printFData, bool printLData, bool pidData) {
    char tg[20];
    sprintf(tg,"%s_%s", tname, tgroup);

    memset(this->buffer, '\0', sizeof(this->buffer));
    memset(this->tmp, '\0', sizeof(this->tmp));

    if (this->_visualizer_mode == 1 && allowLog) {
    sprintf(this->buffer, "\nFEEF,%d,%d,%d,%s,%s,%s,%i,%i,%i,%i,%i,%i,%i,%i,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%i,%i,%i,%i,%i,%i,%i,%i,%.2f,%.2f,%.2f,%.2f,%.2f,%i,%i,%i",
    (long)data->data.start_millis,
    (long)data->data.end_millis,
    ((long)data->data.end_millis - (long)data->data.start_millis),
    tname,
    tgroup,
    tg,
    // channel data, can be absolut or relative, depends on task / group
    data->data.ch[0],
    data->data.ch[1],
    data->data.ch[2],
    data->data.ch[3],
    data->data.ch[4],
    data->data.ch[5],
    data->data.ch[6],
    data->data.ch[7],

    // typcalliy used for output from filters and other double values
    data->data.fdata[0],
    data->data.fdata[1],
    data->data.fdata[2],
    data->data.fdata[3],
    data->data.fdata[4],
    data->data.fdata[5],
    data->data.fdata[6],
    data->data.fdata[7],

    // long values typical for data to channels increas/decrease delta values
    data->data.ldata[0],
    data->data.ldata[1],
    data->data.ldata[2],
    data->data.ldata[3],
    data->data.ldata[4],
    data->data.ldata[5],
    data->data.ldata[6],
    data->data.ldata[7],

    // PID channels
    data->data.pid_rpyth[0],
    data->data.pid_rpyth[1],
    data->data.pid_rpyth[2],
    data->data.pid_rpyth[3],
    data->data.pid_rpyth[4],

    // constants (needed for visualizer)
    data->data.const_hover[0],
    data->data.const_hover[1],
    data->data.const_hover[2],
    data->data.const_hover[3]);
    addCRC2Buffer(buffer);
    print(buffer, true);
    }
    else {
    // human-readable-output
    if (allowLog) {
        if (printCH) {
        sprintf(tmp, 
            "(%3s)RUN(%3d) UPD:%d,ARM:%d,FAIL:%d,LF:%d, CH(R,P,Y,H,T,ARM,AUX2-3): %04d, %04d, %04d, %04d, %04d, %04d, %04d, %04d",
            tgroup,
            ((long)data->data.end_millis - (long)data->data.start_millis),
            data->data.updated,
            data->data.is_armed,
            data->data.failsafe,
            data->data.lost_frame,
            data->data.ch[0],
            data->data.ch[1],
            data->data.ch[2],
            data->data.ch[3],
            data->data.ch[4],
            data->data.ch[5],
            data->data.ch[6],
            data->data.ch[7]
        );
        info(tmp, allowLog, tname, true);
        }
        if (printLData) {
        sprintf(tmp, 
            "LONG (0-7):%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
            data->data.ldata[0],
            data->data.ldata[1],
            data->data.ldata[2],
            data->data.ldata[3],
            data->data.ldata[4],
            data->data.ldata[5],
            data->data.ldata[6],
            data->data.ldata[7]
        );
        info(tmp, allowLog, tname, true);
        }
        if (printFData) {
        sprintf(tmp, 
            "FLOAT(0-7):%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
            data->data.fdata[0],
            data->data.fdata[1],
            data->data.fdata[2],
            data->data.fdata[3],
            data->data.fdata[4],
            data->data.fdata[5],
            data->data.fdata[6],
            data->data.fdata[7]
        );
        info(tmp, allowLog, tname, true);
        }
        if (pidData) {
        sprintf(tmp, 
            "PID  (RPYTH):%d\t%d\t%d\t%d\t%d",
            data->data.pid_rpyth[0],
            data->data.pid_rpyth[1],
            data->data.pid_rpyth[2],
            data->data.pid_rpyth[3],
            data->data.pid_rpyth[4]
        );
        info(tmp, allowLog, tname, true);
        }
    } // allowLog
    }
}

/** convert v into a bit buffer array, pattern=0|1 default = 1 = '0000 0000' **/
void Logger::getBinary(char* buffer, const uint8_t v, uint8_t pattern) {
    switch(pattern) {
    case 0:
        sprintf(buffer, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(v));
        break;
    case 1:
        sprintf(buffer, BYTE_TO_BINARY_PATTERN2, BYTE_TO_BINARY(v));
        break;
    default:
        sprintf(buffer, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(v));
    }
    // return buffer;
}