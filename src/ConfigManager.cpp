#include "ConfigManager.h"

ConfigManager::ConfigManager(uint8_t loglevel) {
    // Initialisierung falls notwendig
    this->loglevel = loglevel;
}

int ConfigManager::getAllNamespaces(char *namespaces[], size_t size) {
    size_t namespace_count = 0;

    // Iterator, um die NVS-Einträge zu durchsuchen
    nvs_iterator_t it = nvs_entry_find(NVS_DEFAULT_PART_NAME, NULL, NVS_TYPE_ANY);
    while (it != nullptr && namespace_count < size) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);

        // Überprüfen, ob der Namespace bereits im Array enthalten ist
        bool found = false;
        for (size_t i = 0; i < namespace_count; ++i) {
            if (strcmp(namespaces[i], info.namespace_name) == 0) {
                found = true;
                break;
            }
        }

        // Wenn der Namespace noch nicht im Array ist, füge ihn hinzu
        if (!found) {
            namespaces[namespace_count] = new char[strlen(info.namespace_name) + 1];
            strcpy(namespaces[namespace_count], info.namespace_name);
            namespace_count++;
        }

        it = nvs_entry_next(it);
    }

    return namespace_count;  // Gibt die Anzahl der gefundenen Namespaces zurück
}

int ConfigManager::getAllKeys(const char* namespace_name, String** keys, size_t* key_count) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Iterator für die Einträge im angegebenen Namespace
    nvs_iterator_t it = nvs_entry_find(NVS_DEFAULT_PART_NAME, namespace_name, NVS_TYPE_ANY);
    
    // Initialisiere Speicher für Keys
    size_t maxKeys = 10;
    *keys = (String*)malloc(maxKeys * sizeof(String));
    if (*keys == NULL) {
        Serial.println("Speicherallokierung fehlgeschlagen.");
        *key_count = 0;
        return;
    }

    *key_count = 0;
    
    // Iteriere durch die Einträge und sammle die Keys
    while (it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        if (*key_count >= maxKeys) {
            maxKeys *= 2;
            *keys = (String*)realloc(*keys, maxKeys * sizeof(String));
            if (*keys == NULL) {
                Serial.println("Speicher-Reallokierung fehlgeschlagen.");
                *key_count = 0;
                nvs_release_iterator(it);
                return;
            }
        }
        (*keys)[*key_count] = String(info.key);
        (*key_count)++;
        it = nvs_entry_next(it);
    }
    
    nvs_release_iterator(it);
    return 0; // Erfolgreich
}

int ConfigManager::getKeyValue(KeyValuePair &kvp) {
    return getKeyValue(kvp.nspace, kvp.key, kvp.value);
}

int ConfigManager::getKeyValue(const String& ns, String key, Value &value) {
    preferences.begin(ns.c_str(), true);
    switch (value.type) {
        case TYPE_INT: {
            value.i_val = preferences.getInt(key.c_str());
            if (loglevel >= 2) Serial.printf(">>GET NS: %s, Key: %s, VType: %d, V: %d \n", ns, key, value.type, value.i_val);
            break;
        }
        case TYPE_DOUBLE: {
            value.d_val = preferences.getDouble(key.c_str());
            if (loglevel >= 2) Serial.printf(">>GET NS: %s, Key: %s, VType: %d, V: %f\n", ns, key, value.type, value.d_val);
            break;
        }
        case TYPE_STRING: {
            value.s_val = preferences.getString(key.c_str());
            if (loglevel >= 2) Serial.printf(">>GET NS: %s, Key: %s, VType: %d, V: '%s'\n", ns, key, value.type, value.s_val.c_str());
            break;
        }
        case TYPE_BLOB: {
            size_t len = preferences.getBytesLength(key.c_str());
            value.blob_val.data = new uint8_t[len];
            value.blob_val.length = len;
            preferences.getBytes(key.c_str(), value.blob_val.data, len);
            printBlob(value);
            break;
        }
        default:
            preferences.end();
            return -1; // Fehlercode
            break;
    }
    preferences.end();
    return 0; // Erfolgreich
}

int ConfigManager::setKeyValue(KeyValuePair &kvp) {
    return setKeyValue(kvp.nspace, kvp.key, kvp.value);
}

int ConfigManager::setKeyValue(const String& ns, String key, Value &value) {
    preferences.begin(ns.c_str(), false);
    switch (value.type) {
        case TYPE_INT: {
            preferences.putInt(key.c_str(), value.i_val);
            if (loglevel >= 2) Serial.printf(">>SET NS: %s, Key: %s, VType: %d, V: %d\n", ns, key, value.type, value.i_val);
            break;
        }
        case TYPE_DOUBLE: {
            preferences.putDouble(key.c_str(), value.d_val);
            if (loglevel >= 2) Serial.printf(">>SET NS: %s, Key: %s, VType: %d, V: %f\n", ns, key, value.type, value.d_val);
            break;
        }
        case TYPE_STRING: {
            preferences.putString(key.c_str(), value.s_val);
            if (loglevel >= 2) Serial.printf(">>SET NS: %s, Key: %s, VType: %d, V: '%s'\n", ns, key, value.type, value.s_val.c_str());
            break;
        }
        case TYPE_BLOB: {
            preferences.putBytes(key.c_str(), value.blob_val.data, value.blob_val.length * value.blob_val.elements);
            if (loglevel >= 2) printBlob(value);
            break;
        }
        default:
            preferences.end();
            return -1; // Fehlercode
    }
    preferences.end();
    return 0; // Erfolgreich
}



void ConfigManager::deleteNamespace(const String& ns) {
    preferences.begin(ns.c_str(), false);
    preferences.clear();
    preferences.end();
}

bool ConfigManager::deleteAllNamespaces() {
    // Initialisiere NVS
    // esp_err_t err = nvs_flash_init();
    // if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     err = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(err);

    // // Handle für NVS-Partition holen
    // nvs_iterator_t it = nvs_entry_find(NVS_DEFAULT_PART_NAME, NULL, NVS_TYPE_ANY);
    // while (it != NULL) {
    //     nvs_entry_info_t info;
    //     nvs_entry_info(it, &info);
    //     it = nvs_entry_next(it);

    //     // Namespace und Key ausgeben (zum Debuggen)
    //     Serial.printf("Namespace '%s', Key '%s'\n", info.namespace_name, info.key);

    //     // Namespace löschen
    //     nvs_handle_t handle;
    //     err = nvs_open(info.namespace_name, NVS_READWRITE, &handle);
    //     if (err == ESP_OK) {
    //         nvs_erase_all(handle);
    //         nvs_commit(handle);
    //         nvs_close(handle);
    //     }
    // }
    esp_err_t err = nvs_flash_erase();
    Serial.printf("Alle Namespaces gelöscht. %d\n", err);
    return err == ESP_OK;

}