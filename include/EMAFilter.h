#ifndef EMAFILTER_H
#define EMAFILTER_H

class EMAFilter {
private:
    float alpha;           // Glättungsfaktor
    float filteredValue;    // Gefilterter Wert

public:
    // Konstruktor mit Angabe des Glättungsfaktors (0 < alpha <= 1)
    EMAFilter(float smoothingFactor);

    // Methode zum Initialisieren des gefilterten Werts (optional)
    void init(float initialValue);

    // Methode zum Berechnen des neuen gefilterten Werts
    float update(float newValue);

    // Methode zum Abrufen des aktuellen gefilterten Werts
    float getFilteredValue() const;
};

#endif // EMAFILTER_H
