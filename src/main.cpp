#include <Arduino.h>
#include "ConfigManager.h"
#include "ReceiverTask.h"
#include "HoverTask.h"
#include "SurfaceTask.h"
#include "MixerTask.h"
#include "BlinkTask.h"
#include "WebServerTask.h"
#include "Preferences.h"
#include "data_struct.h"
#include "globals.h"
#include "Logger.h"
#include "MonitoringTask.h"
#include "SensorTask.h"
#include "Utils.h"

// ************************************************************************************************************
//  Espressif / Python stuff
// ************************************************************************************************************
// komplettes löschen des esp32 (sollte man mindestens 1x am Anfang machen.)
// python -m esptool --chip esp32 erase_flash

// Project - Clean durchführen
// pio run --target clean

// update der Daten aus dem ./data folder (bei ersten mal notwendig, muss VOR dem flashen der Firmware durchgeführt werden.)
// in der platform.ini muss folgender Einträge vorhanden sein:
// build_flags = -DUSE_SPIFFS
// board_build.filesystem = spiffs
//
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
QueueHandle_t queueHoverRcv, queueHoverSend;
QueueHandle_t queueSurface;
QueueHandle_t queueOFlow;
QueueHandle_t queueSteering;

TDataAll globalData;
SemaphoreHandle_t xTDataAllMutex;
SemaphoreHandle_t xbitmaskBlinkMutex; // Mutex zum Schutz der Bitmaske für den BlinkTask

// Sensor-Semaphoren
SemaphoreHandle_t xSemaSensorOFlow; // PMW3901
SemaphoreHandle_t xSemaSensorLidar; // TFPlus
SemaphoreHandle_t xSemaSensorTOF;   // VL53X1
SemaphoreHandle_t xSemaSensorIMU;   // MPU6xxxx
SemaphoreHandle_t xSemaSensorUS;    // Ultraschall
SemaphoreHandle_t xSemaSensors;     // Allgemeiner Semphar für SemaphorTask

EventGroupHandle_t xEventGroup;

//
// Logging-Pattern, detaillierte Beschreibung siehe readme_logging.md.
// Bit 7 ist generell für loggen ON(1)/OFF(0), Bit 6-0 sind indivduell
uint8_t LOG_MASK_RECEIVER = 0b01100000; // B6 = READ, B5=WRITE, B0=MISC
uint8_t LOG_MASK_MIXER = 0b11000010;    // B6 = R_IN
uint8_t LOG_MASK_SURFACE = 0b01100000;  //
uint8_t LOG_MASK_HOVER = 0b01100000;    //
uint8_t LOG_MASK_STATUS = 0b01000000;
uint8_t LOG_MASK_SENSOR = 0b01000000;

uint8_t LOG_MASK_COMPFLIT = 0b0000000;

// uint8_t MOCK_DATA_MASK_SBUS     =0b00000001;    // Verhalten 0, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS     = 0b00000010;   // Verhalten 1, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS = 0b00000100; // Verhalten 2, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS     =0b00001000;    // Verhalten 3, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS     =0b00010000;    // Verhalten 4, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS     =0b00100000;    // Verhalten 5, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS     =0b01000000;    // Verhalten 6, mehr Details siehe readme_mock.md
// uint8_t MOCK_DATA_MASK_SBUS     =0b10000000;    // Verhalten 7, mehr Details siehe readme_mock.md

uint8_t MOCK_DATA_MASK_PMW3901 = 0b00000001;
uint8_t MOCK_DATA_MASK_TFMINI = 0b00000001;
uint8_t MOCK_DATA_MASK_VL53X1 = 0b00000001;
uint8_t MOCK_DATA_MASK_HOVER = 0b00000001;
uint8_t MOCK_DATA_MASK_SURFACE = 0b00000001;
uint8_t MOCK_DATA_MASK_MIXER = 0b00000001;

// globale channel-mapping Variable, wird von allen genutzt die auf channels zugreifen müssen
uint8_t ch_map[NUM_CHANNELS] = {0, 1, 7, 3, 4, 2, 5, 6};

TSBUSGlobalDefaultValues gSBUSDefaultValues;
TSensorCFG sensorCFG;

uint8_t blink_mask[3];

bool generalFreeRTOSError;

char __buf__[50];

extern "C"
{
  extern const int uxTopUsedPriority;
  __attribute__((section(".rodata"))) const int uxTopUsedPriority =
      configMAX_PRIORITIES - 1;
}

