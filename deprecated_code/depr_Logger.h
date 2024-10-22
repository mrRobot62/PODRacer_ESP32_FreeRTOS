#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <HardwareSerial.h>
#include "task_data.h"
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
    Logger(HardwareSerial *bus, int baud=115200, uint8_t level=3);

    /** set or reset the output format to visualizer-mode or human-readable-mode, based on parameter 'on'. Default is ON=1, OFF=0 **/
    void setVisualizerMode(bool on=1) {
      this->_visualizer_mode = on;
    }

    void setLoglevel(uint8_t level=3) {
      this->_level = level;
    }
    void error(const long v, const char *tname="?", bool cr=true) {
      sprintf(buffer, "%i", v);
      this->error(buffer, tname, cr);
    }

    void error(const char *text, const char *tname="?", bool cr=true) {
      if (_level >= 1) {
        this->_print(text, "ERROR", tname, cr);
      }
    }
    void warn(const long v, const char *tname="?", bool cr=true) {
      sprintf(buffer, "%i", v);
      this->warn(buffer, tname, cr);
    }

    void warn(const char *text, const char *tname="?", bool cr=true) {
      if (_level >= 2) {
        this->_print(text, "WARN", tname, cr);
      }
    }


    void info(const long v, bool allowLog, const char *tname="?", bool cr=true) {
      sprintf(buffer, "%i", v);
      this->info(buffer, allowLog, tname, cr);
    }

    void info(const double v, bool allowLog, const char *tname="?", bool cr=true) {
      sprintf(buffer, "%f", v);
      this->info(buffer, allowLog, tname, cr);
    }
    
    void info(const char *text, bool allowLog, const char *tname="?", bool cr=true) {
      if (_level >= 3 && allowLog) {
        this->_print(text, "INFO", tname, cr);
      }
    }

    void getBinary(char* buffer, const uint8_t v, uint8_t pattern=1);

    void print(const long v, bool cr=true) {
      sprintf(buffer, "%i", v);
      this->print(buffer, cr);
    }

    void print(const char *text, bool cr=false) {
      if (cr) {
        this->_bus->println(text);
      }
      else {
        this->_bus->print(text);
      }
    }

    void printBinary(const char *text, bool allowLog, const char* tname, const uint8_t v, bool cr=true) {
      info(text, allowLog, tname, false);
      sprintf(buffer, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(v));
      if (cr) {
        this->_bus->println(buffer);
      }
      else {
        this->_bus->print(buffer);
      }
    }

    void convertValueToBinary(char* buffer, size_t const size, void const * const ptr);


    void once_info(uint16_t *once_mask, uint8_t maskBit, const char *text, const char *tname="?", bool cr=true);
    void once_warn(uint16_t *once_mask, uint8_t maskBit, const char *text, const char *tname="?", bool cr=true);
    void once_error(uint16_t *once_mask, uint8_t maskBit, const char *text, const char *tname="?", bool cr=true);
    void once_data(uint16_t *once_mask, uint8_t maskBit,TaskData *td, const bool allowLog, const char *tname="?", const char *tgroup="-", bool printCH=true, bool printFData=false, bool printLData = false, bool pidData = false);
    void once_binary(uint16_t *once_mask, uint8_t maskBit,const char *text, const char* tname, const uint8_t v, bool cr=true);

    /** is used to log data as output for visualizer or human-readable **/

    void data(TaskData *data, const bool allowLog, const char *tname="?", const char *tgroup="-", bool printCH=true, bool printFData=false, bool printLData = false, bool pidData = false);

    /** create a 16Bit CRC sum from buffer **/
    uint16_t getCRC(const char *buf) {
      FastCRC16 CRC16;
      return CRC16.ccitt((uint8_t*)buf, strlen(buf));
    }

    /** add a 16Bit CRC sum at the end of buffer **/
    void addCRC2Buffer(char *buf) {
        char crc_buf[15];
        sprintf(crc_buf,",%08X", this->getCRC(buf));
        // put crc_buf at the end of buf-address
        memcpy(buf+strlen(buf), crc_buf, sizeof(crc_buf));
    }

    /** extend 'fromBuf' with 'toBuf' **/
    void addBuffer(char *toBuf, const char *fromBuf) {
      memcpy(toBuf+strlen(toBuf), fromBuf, sizeof(fromBuf));
    }

  private:
    HardwareSerial *_bus;
    uint8_t _level;
    char buffer [300];
    bool _visualizer_mode ;
    char tmp[300];

  private:
    void _print(const char *text, const char *LEVEL, const char *tname="?", bool cr=true) {
      memset(buffer, '\0', sizeof(buffer));
      sprintf(buffer,"%5s|%5s|%s", LEVEL, tname, text);
      if (cr) {
        this->_bus->println(buffer);
      }
      else {
        this->_bus->print(buffer);
      }
      this->_bus->flush();
    }

};

#endif