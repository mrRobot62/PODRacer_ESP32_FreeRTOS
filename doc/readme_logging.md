# Logging
Zentrale Logging-Klasse die pro Task spezielle Logausgaben (Format) ausgibt.

## Loglevel
Normalerweise werden unterschiedliche Loglevel bereitgestellt (ERROR/WARN/INFO/DEBUG). Da diese Abstufung aber im Echtenbetrieb wenig Sinn bringen, sind aktuell auch nur INFO-Meldungen implementiert.

Wichtiger sind Logausgaben für bestimmte Tasks ein oder auszublenden - die Logausgaben sind im prinzip nur während der Implementierung sinnvoll

## Log-Visibilität
Im Prinzip loggen alle Tasks separat ihre formatierten Logausgaben. Da die Tasks aber sehr schnell laufen, werden extrem viele Logausgaben produziert und das ist während der Implementierung hinderlich. Daher ist ein Bit-Log-Feld implementiert worden. Ist das Bit für den Jeweiligen Task/Klasse gesetzt, wird eine Logausgabe produziert, ist es nicht gesetzt, wird die Logausgabe ignoriert.

### Log-Bitmaskierung
Jeder Task hat eine eigene uint8_t Bitmaske zur Verfügung. Generell gilt, wenn Bit gesetzt, wird geloggt


| Task                | 7          | 6    | 5     | 4    | 3     | 2    | 1    | 0    | Info                                                                                                                     |
| ------------------- | ---------- | ---- | ----- | ---- | ----- | ---- | ---- | ---- | ------------------------------------------------------------------------------------------------------------------------ |
| Receiver            | Logging ON | READ | WRITE | Free | Free  | Free | Free | Free | Logging ReceiverTask, Zwei Methoden read(), write() die unterschiedlich loggen können                                    |
| Hover               | Logging ON | RAW  | CALC  | Free | Free  | Free | Free | Free | Logging HoverTask, RAW sind nur die Eingangswerte, CALC sind die berechneten Werte aus dem PMW3901                       |
| Surface             | Logging ON | RAW  | CALC  | Free | Free  | Free | Free | Free | Logging aus SurfaceTask,                                                                                                 |
| Mixer               | Logging ON | RIN  | HIN   | SIN  | STIN  | R5   | OUT  | STAT | Logging MixerTask, RIN(ReceiverIN), HIN=HoverIN, SIN=SurfaceIn, STIN=SteeringIn, R5=, OUT=ReceiverOUT, STAT=GlobalStatus |
| ComplementaryFilter | Logging ON | ALL  | SDIST | YAW  | DRIFT | IMU  | Free | Free | Logging ComplementaryFilter. SDIST=ToF/Lidar, YAW=OFlow YawComp, DRIFT=OFlow DriftXY, IMU=MPU6500                        |
|                     |            |      |       |      |       |      |      |      |                                                                                                                          |

**Beispiel Receiver**
- 0b11000000    => Logging ON, READ
- 0b11100000    => Logging ON, READ, WRITE
- 0b0xxxxxxx    => Logging OFF, egal wie alle anderen Bits sind