# PODRacer Tasks
Die PODRacer Firmware basiert auf dem ESP32 und nutz FreeRTOS als grundlage um nebenläufige Task verwenden zu können. Diese laufen quasi parallel und es ziel ist es eine Loop-Rate von 10ms zu erlauben.

## main.cpp

## ReceiverTask
### Beschreibung

### genutzte Sensoren
kein klassischer Sensor sondern eine separate SBUS-Klasse. Diese Klasse ist dafür verantwortlich den SBUS-Receiver auszulesen (read) bzw. wieder Daten zu versenden (write)

### READ - Input Daten
Nutzt die Daten aus der SBUS-Klasse und schreibt diese in die lokale TDataRC Struktur. Zusätzlich werden noch die Bool-Attribute lost_frames, fail_safe und is_armed gesetzt.

Alls Signale werden kalibriert in einen Bereich von 1000-2000 um eine einheitliche Verarbeitung über alles Bereich hinweg zu gewährleisten

### READ - Output Daten
TDataRC, hier werden folgende Attribute gesetzt:
```
typedef struct __attribute__ ((packed)) {
  uint16_t raw_channels[NUMBER_OF_CHANNELS] = {0};      // raw values from receiver
  uint16_t new_channels[MIN_NUMBER_OF_CHANNELS] = {0};  // adjusted values to FC
  uint16_t gimbal_min = 1000;             // lowest gimbal value (calibrated)
  uint16_t gimbal_max = 2000;             // largest gimbal value (calibrated)
  uint16_t gimbal_mid = 1500;             // center postion
  bool fail_safe = false;
  bool lost_frame = false;
  bool is_armed = false; 
  bool prevent_arming = true;             // default true, arming not possible
  uint8_t status_mask = 0b00000000;       //
} TDataRC;
```

Die `TDataRC` wird in die Queue `queueReceiver` geschrieben

###  STATUS_MASK
siehe `readme_arming_logic.md`


### Besondere Informationen zum Task
Dieser Task hat zwei Funktionen `read` und `write` 

#### READ
iest SBUS-Signale und verpackt diese in `TDataRC` und verschickt sie über die Queue

#### WRITE
liest aus er `queueReceiver` eine Datenstruktur `TDataAll` und liest die new_channel[16] Werte und versendet sie an einen SBUS-Empfänger (hier Betaflight FC).

## HoverTask
### Beschreibung
Der HoverTask ist dafür verantwortlich um den PODRacer in eine stabile Schwebeposition zu halten. Hierzu nutzt er den OpticalFlow Sensor PMW3901 und wertet seine Driftbewegung. (ggf. wird noch ein MPU6500 hinzugefügt
)
in X/Y Richtung aus und berechnet daraus eine relative Roll/Pitch bewegung.
Der HoverTask rechnet nur dann, wenn Thrust (Impeller) auf 0 steht. Sobald der Pilot den Gimbal bewegt, ignoriert der Hover alle PMW3901 Ausgaben.

 ### genutzte Sensoren
Genutzt wird der PMW3901 Sensor. Dieser Sensor liefert ein X/Y Signal zurück, das eine Driftbewegung definiert.
Nachfolgend eine Beschreibung wie sich X/YWerte darstellen und entsprechend ausgewertet werden.


          <NORTH>
            -Y
            |
      +X-Y  |  -X-Y
    +X -----|------- -X
      +X+Y  |  -X+Y 
            |
            +Y
        <SOUTH>

Driftet der PODRacer z.B. nach rechts (OST), wird X kleiner, Y bleibt bei 0. Der PODRacer muss also gegensteuern und eine ROLL-Bewegung nach links (ROLL < 1500) machen.
Driftet der PODRacer z.B  nach rechts-oben (NORD-OST), X und Y verkleinern sich, Reaktion des PODRacers, PITCH < 1500, ROLL < 1500. Der PODRacer hebt etwas die Nase und kippt leicht nach links


### Input Daten
Der Sensor PMW3901 nutzt nachfolgende Struktur 
```
 typedef struct {
    double raw_x = 0.0;
    double raw_y = 0.0;
    double normalized_X = 0.0;
    double normalized_Y = 0.0;
    double setPointX = 0.0;
    double setPointY = 0.0;
} TSensorPMW3901;
```
Nur `setPointX` und `setPointY` sind relevant und stellen den SOLL-Wert für X und Y dar. In der Regel liegt der bei 0.0

### Output Daten
Zurück gegeben wird eine berechneter Wert für X+Y und wird in `raw_x` und `raw_y` gespeichert.

### Besondere Informationen zum Task


## SurfaceTask
### Beschreibung

### genutzte Sensoren

### Input Daten

### Output Daten

### Besondere Informationen zum Task


## MixerTask
### Beschreibung

### genutzte Sensoren

### Input Daten

### Output Daten

### Besondere Informationen zum Task