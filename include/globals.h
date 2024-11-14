#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// #define USE_MOCK_SBUS
#define USE_MOCK_PMW3901

//
// Bemekrung : BIT0-7 sind nicht die Position, sondern der Wert an der Postion
//  BIT7 = 0x80, BIT6 = 0x40 usw. Das macht die Auswertung und binär Operationen einfacher
//  als hart auf ein BitPosition (z.B. Position 7 zu gehen)
#define LOGGING_BIT BIT7
#define LOGGING_RECV_READ BIT6
#define LOGGING_RECV_WRITE BIT5

#define LOGGING_BIT BIT7
#define LOGGING_RECV_READ BIT6
#define LOGGING_RECV_WRITE BIT5
#define LOGGING_RECV_MISC BIT0

#define LOGGING_MIX_R1 BIT6
#define LOGGING_MIX_R2 BIT5
#define LOGGING_MIX_R3 BIT4
#define LOGGING_MIX_R4 BIT3
#define LOGGING_MIX_R5 BIT2
#define LOGGING_MIX_WRITE BIT1
#define LOGGING_MIX_STAT BIT0

#define LOGGING_HOVER_READ BIT6
#define LOGGING_HOVER_SEND BIT5
#define LOGGING_SURF_RAW BIT6
#define LOGGING_SURF_CALC BIT5

#define ARMING_BIT BIT0
#define PREVENT_BIT BIT1
#define CHANGE_BIT BIT2

extern uint8_t LOG_MASK_RECEIVER;
extern uint8_t LOG_MASK_MIXER;   //
extern uint8_t LOG_MASK_SURFACE; //
extern uint8_t LOG_MASK_HOVER;   //
extern uint8_t LOG_MASK_STATUS;  //

extern uint8_t MOCK_DATA_MASK_SBUS;
extern uint8_t MOCK_DATA_MASK_PMW3901;
extern uint8_t MOCK_DATA_MASK_TFMini;
extern uint8_t MOCK_DATA_MASK_VL53X1;

#define SBUS_RX_PIN 16
#define SBUS_TX_PIN 17

extern char __buf__[50];

/**
 * @brief setzt bit in mask auf 1
 */
#define SET_BIT(mask, bit) ((mask) |= (bit))

/**
 * @brief Löscht bit in mask auf 0
 */
#define CLEAR_BIT(mask, bit) ((mask) &= ~(bit))

/**
 * @brief prüft bit in mask. Gibt 1 zurück wenn gesetzt sonst 0
 */
#define CHECK_BIT(mask, bit) (((mask) & (bit)) != 0)

/**
 * @brief prüft ob bit in mask 0 ist, Gibt true(1) zurück wenn 0, sonst false (0)
 */
#define CHECK_BIT_ZERO(mask, bit) (((mask) & (bit)) == 0)

// Wechselt das angegebene Bit zwischen 0 und 1
/**
 * @brief wechselt das bit in mask. Aus 1 wird 0, aus 0 wird 1
 */
#define TOGGLE_BIT(mask, bit) ((mask) ^= (bit))

/**
 * @brief prüft ob Bits in pattern in mask gesetzt sind. Gibt true/false zurück
 * @return true, pattern-bits sind in mask gesetzt, sonst false
 */
#define CHECK_BIT_PATTERN(mask, pattern) (((mask) & (pattern)) == (pattern))

/**
 * @brief Setzt in mask an Position x das bit exakt wie an position y
 * @param mask in dieser Maske wird das Bit x gesetzt
 * @param targetBit Ziel Bit-Position die gesetzt wird
 * @param sourceBit Quell Bit-Position. Dieses Bit wird gelesen
 *
 */
#define SET_BIT_AS(mask, targetBit, sourceBit) \
    ((mask) = ((mask) & ~(targetBit)) | (((mask) & (sourceBit)) ? (targetBit) : 0))

/**
 * @brief COMPARE_BITS vergleicht zwei Bits, wenn beide gleich return true, sonst false
 * @param x : Wert der geprüft wird
 * @param b1 : Bitposition für Bit 1
 * @param b2 : Bitposition für Bit 2
 */
#define COMPARE_BITS(x, b1, b2) (((x & (b1)) != 0) == ((x & (b2)) != 0))

/**
 * @brief Setzt in mask das bit auf value
 * @param mask Maske die geändert wird
 * @param bit Bitposition die gesetzt wird
 * @param value Wert der gesetzt wird (0 oder 1)
 */
#define SET_BIT_VALUE(mask, bit, value) \
    ((mask) = ((mask) & ~(bit)) | ((value) ? (bit) : 0))

#endif