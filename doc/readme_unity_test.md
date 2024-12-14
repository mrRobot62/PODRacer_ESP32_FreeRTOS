# Test
Verwendet wird die ESP32 Library "unity.h"

# Wichtige Begriffe und Vorgehensweisen für optimierte Unity-Tests auf einem ESP32

## **Wichtige Begriffe**
1. **Assertions**
   - Zentrale Elemente der Tests, die bestimmte Bedingungen überprüfen.
   - Beispiele: `TEST_ASSERT_EQUAL`, `TEST_ASSERT_TRUE`.

2. **Setup und Teardown**
   - Funktionen:
     - `setUp()` – Vorbereitung der Testumgebung.
     - `tearDown()` – Bereinigung nach jedem Test.

3. **Mocks und Stubs**
   - Platzhalter für Hardware-Funktionen oder externe Abhängigkeiten.
   - Ermöglichen unabhängige Tests ohne echte Hardware.

4. **Testgruppen**
   - Logische Gruppierung von Tests, um Funktionen strukturiert zu prüfen.

5. **Interrupt-Test**
   - Tests für Interrupt-Routinen (`IRAM_ATTR`) auf korrekte Verarbeitung.

6. **Timing und Delays**
   - Berücksichtigung von zeitkritischen Funktionen, z. B. Sensor-Lesungen.

7. **GPIO-Tests**
   - Testen von Pin-Funktionen wie `pinMode()`, `digitalWrite()`, `digitalRead()`.

8. **Testabdeckung (Coverage)**
   - Sicherstellen, dass alle Code-Pfade getestet werden, um eine hohe Testqualität zu gewährleisten.

---

## **Vorgehensweisen für optimierte Unity-Tests**

### **Allgemein**
1. **Minimaler Hardware-Einsatz**
   - Verwende Mocks, um Hardware-unabhängige Tests zu ermöglichen.

2. **Isolierte Tests**
   - Jeder Test sollte unabhängig von anderen laufen.

3. **Eingangsvalidierung**
   - Überprüfe Eingabewerte auf gültige Bereiche, um unerwartetes Verhalten zu vermeiden.

4. **Test der Randbedingungen**
   - Teste Grenzfälle wie minimale, maximale oder ungewöhnliche Eingabewerte.

5. **Priorisierung von kritischen Funktionen**
   - Beginne mit Tests der Kernfunktionen und erweitere auf weniger kritische Teile.

6. **Verwendung von `setUp()` und `tearDown()`**
   - Initialisiere Testdaten oder Hardwarekonfigurationen vor jedem Test.

7. **Zeitoptimierte Tests**
   - Vermeide lange Wartezeiten durch Simulationen oder kurze Delays.

8. **Speicherverbrauch überwachen**
   - Achte darauf, dass Testumgebungen nicht zu viel RAM oder Flash beanspruchen.

9. **Serielle Debug-Ausgabe**
   - Nutze `Serial.print()` für Debugging während der Tests.

10. **Automatisierte Tests**
    - Führe Tests automatisch mit `pio test` in PlatformIO aus.

---

### **ESP32-spezifisch**
1. **WiFi-Funktionen**
   - Simuliere Netzwerkverbindungen und teste Wiederverbindungen.

2. **Tasks und Multithreading**
   - Teste Funktionen in FreeRTOS-Tasks, z. B. Synchronisation.

3. **Sensoren und Aktoren**
   - Mocke Sensorausgaben und teste die Datenverarbeitung.

4. **Flash-Speicher (SPIFFS, LittleFS)**
   - Teste das Lesen und Schreiben von Dateien auf dem Flash.

5. **Kommunikationsschnittstellen**
   - Simuliere UART, I2C oder SPI für unabhängige Tests.

---

## **Zusammenfassung**
- **Isolierte und gut strukturierte Tests** erhöhen die Wartbarkeit.
- **Mocks und Stubs** erleichtern Tests ohne echte Hardware.
- **ESP32-spezifische Funktionen** wie WiFi und Tasks sollten speziell berücksichtigt werden.

Möchtest du zu einem Punkt ein Beispiel oder eine genauere Anleitung? 

# Test-Funktionen
Relevante Funktionen für das Testen mit Unity

