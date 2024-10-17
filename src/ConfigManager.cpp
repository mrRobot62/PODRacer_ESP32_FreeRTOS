#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "ConfigManager.h"

Preferences preferences;

String namespaces[] = {
    "general",
    "hovertask",
    "surfacetask",
    "opticalflowtask",
    "mixertask",
    "receivertask",
    "blinktask"
};

// Definiere einen Enum für die verschiedenen Datentypen
typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
} ValueType;

// Definiere eine Union, die verschiedene Datentypen aufnehmen kann
typedef union {
    int i_val;
    double d_val;
    char s_val[50];
} Value;

// Definiere eine Struktur für Key/Value-Paare mit Namespace und flexiblem Wert
typedef struct {
    char key[50];
    char nspace[10];  // Der Namespace mit einer Länge von maximal 10 Bytes
    Value value;         // Union für den Wert
    ValueType type;      // Typ des Wertes
} KeyValuePair;


// Ändere auch hier 'namespace' in 'namespaceName'
void writeConfig(const char* namespaceName, const char* key, int value) {
    preferences.begin(namespaceName, false);
    preferences.putInt(key, value);
    preferences.end();
}

int readConfig(const char* namespaceName, const char* key, int defaultValue) {
    preferences.begin(namespaceName, true);
    int value = preferences.getInt(key, defaultValue);
    preferences.end();
    return value;
}

//------------------------------------------------------------
// Reset alle Preferences und fülle mit Default Werten
//------------------------------------------------------------
void resetAllPreferences() {

}

#endif