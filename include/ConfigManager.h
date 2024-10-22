#ifndef CONFIGMANAGER2_H
#define CONFIGMANAGER2_H

#include <Preferences.h>
#include <Arduino.h>
#include <nvs.h>
#include <nvs_flash.h>

// Originales Makro
#define TO_STRING(x) #x

// zerlegt zum Beispiel data.gimbal_min und gibt nur gimbal_min zurück
// oder wenn ein Pointer genutzt wird dann gibt data->gimbal_min ebenfalls nur gimbal_min zurück
#define EXTRACT_FIELD_NAME(field) (strrchr(#field, '.') ? strrchr(#field, '.') + 1 : (strrchr(#field, '-') ? strrchr(#field, '>') + 1 : #field))

#define ARRAY_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BLOB
} ValueType;

// Definiere eine Union, die verschiedene Datentypen aufnehmen kann
typedef struct {
    ValueType type;
    int i_val;
    double d_val;
    String s_val;
    struct {
        uint8_t* data;
        size_t length;          // length of data
        size_t elements;        // number of elements inside data (array)
    } blob_val;
} Value;

// Definiere eine Struktur für Key/Value-Paare mit Namespace und flexiblem Wert
typedef struct {
    String key;
    String nspace;
    Value value;
} KeyValuePair;


class ConfigManager {
public:
    ConfigManager(uint8_t loglevel=2);
    void setLogLevel(uint8_t loglevel) {
        this->loglevel = loglevel;
    }

    void initFlash() {
        // NVS-Flash-Speicher initialisieren
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            // Keine freien Seiten oder neue NVS-Version gefunden
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(err);
    };

    int getAllNamespaces(char *namespaces[], size_t size);
    int getAllKeys(const char* namespace_name, String   **keys, size_t* key_count);
    int getKeyValue(KeyValuePair &kvp);
    int getKeyValue(const String& ns, String key, Value &value);
    int setKeyValue(KeyValuePair &kvp);
    int setKeyValue(const String& ns, String key, Value &value);

    void deleteNamespace(const String& ns);
    bool deleteAllNamespaces();

    void printBlob(const Value& value) {
        if (value.type == TYPE_BLOB) {
            Serial.print(">> (BLOB) ");
            for (size_t i = 0; i < value.blob_val.length; i++) {
                Serial.print(value.blob_val.data[i]); // Ausgabe in hexadezimal
                Serial.print(", ");
            }
            Serial.println(); // Neue Zeile nach der Ausgabe
        } else {
            Serial.println("Der Typ ist kein BLOB.");
        }
    }

    /**
     * @brief Convert an array into a byte stream
     * @param void* array: array to convert
     * @param length: length of this array (sizeof(array))
     * @param elementSize: number of entries in this array
     * @param **byteArray: convert into this uint8_t byte array
     * 
     * @return byteLength: number of converted bytes
     */
    void arrayToByteArray(const void* array, size_t length, size_t elementSize, uint8_t** byteArray, size_t* byteLength) {
        *byteLength = length * elementSize;
        *byteArray = (uint8_t*)malloc(*byteLength);
        Serial.printf("Length: %d, Elements: %d, ByteLength: %d \n", length, elementSize, *byteLength);
        if (*byteArray) {
            memcpy(*byteArray, array, *byteLength);
        }
    }

    /**
     * @brief convert a byte-stream into an array. Independendly of the target data type. 
     * @param *byteArray: convert this byte-stream
     * @param byteLenght: length of the byte-stream
     * @param *array: convert into this array. Can be any type of array
     */
    void byteArrayToArray(const uint8_t* byteArray, size_t byteLength,  void* array) {
        memcpy(array, byteArray, byteLength);
    }


    // Generische Serialisierungsfunktion für jede Struktur
    template <typename T>
    void serializeData(const T& data, char* byteStream, size_t *byteLength) {
        memcpy(byteStream, &data, sizeof(T));
        *byteLength = sizeof(T);
    }

    // Generische Deserialisierungsfunktion für jede Struktur
    template <typename T>
    void deserializeData(const char* byteStream, T& data, size_t *byteLength) {
        memcpy(&data, byteStream, sizeof(T));
        *byteLength = sizeof(data);
    }

private:
    Preferences preferences;
    uint8_t loglevel;
};

#endif // CONFIGMANAGER2_H
