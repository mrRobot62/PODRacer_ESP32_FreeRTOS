#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "WebServerTask.h"
#include <Preferences.h>
#include "data_struct.h"

//#include "ConfigManager.h"
#include <ArduinoJson.h>  // Füge dies hinzu für DynamicJsonDocument

// Access Point Einstellungen
const char* ssid = "PODRacer_AP";
const char* password = "12345678";

// UDP Einstellungen
WiFiUDP udp;
const int udpPort = 4210;
const char* udpBroadcastAddress = "192.168.4.255";  // Broadcast-Adresse für das ESP32-Netzwerk



void WiFiEvent(WiFiEvent_t event);

// Globale Variablen für den Webserver
AsyncWebServer server(80);

// Deklariere `preferences` als extern, damit es in anderen Dateien verwendet werden kann
//extern Preferences preferences;

QueueHandle_t queueWebServer;

// Funktionen
void setupWiFiAP() {
    Serial.println("Setting up Access Point...");
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

template<typename T>
void sendData(const T *data, size_t byteLength) {
//    udp.beginPacket(udpBroadcastAddress, udpPort);
//    udp.write((uint8_t*)&tdataAll, dataSize);
//    udp.endPacket();
}

// Override for TDataRC
template<>
void sendData(const TDataRC *data, size_t byteLength) {

}

// Override for TDataOFlow
template<>
void sendData(const TDataOFlow *data, size_t byteLength) {

}

// Override for TDataSurface
template<>
void sendData(const TDataSurface *data, size_t byteLength) {

}

// Override for TDataStatus
template<>
void sendData(const TDataStatus *data, size_t byteLength) {

}


// Eine Liste von Beispiel-Namespaces (dynamisch erweiterbar)

// Sende die Liste der Namespaces an die HTML-Seite
void handleGetNamespaces(AsyncWebServerRequest *request) {
    // String response = "{\"namespaces\": [";
    // for (int i = 0; i < sizeof(namespaces) / sizeof(namespaces[0]); i++) {
    //     response += "\"" + namespaces[i] + "\"";
    //     if (i < sizeof(namespaces) / sizeof(namespaces[0]) - 1) {
    //         response += ", ";
    //     }
    // }
    // response += "]}";
    // request->send(200, "application/json", response);
}

// Lade Key-Value-Paare für den ausgewählten Namespace
void handleLoadConfig(AsyncWebServerRequest *request) {
    if (request->hasParam("namespace")) {
        String ns = request->getParam("namespace")->value();

        // Beispielkonfiguration
        String response = "{\"config\": [";
        // response += "{\"key\": \"setting1\", \"value\": \"" + preferences.getString("setting1", "default1") + "\"},";
        // response += "{\"key\": \"setting2\", \"value\": \"" + preferences.getString("setting2", "default2") + "\"}";
        // response += "]}";

        request->send(200, "application/json", response);
    } else {
        request->send(400, "text/plain", "Namespace not provided");
    }
}


// Speichere die Key-Value-Paare
void handleSaveConfig(AsyncWebServerRequest *request) {
    if (request->hasParam("plain", true)) {
        String body = request->getParam("plain", true)->value();

        DynamicJsonDocument doc(1024);  // Dynamische JSON-Allokation
        DeserializationError error = deserializeJson(doc, body);
        
        if (error) {
            request->send(400, "text/plain", "JSON parsing failed");
            return;
        }
        
        String ns = doc["namespace"];
        JsonObject config = doc["config"];

        // preferences.begin(ns.c_str(), false);
        // for (JsonPair kv : config) {
        //     preferences.putString(kv.key().c_str(), kv.value().as<String>());
        // }
        // preferences.end();

        // request->send(200, "text/plain", "Config saved");
    } else {
        request->send(400, "text/plain", "Invalid data");
    }
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void handleIndex(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
}

void handleConfig(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/config.html", String(), false);
}

void handleWifi(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/wifi.html", String(), false);
}

void webServerTask(void *parameter) {
    TDataAll dataAll;

    // Deaktivieren des persistenten Speichers für WiFi-Einstellungen
    WiFi.persistent(false);

    // Deaktiviert jegliche vorhandene WiFi-Verbindung und WPS
    WiFi.disconnect(true, true);

    // Access Point Modus aktivieren
    //WiFi.mode(WIFI_AP);

    // Initialisiere die Queue, bevor du sie verwendest
    queueWebServer = xQueueCreate(10, sizeof(int));
    if (queueWebServer == NULL) {
        Serial.println("Failed to create queue in webServerTask");
        vTaskDelete(NULL);
        return;
    }

    // Initialisiere SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
        return;
    }

    // Initialisiere WiFi im Access Point Modus
    WiFi.softAP("ESP32-AccessPoint", "password123");

    // Handle Web-Requests
    server.on("/", HTTP_GET, handleIndex);
    server.on("/config", HTTP_GET, handleConfig);
    server.on("/wifi", HTTP_GET, handleWifi);
    server.on("/save-config", HTTP_POST, handleSaveConfig);

    // Setze eine Standard-404 Seite
    server.onNotFound(notFound);

    // Starte den Server
    server.begin();

    // WLAN-Ereignisse überwachen
    WiFi.onEvent(WiFiEvent);  // Registrierung des WiFi-Event-Handlers


    // FreeRTOS Task läuft permanent, ohne Aktion
    while (true) {
        // Daten aus der Queue holen und senden
        Serial.printf("Aktuelle dataAll-Size: %d | ", sizeof(dataAll));
        if (xQueueReceive(queueWebServer, &dataAll, portMAX_DELAY) == pdTRUE) {
            sendData(&dataAll, sizeof(dataAll));
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_AP_STACONNECTED: {
            Serial.println("Client connected to the Access Point");
            break;
        }
        case SYSTEM_EVENT_AP_STADISCONNECTED: {
            Serial.println("Client disconnected from the Access Point");
            break;
        }
        default:
            Serial.printf("unknown WiFiEvent '%d'\n", int(event));
            break;
    }
}