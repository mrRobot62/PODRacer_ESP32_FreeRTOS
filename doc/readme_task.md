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
- uint16_t raw_channels[16]
- bool fail_safe
- bool lost_frame
- is_armed

Die `TDataRC` wird in die Queue `queueReceiver` geschrieben

###  


### Besondere Informationen zum Task
Dieser Task hat zwei Funktionen `read` und `write` 

#### READ
iest SBUS-Signale und verpackt diese in `TDataRC` und verschickt sie über die Queue

#### WRITE
liest aus er `queueReceiver` eine Datenstruktur `TDataAll` und liest die new_channel[16] Werte und versendet sie an einen SBUS-Empfänger (hier Betaflight FC).

## HoverTask
### Beschreibung

### genutzte Sensoren

### Input Daten

### Output Daten

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