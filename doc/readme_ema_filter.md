# EMAFilter Klasse für ESP32

Diese Dokumentation beschreibt die Implementierung und Verwendung der `EMAFilter`-Klasse, die einen Exponential Moving Average (EMA)-Filter auf einem ESP32 bereitstellt.

## Übersicht

Ein Exponential Moving Average (EMA)-Filter ist ein digitaler Filter, der auf eine Folge von Datenpunkten angewendet wird, um einen gleitenden Durchschnitt zu berechnen. Er gibt mehr Gewicht auf neuere Datenpunkte, wodurch er schneller auf aktuelle Änderungen reagiert als ein einfacher gleitender Durchschnitt (SMA).

### Formel für den EMA

Der EMA eines Datenpunkts $ x_t $ wird folgendermaßen berechnet:

$ EMA_t = \alpha \cdot x_t + (1 - \alpha) \cdot EMA_{t-1} $

wobei:
- $\alpha$ der Glättungsfaktor ist und zwischen 0 und 1 liegt.
- $x_t$ der aktuelle Datenpunkt ist.
- $EMA_{t-1}$ der vorherige EMA-Wert ist.

Je größer der Wert von $\alpha$, desto schneller reagiert der EMA auf Änderungen.

### Erklärung
**Glättungsfaktor ($\alpha$):** Der Parameter bestimmt, wie schnell der Filter auf Änderungen reagiert. Ein höherer Wert bedeutet eine schnellere Reaktion.

**Datenpunkte:** Diese Beispiele zeigen, wie der Filter auf unterschiedliche Typen von Daten angewendet wird, von Temperatursensoren bis hin zu Aktienkursen und Positionsdaten.
Diese Beispiele zeigen die Vielseitigkeit des EMA-Filters und wie er in verschiedenen Anwendungsfällen zur Daten-Glättung und Analyse verwendet werden kann. Probier's aus und schau, wie es funktioniert! 

# Beispiel Sourcecode
## header
```
#ifndef EMAFILTER_H
#define EMAFILTER_H

class EMAFilter {
public:
    EMAFilter(float alpha);
    void addDataPoint(float dataPoint);
    float getFilteredValue() const;

private:
    float alpha;       // Glättungsfaktor
    float ema;         // Aktueller EMA-Wert
    bool firstRun;     // Flag für den ersten Durchlauf
};

#endif // EMAFILTER_H

```
# cpp
```
#include "EMAFilter.h"

EMAFilter::EMAFilter(float alpha) : alpha(alpha), ema(0.0f), firstRun(true) {}

void EMAFilter::addDataPoint(float dataPoint) {
    if (firstRun) {
        ema = dataPoint;
        firstRun = false;
    } else {
        ema = alpha * dataPoint + (1 - alpha) * ema;
    }
}

float EMAFilter::getFilteredValue() const {
    return ema;
}

```

# Beispiel Datenpunkte
Schrittweise Berechnung

**Datenpunkte**: ${10.0, 11.0, 12.5, 14.0, 13.5, 13.0, 12.0}$
**Glättungsfaktor:** 0.1

**Initialisierung:**
Der erste EMA-Wert wird normalerweise als der erste Datenpunkt gesetzt:
$EMA1=10.0$

- **Zweiter Datenpunkt:** $EMA_2 = 0.1 \cdot 11.0 + (1 - 0.1) \cdot 10.0 = 10.1$
- **Dritter Datenpunkt:** $EMA_3 = 0.1 \cdot 12.5 + (1 - 0.1) \cdot 10.1 = 10.35$
- **Vierter Datenpunkt:** $EMA_4 = 0.1 \cdot 14.0 + (1 - 0.1) \cdot 10.35 = 10.715 $
- **Fünfter Datenpunkt:** $EMA_5 = 0.1 \cdot 13.5 + (1 - 0.1) \cdot 10.715 = 10.9935 $
- **Sechster Datenpunkt:** $EMA_6 = 0.1 \cdot 13.0 + (1 - 0.1) \cdot 10.9935 = 11.19415$
- **Siebter Datenpunkt:** $EMA_7 = 0.1 \cdot 12.0 + (1 - 0.1) \cdot 11.19415 = 11.374735$

Der gefilterte Wert nach dem letzten Datenpunkt ist somit $11.374735$
.