void setup()
{

  generalFreeRTOSError = false;
  Serial.begin(115200);
  globalCFG.initFlash(false); // explizit KEIN komplettes Löschen des Flashes durchführen

  logger = new Logger(&Serial, 115200, 3);

  Serial.println("-----------------------------------------------------------------------------");
  Serial.println("--- PODRacer Firmware                                                     ---");
  Serial.println("-----------------------------------------------------------------------------");
  Serial.println("\nStarting.....\n");
  // MAP: 0=ROLL, 1=PITCH, 2=THROTTLE, 3=YAW, 4=ARM, 5+6=AUX, 7=HOVER
  uint8_t DEFAULT_CHANNEL_MAP[NUM_CHANNELS] = {0, 1, 7, 3, 4, 2, 5, 6};
  setChannelMapping(DEFAULT_CHANNEL_MAP);
  logger->info(ch_map);

  // Erstelle die Queues
  queueReceiver = xQueueCreate(QUEUE_SIZE, sizeof(TDataRC));
  queueSurface = xQueueCreate(QUEUE_SIZE, sizeof(TDataSurface));
  queueOFlow = xQueueCreate(QUEUE_SIZE, sizeof(TDataOFlow));
  queueHoverRcv = xQueueCreate(QUEUE_SIZE, sizeof(TDataHover));
  queueHoverSend = xQueueCreate(QUEUE_SIZE, sizeof(TDataHover));
  queueSteering = xQueueCreate(QUEUE_SIZE, sizeof(TDataSteering));

  // Mutex erstellen
  xbitmaskBlinkMutex = xSemaphoreCreateMutex();
  if (xbitmaskBlinkMutex == NULL)
  {
    logger->message("BlinkMutex nicht verfügbar", 1, millis(), "main", "Z125");
    while (1)
      ;
  }

  // EventGroup initialisieren
  xEventGroup = xEventGroupCreate();
  if (xEventGroup == NULL)
  {
    Serial.println("Failed to create event group");
    return; // Bei Fehler kann das Programm hier sicher abbrechen oder neu starten
  }
  Serial.println("\nEvent group ready.....\n");

  // ReceiverTask wird Core 0 zugewiesen (höchste Priorität)
  xTaskCreatePinnedToCore(receiverTask, "ReceiverTask", 4096, NULL, 2, NULL, 1);
  Serial.println("\nReceiver task created.....\n");

  // SensorTask, läuft auf CORE 1
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 4096, NULL, 2, NULL, 1);
  Serial.println("\nSensorTask task created.....\n");

  // Hover wird Core 1 zugewiesen
  xTaskCreatePinnedToCore(hoverTask, "HoverTask", 4096, NULL, 1, NULL, 1);
  Serial.println("\HoverTask task created.....\n");

  // DistanceTask wird Core 1 zugewiesen
  // xTaskCreatePinnedToCore(surfaceTask, "SurfaceTask", 4096, NULL, 1, NULL, 1);
  // Serial.println("\SurfaceTask task created.....\n");

  // MixerTask wird Core 1 zugewiesen
  xTaskCreatePinnedToCore(mixerTask, "MixerTask", 4096, NULL, 1, NULL, 1);
  Serial.println("\nMixerTask task created.....\n");

  // Starte den WebServer als FreeRTOS-Task
  // vermutlich stimmt was nicht mit HEAP oder QUEUE größe, Runtime Fehler
  // xTaskCreatePinnedToCore(webServerTask, "WebServerTask", 8192, NULL, 1, NULL, 0);
  // Serial.println("\WebServerTask task created.....\n");

  // Starte BlinkTask mit niedriger Prio
  // Parameter 1 : PIN, Parameter 2:
  static uint32_t led1Params[] = {LED_STATE, 0x00, 0b00010000};
  static uint32_t led2Params[] = {LED_ERR01, 0x01, 0b00100000};
  static uint32_t led3Params[] = {LED_ERR02, 0x02, 0b01000000};

  xTaskCreatePinnedToCore(blinkTask, "LED1Task", 2560, led1Params, 1, NULL, 1);
  xTaskCreatePinnedToCore(blinkTask, "LED2Task", 2560, led2Params, 1, NULL, 1);
  xTaskCreatePinnedToCore(blinkTask, "LED3Task", 2560, led3Params, 1, NULL, 1);
  Serial.println("\nLED1Task, LED2Task, LED3Task task created.....\n");

  // Erstelle den MonitoringTask mit einer moderaten Priorität
  // xTaskCreate(
  //     monitoringTask,   // Task-Funktion
  //     "MonitoringTask", // Name des Tasks
  //     4096,             // Stackgröße
  //     nullptr,          // Parameter für den Task (none)
  //     1,                // Task-Priorität
  //     nullptr           // Task-Handle (optional)
  // );
  // Serial.println("\nMonitoringTask task created.....\n");
}

void loop()
{
  // Hauptloop wird nicht benötigt, da FreeRTOS die Tasks managt
}
