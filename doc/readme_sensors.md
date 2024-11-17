# Sensoren

## PMW3901
Der **PMW3901** ist ein Optical Flow-Sensor, der Bewegungen relativ zur Oberfläche unterhalb des Sensors misst. Er wird häufig in Drohnen und Robotikanwendungen eingesetzt, um stabile Positions- und Geschwindigkeitsinformationen ohne GPS zu ermöglichen. Der Sensor arbeitet mit einem integrierten Bildsensor und berechnet Bewegungsvektoren in Echtzeit.

### Technische Daten
| **Eigenschaft**       | **Technische Details**       |
| --------------------- | ---------------------------- |
| **Technologie**       | Optical Flow                 |
| **Messbereich**       | Bis zu 7,4 Meter pro Sekunde |
| **Auflösung**         | 400 DPI                      |
| **Bildrate**          | 120 Frames pro Sekunde (fps) |
| **Schnittstelle**     | SPI                          |
| **Betriebsspannung**  | 3,3 V                        |
| **Stromverbrauch**    | ~25 mA                       |
| **Temperaturbereich** | -20 °C bis +85 °C            |
| **Abmessungen**       | ~12,5 mm x 12,5 mm x 5 mm    |
| **Gewicht**           | ~1 Gramm                     |


### Hardware
|  PIN  | Info   |
| :---: | ------ |
|   5   | CS-PIN |
|       |        |

### Einfaches Software-Beispiel
```
#include <Adafruit_PMW3901.h>

// SPI-Pins für den ESP32
#define CS_PIN 5  // Chip Select (CS)

// PMW3901 Objekt erstellen
Adafruit_PMW3901 flowSensor = Adafruit_PMW3901();

void setup() {
    // Seriellen Monitor starten
    Serial.begin(115200);
    while (!Serial);

    // PMW3901 initialisieren
    if (!flowSensor.begin(CS_PIN)) {
        Serial.println("PMW3901 konnte nicht initialisiert werden!");
        while (1); // Halte das Programm an, falls der Sensor nicht gefunden wird
    }

    Serial.println("PMW3901 erfolgreich initialisiert");
}

void loop() {
    // Bewegungsdaten abfragen
    int16_t deltaX, deltaY;
    flowSensor.readMotionCount(&deltaX, &deltaY);

    // Bewegungsdaten ausgeben
    Serial.print("Delta X: ");
    Serial.print(deltaX);
    Serial.print(" | Delta Y: ");
    Serial.println(deltaY);

    delay(100); // 100 ms Abfrageintervall
}

```

### Implementierung PODRacer



## LIDAR TFMini
Der TFMini LiDAR ist ein kompakter Time-of-Flight (ToF)-Distanzsensor, der präzise Entfernungsmessungen ermöglicht. Der Sensor nutzt Infrarotlicht und misst Entfernungen basierend auf der Laufzeit eines Lichtimpulses. 

### Technische Daten
| **Eigenschaft**                 | **Technische Details**         |
| ------------------------------- | ------------------------------ |
| **Technologie**                 | Time-of-Flight (ToF)           |
| **Messbereich**                 | 30 cm bis 12 Meter             |
| **Genauigkeit**                 | ±1% bei Entfernungen < 6 Meter |
| **Messfrequenz**                | Bis zu 100 Hz                  |
| **Auflösung**                   | 1 cm                           |
| **Blickwinkel (FoV)**           | ~3,6°                          |
| **Lichtquelle**                 | 850 nm Infrarot-Laser          |
| **Kommunikationsschnittstelle** | UART, I²C (je nach Modell)     |
| **Betriebsspannung**            | 5 V                            |
| **Stromverbrauch**              | ~120 mA                        |
| **Temperaturbereich**           | -20 °C bis +60 °C              |
| **Abmessungen**                 | 42 mm x 15 mm x 16 mm          |
| **Gewicht**                     | ~6 Gramm                       |

### Hardware
|  PIN  | Info |
| :---: | ---- |
|       |      |
|       |      |

### Einfaches Software-Beispiel
Basierend auf die Arduino-Library TFMini
```
#include <TFMini.h>

// SoftwareSerial ist auf ESP32 nicht notwendig, daher nutze die Hardware-Serielle Schnittstelle
#define RX_PIN 16 // Empfangen (TX des Sensors)
#define TX_PIN 17 // Senden (RX des Sensors)

// TFMini-Sensor Objekt erstellen
TFMini tfmini;

void setup() {
    // Seriellen Monitor starten
    Serial.begin(115200);
    while (!Serial);

    // UART für den TFMini initialisieren
    Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

    // TFMini starten
    tfmini.begin(&Serial2);

    Serial.println("TFMini erfolgreich initialisiert!");
}

void loop() {
    // Daten vom Sensor lesen
    uint16_t distance = tfmini.getDistance(); // Entfernung in cm
    uint16_t strength = tfmini.getSignalStrength(); // Signalstärke
    int16_t temperature = tfmini.getTemperature(); // Temperatur in °C * 8

    // Überprüfen, ob die Messung gültig ist
    if (distance > 30 && distance < 12000) { // Gültiger Bereich: 30 cm bis 12 m
        Serial.print("Entfernung: ");
        Serial.print(distance);
        Serial.print(" cm | Signalstärke: ");
        Serial.print(strength);
        Serial.print(" | Temperatur: ");
        Serial.print(temperature / 8.0, 1);
        Serial.println(" °C");
    } else {
        Serial.println("Ungültige Messung");
    }

    delay(100); // Abfrageintervall
}
```

### Implementierung PODRacer


## TOF VL53L1
Time-of-Flight (ToF) Laserdistanzsensor.
Fortgeschrittene Version des VL53L0 mit höherer Reichweite, besserer Genauigkeit und konfigurierbarem Sichtfeld (Field of View, FoV).

