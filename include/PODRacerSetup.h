#ifndef PODRACER_SETUP_H
#define PODRACER_SETUP_H

#include "ConfigManager.h"  // Importiere den generischen ConfigManager
#include "data_struct.h"
// Originales Makro
#define TO_STRING(x) #x

// zerlegt zum Beispiel data.gimbal_min und gibt nur gimbal_min zurück
// oder wenn ein Pointer genutzt wird dann gibt data->gimbal_min ebenfalls nur gimbal_min zurück
#define EXTRACT_FIELD_NAME(field) (strrchr(#field, '.') ? strrchr(#field, '.') + 1 : (strrchr(#field, '-') ? strrchr(#field, '>') + 1 : #field))

class PODRacerSetup {
public:
    PODRacerSetup(ConfigManager* configManager);  // Konstruktor mit ConfigManager

    // Methoden zur Konfiguration der Tasks
    void setDefaultValues(const char* namespace_name, TDataRC* data);
    void readValuesForTask(const char* namespace_name, TDataRC* data);

    void setDefaultValues(const char* namespace_name, TDataOFlow* data);
    void readValuesForTask(const char* namespace_name, TDataOFlow* data);

    void setDefaultValues(const char* namespace_name, TDataSurface* data);

private:
    size_t size;
    char *blob;

    ConfigManager* configManager;  // Verweis auf den ConfigManager
};

#endif // PODRACER_SETUP_H
