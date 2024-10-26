#include <Arduino.h>
#include "ConfigManager.h"
#include "ReceiverTask.h"
#include "SurfaceTask.h"
#include "MixerTask.h"
#include "BlinkTask.h"
#include "WebServerTask.h"
#include "Preferences.h"
#include "data_struct.h"
#include "Logger.h"

// ************************************************************************************************************
//  Espressif / Python stuff
// ************************************************************************************************************
// komplettes löschen des esp32 (sollte man mindestens 1x am Anfang machen.)
// python -m esptool --chip esp32 erase_flash

// update der Daten aus dem ./data folder (bei ersten mal notwendig, muss VOR dem flashen der Firmware durchgeführt werden.)
// pio run --target uploadfs

// update der firmware 
// pio run --target upload

// Espressif IDF nutzen
// 1. get_idf     // stellt das Environment entsprechend zur Verfügung
// 2. jetzt stehen "idf*.py" module zur Verfügung
//
// elf-File durchsuchen (bezogen auf einen Crash mit Adressangabe)
// xtensa-esp32-elf-addr2line -pfia -e .pio/build/esp32dev/firmware.elf <adressen>
// ************************************************************************************************************

//
// 
// Jeder Task bekommt seine eigene Queue
// Der Mixer liest all diese Queues und führt ein Update auf die globale TDataAll durch
//

Logger *logger;

ConfigManager globalCFG;

extern Preferences preferences;

// Definiere die Queues
QueueHandle_t queueReceiver;
QueueHandle_t queueSurface;
QueueHandle_t queueOFlow;
QueueHandle_t queueHover;
QueueHandle_t queueSteering;

TDataAll globalData;
SemaphoreHandle_t xTDataAllMutex;
EventGroupHandle_t xEventGroup;

void setup() {
  Serial.begin(115200);
  globalCFG.initFlash(false);       // explizit KEIN komplettes Löschen des Flashes durchführen

  logger = new Logger(&Serial, 115200, 3);

  // Erstelle die Queues
  queueReceiver = xQueueCreate(10, sizeof(TDataRC));
  queueSurface  = xQueueCreate(10, sizeof(TDataSurface));
  queueOFlow    = xQueueCreate(10, sizeof(TDataOFlow));
  queueHover    = xQueueCreate(10, sizeof(TDataHover));
  queueSteering = xQueueCreate(10, sizeof(TDataSteering));

  // EventGroup initialisieren
  xEventGroup = xEventGroupCreate();
  if (xEventGroup == NULL) {
      Serial.println("Failed to create event group");
      return;  // Bei Fehler kann das Programm hier sicher abbrechen oder neu starten
  }

  // ReceiverTask wird Core 0 zugewiesen (höchste Priorität)
  xTaskCreatePinnedToCore(receiverTask, "ReceiverTask", 4096, NULL, 2, NULL, 1);

  // DistanceTask wird Core 1 zugewiesen
  xTaskCreatePinnedToCore(surfaceTask, "SurfaceTask", 4096, NULL, 1, NULL, 1);

  // MixerTask wird Core 1 zugewiesen
  xTaskCreatePinnedToCore(mixerTask, "MixerTask", 4096, NULL, 1, NULL, 1);

  // Starte den WebServer als FreeRTOS-Task
  // vermutlich stimmt was nicht mit HEAP oder QUEUE größe, Runtime Fehler
  xTaskCreatePinnedToCore(webServerTask, "WebServerTask", 8192, NULL, 1, NULL, 0);

  // Starte BlinkTask mit niedriger Prio
  xTaskCreatePinnedToCore(blinkTask, "BlinkTask", 2024, NULL, 1, NULL, 1);
}

void loop() {
  // Hauptloop wird nicht benötigt, da FreeRTOS die Tasks managt
}