| **Funktion**                      | **Beispiel**                                                    | **Info**                                                                   |
| --------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------------- |
| **`TEST_ASSERT`**                 | `TEST_ASSERT(x > 0);`                                           | Prüft, ob die Bedingung wahr ist.                                          |
| **`TEST_ASSERT_EQUAL`**           | `TEST_ASSERT_EQUAL(5, add(2, 3));`                              | Prüft, ob zwei Werte gleich sind.                                          |
| **`TEST_ASSERT_NOT_EQUAL`**       | `TEST_ASSERT_NOT_EQUAL(0, getStatus());`                        | Prüft, ob zwei Werte unterschiedlich sind.                                 |
| **`TEST_ASSERT_NULL`**            | `TEST_ASSERT_NULL(pointer);`                                    | Prüft, ob ein Zeiger `NULL` ist.                                           |
| **`TEST_ASSERT_NOT_NULL`**        | `TEST_ASSERT_NOT_NULL(pointer);`                                | Prüft, ob ein Zeiger nicht `NULL` ist.                                     |
| **`TEST_ASSERT_TRUE`**            | `TEST_ASSERT_TRUE(isReady());`                                  | Prüft, ob die Bedingung wahr ist.                                          |
| **`TEST_ASSERT_FALSE`**           | `TEST_ASSERT_FALSE(isError());`                                 | Prüft, ob die Bedingung falsch ist.                                        |
| **`TEST_ASSERT_EQUAL_HEX`**       | `TEST_ASSERT_EQUAL_HEX(0xFF, readRegister());`                  | Prüft, ob zwei Werte im Hexadezimalformat gleich sind.                     |
| **`TEST_ASSERT_EQUAL_STRING`**    | `TEST_ASSERT_EQUAL_STRING("hello", getString());`               | Prüft, ob zwei Strings gleich sind.                                        |
| **`TEST_ASSERT_EQUAL_INT_ARRAY`** | `TEST_ASSERT_EQUAL_INT_ARRAY(expectedArray, actualArray, len);` | Prüft, ob zwei Integer-Arrays gleich sind.                                 |
| **`TEST_IGNORE`**                 | `TEST_IGNORE();`                                                | Ignoriert den aktuellen Test.                                              |
| **`TEST_FAIL`**                   | `TEST_FAIL();`                                                  | Bricht den Test ab und markiert ihn als fehlgeschlagen.                    |
| **`UNITY_BEGIN`**                 | `UNITY_BEGIN();`                                                | Initialisiert den Testlauf.                                                |
| **`UNITY_END`**                   | `UNITY_END();`                                                  | Beendet den Testlauf und gibt die Ergebnisse aus.                          |
| **`RUN_TEST`**                    | `RUN_TEST(test_function);`                                      | Führt eine Testfunktion aus.                                               |
| **`setUp`**                       | `void setUp() { /* setup code */ }`                             | Wird vor jedem Test automatisch aufgerufen, um die Umgebung vorzubereiten. |
| **`tearDown`**                    | `void tearDown() { /* cleanup code */ }`                        | Wird nach jedem Test automatisch aufgerufen, um die Umgebung aufzuräumen.  |



