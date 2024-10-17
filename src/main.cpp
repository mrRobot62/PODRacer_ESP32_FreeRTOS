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
#include "RECVStruct.h" // Neu hinzufügen

// komplettes löschen des esp32
// python -m esptool --chip esp32 erase_flash


RECV_Struct RECV;  // Dies ist die Definition der globalen Struktur
extern Preferences preferences;

void setup() {
  Serial.begin(115200);

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
