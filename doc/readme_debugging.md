# Debugging mit ESP-PROG

# Allgemeines
Für **MAC** und **WINDOWS** gibt es zwei weitere Kapitel mit "Spezialitäten" ;-)

## platformio.ini
**Wichtig Informationen:**
- es macht Sinn sich für das Debuggen ein eigenes Environment zu erstellen (hier **[env:esp32dev_debug]**), weil dann die Möglichkeit besteht getrennt zwischen "release" und "debug" zu generieren. Im Debug-Environment unternimmt der Compiler keinerlei Optimierung und somit wird sichergestellt, das der Debugger korrekt funktioniert
- **debug_build_flags:** wichtige debug-compiler-flags
  - **-O0** vorhanden ist, hiermit werden explizit Optimierungen deaktiviert
  - **-gdwarf-2** bitte nicht dwarf-4 verwenden, das kann dazu führen, das lokale Variablen im Debugger nicht angezeigt werden
  - **-ggdb** Fügt vollständige Debug-Symbole hinzu
  - **-fno-inline** inline-statements werden nicht optimiert
  - **-fno-omit-frame-pointer** Erzwingt die Verwendung eines Frame-Pointers, was die Debugging-Informationen stabiler macht
  - 
- **platform_packages:** tool-openocd-esp32 @ ~2.1200.0, VSCode dazu zwingen auf diese Version für OpenOCD aufzusetzen. Ältere Versionen (z.B. 2.11) zeigen keine lokale Variablen an 
- **debug_speed:** hier muss man experimentieren was auf seiner HW läuft. Bei mir Mac M3 habe ich 20000 eingestellt und läuft stabil
- **upload_port**: wird automatisch ermittelt, daher muss man hier nichts eingeben/nutzebn
- **monitor_speed**: während des Debuggens funktioniert die Ausgabe über Serial.print() **NICHT**
-  
```

[env:esp32dev_debug]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
platform_packages = tool-openocd-esp32 @ ~2.1200.0
debug_tool = esp-prog 
upload_protocol = esp-prog
;upload_port = /dev/cu.usbserial-101
debug_init_break = tbreak mixerTask 
debug_speed = 5000 
build_type = debug 
build_unflags = -O1 -O2 -O3 -Os -s -Wl,-S

debug_build_flags = 
	-O0
	-g3
	-ggdb
	-DDEBUG
	-DCORE_DEBUG_LEVEL=5
;	-DARDUINO_RUNNING_CORE=0
	-DDEBUG_FREERTOS
    -fno-inline
    -fno-omit-frame-pointer
	-gdwarf-2

debug_extra_cmds =
    source /Users/bernhardklein/esp/esp-idf/gdbinit
    set substitute-path /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf /Users/bernhardklein/esp/esp-idf

build_flags = 
	-I include
	-fpermissive
	-DUSE_SPIFFS
	-DconfigUSE_TRACE_FACILITY=1
	-DconfigUSE_STATS_FORMATTING_FUNCTIONS=1

lib_extra_dirs = ~/Documents/Arduino/libraries
lib_deps = 
	SPIFFS
	frankboesing/FastCRC@^1.41
	br3ttb/PID@^1.2.1
	budryerson/TFMPlus@^1.5.0
	me-no-dev/AsyncTCP@^1.1.1
	mathieucarbou/ESPAsyncWebServer@^3.3.12
	bblanchon/ArduinoJson@^7.2.0
	bolderflight/Bolder Flight Systems SBUS@^8.1.4
	adafruit/Adafruit MPU6050@^2.2.6
board_build.filesystem = spiffs
```

## launch.json

Zum Debuggen nachfolgende Struktur in die launch.json hinzufügen.
für **Windows** bitte prüfen ob die ToolChain tatsächlich auch in diesem Folder unter dem User gespeichert ist. GGF. muss der Pfad angepasst werden.

> bitte den "activeEnv" Eintrag beachten, hier muss der Name des Debug-Environments eingetragen werden
>

``` 
    "activeEnv": "esp32dev_debug",
    "configurations": [
        {
            "type": "platformio-debug",
            "request": "launch",
            "name": "ESP32-DEBUG OSX",
            "executable": "${workspaceFolder}/.pio/build/{config:activeEnv}/firmware.elf",
            "toolchainBinDir": "${env:HOME}/.platformio/packages/toolchain-xtensa-esp32/bin",
            "internalConsoleOptions": "openOnSessionStart",
            "preLaunchTask": {
                "type": "PlatformIO",
                "task": "Pre-Debug"
            }
        },
        ...

``` 

# Starten des Debuggers
Im Hintergrund passiert innerhalb von Platformio folgendes:
- automatisch wird OpenOCD gestartet, daher ist es wichtig, das die neuste Version zur Verfügung steht. Siehe Kapitel: [platformio.ini](#platformio.ini)
- der GDB-Debugger wird automatisch gestartet
- Der ESP-PROG benötigt relativ lange bis man überhaupt debuggen kann - also Ruhe bewahren - es kann locker 60Sekunden dauern, bis der Breakpoint erreicht wird.
- Die Firmware wird compiliert und dann über den ESP-PROG automatisch auf den ESP32 geladen
- das Flag `debug_init_break = tbreak mixerTask` gibt an in welcher Funktion der Debugger automatisch anhalten soll. 
- im Code-Fenster können jetzt Breakpoints gesetzt werden
- im Watch-Fenster kann man sich nun Variablen zur Beobachtung hinzufügen
- wenn lokale Variablen als <undefined> angezeigt werden, liegt der Fehler mit sehr hoher Wahrscheinlichkeit an einer veralteten OpenOCD Version (prüfe genau DEBUG_CONSOLE output)
- 


# MacOS (OSX Sonoma)
Es muss kein expliziter FTDI-Treiber installiert werden. Der von Apple gelieferte Treiber funktioniert einwandfrei



# Windows