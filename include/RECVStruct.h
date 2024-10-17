#ifndef RECV_STRUCT_H
#define RECV_STRUCT_H

// Globale Struktur für RECV
struct RECV_Struct {
    int channel[16];
    bool armingAllowed;
    int armingState;
    bool isArmed;
};

// Deklariere die globale Struktur, die in mehreren Dateien verwendet wird
extern RECV_Struct RECV;

#endif
