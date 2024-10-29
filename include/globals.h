#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define USE_MOCK_SBUS
#define USE_MOCK_PMW3901

#define LOGGING_BIT 7
#define LOGGING_RECV_READ 6
#define LOGGING_RECV_WRITE 5
#define LOGGING_MIX_S1 6
#define LOGGING_MIX_S2 5
#define LOGGING_MIX_S3 4
#define LOGGING_MIX_S4 3
#define LOGGING_HOVER_RAW 6
#define LOGGING_HOVER_CALC 5
#define LOGGING_SURF_RAW 6
#define LOGGING_SURF_CALC 5


extern uint8_t LOG_MASK_RECEIVER;
extern uint8_t LOG_MASK_MIXER;       //
extern uint8_t LOG_MASK_SURFACE;     //
extern uint8_t LOG_MASK_HOVER;       //

extern uint8_t MOCK_DATA_MASK_SBUS;
extern uint8_t MOCK_DATA_MASK_PMW3901;
extern uint8_t MOCK_DATA_MASK_TFMini;
extern uint8_t MOCK_DATA_MASK_VL53X1;


#endif