# Unity-Attribute
| **Attribut**           | **Beispiel**                                                           | **Info**                                                                             |
| ---------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------------ |
| **`condition`**        | `TEST_ASSERT(x > 0);`                                                  | Eine Bedingung, die als wahr (`true`) evaluiert werden muss, damit der Test besteht. |
| **`expected`**         | `TEST_ASSERT_EQUAL(5, result);`                                        | Der erwartete Wert, mit dem der tatsächliche Wert verglichen wird.                   |
| **`actual`**           | `TEST_ASSERT_EQUAL(5, result);`                                        | Der tatsächliche Wert, der getestet wird.                                            |
| **`pointer`**          | `TEST_ASSERT_NOT_NULL(ptr);`                                           | Ein Zeiger, der getestet wird (z. B. ob er `NULL` ist oder nicht).                   |
| **`string`**           | `TEST_ASSERT_EQUAL_STRING("ok", str);`                                 | Ein String, der mit einem erwarteten String verglichen wird.                         |
| **`length`**           | `TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, 5);`                    | Gibt die Länge eines Arrays an, das verglichen werden soll.                          |
| **`setup`**            | `void setUp() { /* init code */ }`                                     | Funktion zur Vorbereitung der Testumgebung, wird vor jedem Test aufgerufen.          |
| **`teardown`**         | `void tearDown() { /* cleanup code */ }`                               | Funktion zur Bereinigung der Testumgebung, wird nach jedem Test aufgerufen.          |
| **`test_function`**    | `RUN_TEST(test_example);`                                              | Die spezifische Testfunktion, die ausgeführt werden soll.                            |
| **`UNITY_BEGIN()`**    | `UNITY_BEGIN();`                                                       | Initialisiert den Testlauf und bereitet die Ausgabe vor.                             |
| **`UNITY_END()`**      | `UNITY_END();`                                                         | Beendet den Testlauf und gibt die Ergebnisse aus.                                    |
| **`message`**          | `TEST_ASSERT_MESSAGE(false, "Fail");`                                  | Eine benutzerdefinierte Nachricht, die im Fehlerfall ausgegeben wird.                |
| **`assertion`**        | `TEST_ASSERT_EQUAL(expected, actual);`                                 | Eine Bedingung oder ein Vergleich, der geprüft wird.                                 |
| **`group`**            | Gruppiert Tests in einer strukturierten Test-Suite.                    | Hilft, Tests thematisch zu organisieren und auszuführen.                             |
| **`timeout`**          | Kann verwendet werden, um Tests abzubrechen, wenn sie zu lange dauern. | Relevant bei Zeitkritischen Funktionen.                                              |
| **`expected_failure`** | Nicht direkt unterstützt, kann mit `TEST_IGNORE()` simuliert werden.   | Markiert Tests, die absichtlich fehlschlagen sollen.                                 |
| **`loop`**             | `void loop() {}`                                                       | Kann für kontinuierliche Tests oder spezielle Testbedingungen verwendet werden.      |


# Beispiele

# Beispiel 1: GPIO-Tests

## Beschreibung
Dieses Beispiel testet die Konfiguration und den Zustand eines GPIO-Pins.

### **Code**
```cpp
#include <unity.h>

void test_gpio_output() {
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(5)); // Überprüft, ob der GPIO 5 HIGH ist
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_gpio_output);
    UNITY_END();
}

void loop() {}
```

# Beispiel 3: Mock eines Sensors

## Beschreibung
Dieses Beispiel simuliert einen Sensor und testet die Verarbeitung der Sensordaten.

### **Code**
```cpp
int add(int a, int b) {
    return a + b;
}

void test_add_function() {
    TEST_ASSERT_EQUAL(5, add(2, 3));  // Überprüft Addition von 2 + 3
    TEST_ASSERT_EQUAL(0, add(0, 0)); // Überprüft Addition von 0 + 0
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_add_function);
    UNITY_END();
}

void loop() {}
```

# Beispiel 4: Setup und Teardown

## Beschreibung
Dieses Beispiel zeigt, wie man setUp() und tearDown() verwendet, um Testdaten vor und nach einem Test zu verwalten.

### **Code**
```cpp
int global_var;

void setUp() {
    global_var = 42; // Initialisiert die globale Variable vor jedem Test
}

void tearDown() {
    global_var = 0; // Bereinigt die globale Variable nach jedem Test
}

void test_global_var() {
    TEST_ASSERT_EQUAL(42, global_var); // Überprüft den Initialwert
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_global_var);
    UNITY_END();
}

void loop() {}
```


# Beispiel 5: Interrupt-Test

## Beschreibung

Dieses Beispiel testet eine Interrupt-Routine, indem simuliert wird, dass ein Signal den Interrupt auslöst.

### Code
```cpp
volatile bool interrupt_flag = false;

void IRAM_ATTR mock_interrupt() {
    interrupt_flag = true; // Setzt das Flag bei Interrupt
}

void test_interrupt() {
    attachInterrupt(digitalPinToInterrupt(2), mock_interrupt, RISING);
    interrupt_flag = false;
    digitalWrite(2, HIGH); // Simuliert ein HIGH-Signal
    delay(10);             // Wartet auf den Interrupt
    TEST_ASSERT_TRUE(interrupt_flag); // Überprüft, ob der Interrupt ausgelöst wurde
    detachInterrupt(digitalPinToInterrupt(2));
}

void setup() {
    pinMode(2, OUTPUT);
    UNITY_BEGIN();
    RUN_TEST(test_interrupt);
    UNITY_END();
}

void loop() {}
```
