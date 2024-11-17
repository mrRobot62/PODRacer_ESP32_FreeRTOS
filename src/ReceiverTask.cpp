#include <Arduino.h>
#include <HardwareSerial.h>
#include "ReceiverTask.h"
#include "ReceiverSBUS.h"
#include "globals.h"
#include "MockReceiverSBUS.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xMutex;
TDataRC queueRCData;

void updateMaskBlinkPattern(uint8_t newMask, uint8_t ledIndex)
{
  // Zugriff auf die Bitmaske mit Mutex schützen
  if (xSemaphoreTake(xbitmaskBlinkMutex, portMAX_DELAY) == pdTRUE)
  {
    blink_mask[ledIndex] = newMask;
    xSemaphoreGive(xbitmaskBlinkMutex);
  }
}

void receiverTask(void *parameter)
{
  char buffer[50];
  // Beispiel: Setup für SBUS Empfang (angepasst auf deinen Empfänger)
  // HardwareSerial sbusSerial(2); // UART2 für SBUS
  // sbusSerial.begin();

  // nur das Interface wird als Pointer genutzt
  // dadurch haben wir die Möglichkeit problemlos auch Mock-Klassen zu compilieren
  IReceiver *receiver;

  // lokaler mutex um während des auslesens des SBUS-Receiver einen
  // lock setzen
  SemaphoreHandle_t xReceiverMutex;
  xReceiverMutex = xSemaphoreCreateMutex();

  //
  // lokaler Datenspeicher für RC-daten
  TDataRC *lokalRCData;
  TDataHover *lokalHoverData;
  //
  //
  uint8_t armingMask = 0b00000001;
  //                     ||||||||_____ Bit 0 : PreventArmingBit         1= Arming nicht möglich, 0=Arming möglich
  //                     |||||||______ Bit 1 : LastArming State         1= letzte Position des ArmingSw
  //                     ||||||_______ Bit 2 : Current ArmingState      1= ArmingSwitch = ON, 0=off
  //                     |||||________ Bit 3 : Thrust Min               1= Gimbal auf Min
  //                     ||||_________ Bit 4 : Throttle Min             1= Poti auf Min
  //                     |||__________ Bit 5 : Yaw CenterPostion        1= Gimbal in CP
  //                     ||___________ Bit 6 : Pitch CenterPosition     1= Gimbal in CP
  //                     |____________ Bit 7 : Roll CenterPosition      1= Gimbal in CP
  //
  //

  Serial.println("Receiver running...");
//
// Soll die Mock-Klasse oder die physikalische Sensor-Klasse eingebunden werden
//
#ifdef USE_MOCK_SBUS
  receiver = new MockReceiverSBUS(MOCK_DATA_MASK_SBUS); // in main.cpp kann nach bedarf MOCK_DATA_MASK_SBUS angepasst werden
#else
  // receiver = new ReceiverSBUS(nullptr, SBUS_RX_PIN, SBUS_TX_PIN);
  receiver = new ReceiverSBUS(SBUS_RX_PIN, SBUS_TX_PIN);
#endif

  while (!generalFreeRTOSError) // bei globalen Fehlern wird der Task abgebrochen
  {
    lokalRCData = new TDataRC();
    lokalHoverData = new TDataHover();
    lokalRCData->armingMask = armingMask;
    if (CHECK_BIT(lokalRCData->armingMask, BIT2) && CHECK_BIT(lokalRCData->armingMask, BIT1))
    {
      lokalRCData->is_armed = true;
    }
    // sprintf(buffer, "(1) >>>> is_armed: %d | recvStatus: %c%c%c%c", uint8_t(lokalRCData->is_armed), BYTE_TO_BINARY_LSB(lokalRCData->recvStatus));
    // Serial.println(buffer);

    // logger->info(*lokalRCData,millis(), "RECV","----1");
    //-----------------------------------------------------------------------------------------
    //  --- READ - SBUS Daten
    //-----------------------------------------------------------------------------------------
    //  immer den Receiver auslesen
    //  Serial.printf("RECEIVER lokalRCData: %d, %d, %d", lokalRCData->gimbal_max, lokalRCData->gimbal_mid, lokalRCData->gimbal_min);
    receiver->read(lokalRCData);

    if (CHECK_BIT_PATTERN(lokalRCData->armingMask, 0b11111101))
    {
      // Sonderkonstellation bei Reboot, Prevent steht dann auf ON, ARM-SW=ON, ARMING=OFF
      // Armen nicht erlaubt, Flag löschen
      lokalRCData->is_armed = false;
      SET_BIT(lokalRCData->armingMask, BIT0);
    }
    else if (
        CHECK_BIT_PATTERN(lokalRCData->armingMask, 0b11111000) == false &&
        CHECK_BIT(lokalRCData->armingMask, BIT2) &&
        CHECK_BIT_ZERO(lokalRCData->armingMask, BIT1))
    {
      // Konstellation: Copter wurde gearmet, ging nicht, weil nicht in Grundeinstellung
      // BIT 7-3 xxxxx
      // BIT 2 = 1 (ARM-SW)
      // BIT 1 = 0 (ARMING)
      // BIT 0 = 0 (Prevent OFF)
      // Bleibt gearmed aber Grundeinstellung wird durchgeführt
      // Erst durch DISARMEN kann das PreventFlag auf 0 gesetzt werden
      lokalRCData->is_armed = false;
      SET_BIT(lokalRCData->armingMask, BIT0);
    }

    else if (CHECK_BIT_PATTERN(lokalRCData->armingMask, 0b11111000) &&
             CHECK_BIT_PATTERN(lokalRCData->armingMask, 0b00000100) &&
             CHECK_BIT_ZERO(lokalRCData->armingMask, BIT0))
    {
      // Alle Preventig-Situationen sind überprüft worden.
      // Arming ist jetzt möglich
      lokalRCData->is_armed = true;
      SET_BIT(lokalRCData->armingMask, BIT1);
      CLEAR_BIT(lokalRCData->armingMask, BIT0);
    }
    else if (CHECK_BIT_ZERO(lokalRCData->armingMask, BIT2))
    {
      //
      // wenn der ARM-SW 0 ist , dann ARMING-Flag und PREVENT-FLAG zurücksetzen
      CLEAR_BIT(lokalRCData->armingMask, BIT0);
      CLEAR_BIT(lokalRCData->armingMask, BIT1);
    }

    // der HoverTask benötigt die aktuellen RawChannel Daten für seine Verarbeitung
    // raw_channel daten in Hover-Struktur kopieren
    memcpy(lokalHoverData->raw_channels, lokalRCData->raw_channels, sizeof(lokalHoverData->raw_channels));
    //--------------------------------------------------------------
    // aktuelle Daten in Queues senden
    //--------------------------------------------------------------
    //
    // wenn Mutex verfügbar, dann in Queue schreiben
    if (xSemaphoreTake(xReceiverMutex, MUTEX_WAIT_TIMEOUT) == pdTRUE)
    {
      // Mutex freigeben
      xSemaphoreGive(xReceiverMutex);

      xQueueSend(queueReceiver, lokalRCData, portMAX_DELAY);
      // xQueueSend(queueHoverRcv, lokalHoverData, portMAX_DELAY);

      if (CHECK_BIT(LOG_MASK_RECEIVER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_RECEIVER, LOGGING_RECV_READ))
      {
        logger->info(*lokalRCData, millis(), "RECV", "READ");
      }

      //--------------------------------------------------------------
      // Blink-Pattern
      //--------------------------------------------------------------
      if (lokalRCData->is_armed)
      {
        updateMaskBlinkPattern(0b00010001, 0); // LED_STATE, 100ms
        updateMaskBlinkPattern(0b00000000, 1); // LED_ERR1 off
        updateMaskBlinkPattern(0b00000000, 2); // LED_ERR2 off
      }
      else
      {
        if (CHECK_BIT(lokalRCData->armingMask, BIT0) || CHECK_BIT_PATTERN(lokalRCData->armingMask, 0b11111000) == false)
        {
          // disarmed, nicht in Grundeinstellung
          updateMaskBlinkPattern(0b00011000, 0); // LED_STATE & LED_ERR1 1000ms
          updateMaskBlinkPattern(0b00100001, 1); // LED_ERR1 fast
          updateMaskBlinkPattern(0b00000000, 2); // LED_ERR2 off
        }
        else 
        {
          // normales DISARM BlinkPattern
          updateMaskBlinkPattern(0b00011000, 0); // LED_STATE => 1000ms
          updateMaskBlinkPattern(0b00000000, 1); // LED_ERR1 off
          updateMaskBlinkPattern(0b00000000, 2); // LED_ERR2 off
        }

      }
    }
    else
    {
      logger->warn(*lokalRCData, millis(), "RECVR", "MUTEX");
    }

    armingMask = lokalRCData->armingMask & 0b00000111;

    //-----------------------------------------------------------------------------------------
    // --- WRITE - SBUS Daten
    //-----------------------------------------------------------------------------------------
    // reset rcData
    // memset(lokalRCData, 0, sizeof(&lokalRCData));
    // ab hier beginnt der WRITE-Teil des ReceiverTasks
    // wir prüfen ob ein Event-Bit vom Mixer gesetzt wurde, wenn ja, dann
    // liest der ReceiverTask die global TDataAll Struktur und schreibt die Daten
    // über SBUS.write() zum FlightController

    EventBits_t uxBits = xEventGroupGetBits(xEventGroup);
    // Prüfen, ob das spezifische BIT_RECEIVER gesetzt ist
    if (uxBits & BIT_RECEIVER)
    {
      if (xSemaphoreTake(xTDataAllMutex, MUTEX_WAIT_TIMEOUT) == pdTRUE)
      {
        receiver->write(&globalData.rc);
        xSemaphoreGive(xTDataAllMutex);
        if (CHECK_BIT(LOG_MASK_RECEIVER, LOGGING_BIT) && CHECK_BIT(LOG_MASK_RECEIVER, LOGGING_RECV_WRITE))
        {
          logger->info(globalData.rc, millis(), "RECV", "WRITE");
        }
        // Lösche das Ereignisbit, nachdem die Aktion abgeschlossen ist
        xEventGroupClearBits(xEventGroup, BIT_RECEIVER);
      }
    }
    else
    {
      logger->warn(*lokalRCData, millis(), "RECVW", "MUTEX");
    }
    //
    // den akutellen Status zwischenspeichern
    // lastGlobalStatus = globalData.status.status_pattern;
    delete (lokalRCData);
    delete (lokalHoverData);
    vTaskDelay(pdMS_TO_TICKS(LOOP_TIME));
  }
}