### Technische Daten

| **Eigenschaft**         | **VL53L0**                  | **VL53L1**                   |
| ----------------------- | --------------------------- | ---------------------------- |
| **Technologie**         | Time-of-Flight (ToF)        | Time-of-Flight (ToF)         |
| **Maximale Reichweite** | Bis zu 2 Meter              | Bis zu 4 Meter               |
| **Genauigkeit**         | ±3% bei 1 Meter             | ±1% bei 4 Meter              |
| **Response Time**       | ~30 ms                      | ~20 ms                       |
| **Field of View (FoV)** | Fest (ca. 25°)              | Konfigurierbar (15° bis 27°) |
| **Stromverbrauch**      | Niedrig (~20 mA im Betrieb) | Moderat (~25 mA im Betrieb)  |
| **Abmessungen**         | 4.4 mm x 2.4 mm x 1 mm      | 4.9 mm x 2.5 mm x 1.56 mm    |
| **Schnittstelle**       | I²C                         | I²C                          |
| **Lichtquelle**         | 940 nm Laser                | 940 nm Laser                 |
| **Temperaturbereich**   | -20 °C bis +70 °C           | -20 °C bis +85 °C            |
| **Kosten (ungefähr)**   | Günstiger (~5–10 USD)       | Teurer (~10–15 USD)          |


### Hardware
|  PIN  | Info |
| :---: | ---- |
|       |      |
|       |      |

### Einfaches Software-Beispiel
Basierend auf die Aruduino-Library von Pololu

```
#include <Wire.h>
#include <VL53L1X.h>

// VL53L1X Objekt erstellen
VL53L1X sensor;

void setup() {
    // Seriellen Monitor starten
    Serial.begin(115200);
    while (!Serial);

    // I²C starten
    Wire.begin(21, 22); // SDA, SCL

    // Sensor initialisieren
    Serial.println("VL53L1X Initialisierung...");
    if (!sensor.init()) {
        Serial.println("VL53L1X konnte nicht initialisiert werden!");
        while (1); // Anhalten bei Fehler
    }

    // Messbereich und Timing einstellen
    sensor.setDistanceMode(VL53L1X::Long); // Long: Reichweite bis 4m
    sensor.setMeasurementTimingBudget(50000); // 50ms Timing-Budget
    sensor.startContinuous(50); // Messungen alle 50ms

    Serial.println("VL53L1X erfolgreich initialisiert!");
}

void loop() {
    // Entfernung messen
    int distance = sensor.read();
    if (distance >= 0) {
        Serial.print("Entfernung: ");
        Serial.print(distance);
        Serial.println(" mm");
    } else {
        Serial.println("Fehler bei der Entfernungsmessung!");
    }

    delay(50); // Abfrageintervall
}

```

### Implementierung PODRacer

## MCU6500
Der MCU6500 ist ein fortschrittlicher Inertial Measurement Unit (IMU)-Sensor, der einen 6-Achsen-Bewegungssensor (3-Achsen-Gyroskop + 3-Achsen-Beschleunigungssensor) integriert. Dieser Sensor ist speziell für Anwendungen ausgelegt, die eine präzise Bewegungserfassung und -analyse erfordern.
    
### Technische Daten
| **Eigenschaft**                 | **Technische Details**                            |
| ------------------------------- | ------------------------------------------------- |
| **Sensortyp**                   | 6-Achsen IMU                                      |
| **Komponenten**                 | 3-Achsen-Gyroskop, 3-Achsen-Beschleunigungssensor |
| **Gyroskopbereich**             | ±250, ±500, ±1000, ±2000 °/s                      |
| **Beschleunigungsbereich**      | ±2, ±4, ±8, ±16 g                                 |
| **Auflösung**                   | Gyro: 16 bit, Accel: 16 bit                       |
| **Abtastrate**                  | Bis zu 1 kHz                                      |
| **Kommunikationsschnittstelle** | I²C, SPI                                          |
| **Betriebsspannung**            | 1,8 V – 3,6 V                                     |
| **Stromverbrauch**              | <1 mA (normaler Betrieb)                          |
| **Temperaturbereich**           | -40 °C bis +85 °C                                 |
| **Abmessungen**                 | 3 mm x 3 mm x 0,9 mm                              |
| **Besondere Funktionen**        | Eingebaute Sensorfusion, Bewegungsverarbeitung    |

### Hardware
|  PIN  | Info |
| :---: | ---- |
|       |      |
|       |      |

### Einfaches Software-Beispiel
Basierend auf die Library MPU6050 von Jeff Rowberg
```

#include <Wire.h>
#include <MPU6050.h>

// Objekt für den Sensor erstellen
MPU6050 sensor;

void setup() {
    // Seriellen Monitor starten
    Serial.begin(115200);
    while (!Serial);

    // I²C starten
    Wire.begin(21, 22); // SDA, SCL

    // Sensor initialisieren
    Serial.println("MCU6500 Initialisierung...");
    if (!sensor.testConnection()) {
        Serial.println("Sensor konnte nicht gefunden werden!");
        while (1);
    }
    Serial.println("Sensor erfolgreich initialisiert!");

    // Sensor-Konfiguration
    sensor.initialize();
}

void loop() {
    // Bewegungsdaten auslesen
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    sensor.getAcceleration(&ax, &ay, &az); // Beschleunigungsdaten
    sensor.getRotation(&gx, &gy, &gz);    // Gyroskopdaten

    // Daten ausgeben
    Serial.print("Beschleunigung [X, Y, Z]: ");
    Serial.print(ax);
    Serial.print(", ");
    Serial.print(ay);
    Serial.print(", ");
```

### Implementierung PODRacer
