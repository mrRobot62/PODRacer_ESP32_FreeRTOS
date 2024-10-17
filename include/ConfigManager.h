#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Preferences.h>
#include <ArduinoJson.h>
#include <Arduino.h>

// Deklariere `preferences` als extern, damit es in anderen Dateien verwendet werden kann
extern Preferences preferences;
extern String namespaces[];

// Ändere 'namespace' in 'namespaceName' oder 'ns'
void writeConfig(const char* namespaceName, const char* key, int value);
int readConfig(const char* namespaceName, const char* key, int defaultValue);

#endif


