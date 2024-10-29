# MOCK-Klassen
MOCK-Klassen ersetzen während der Entwicklung die phyische Sensorklasse und bieten das gleiche Interface als Sensorklassen.
Sowohl MOCK als auch Sensor implementieren das ISensor-Interface und werden entweder im Task oder in der Controllerklasse
deklariert und instanziiert.

Um unterschiedliche Verhaltensweisen während der Entwicklung zu simulieren, kann einer Mock-Klasse ein Bit-Array übergeben werden. Über dieses Bit-Muster muss in der MOCK-Klasse ein entsprechendes "Verhalten/Ausgabe" implementiert werden.

Da Mock-Klassen für alle Tasks / Sensoren implementiert werden können, kann somit ein komplexes Muster getestet werden.

## Bit-Array für Tests
Jeder Mock-Klasse kann bei der Instanziierung im Konstruktor diese Bitmaske übergeben werden. Je nach Bit werden unterschiedliche
Daten generiert

**Beispiel aus ReceiverTask.cpp**
```
  #ifdef USE_MOCK_SBUS
    receiver = new MockReceiverSBUS(MOCK_DATA_MASK_SBUS); // in main.cpp kann nach bedarf MOCK_DATA_MASK_SBUS angepasst werden
  #else
    receiver = new ReceiverSBUS(&hsBus2, SBUS_RX, SBUS_TX );
  #endif
```

### Sensoren
Bit 0 - ist in der Regel immer eine Default Einstellung. <b>Fett dargestellte Werte</b> sind Änderungen zum Default
<table>
    <caption>Controller-Klassen / Tasks</caption>
    <thead>
        <tr>
            <th style="border: 1px solid black;">BIT</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_PMW3901</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_TFMINI</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_VL53X1</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td style="border: 1px solid black;">0</td>
            <td style="border: 1px solid black;">X=0, Y=0, preventArming=false</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">1</td>
            <td style="border: 1px solid black;"><b>X <> 0</b>, Y = 0, <b>preventArming=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">2</td>
            <td style="border: 1px solid black;">X = 0, <b>Y <> 0</b>, <b>preventArming=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">3</td>
            <td style="border: 1px solid black;"><b>X <> 0, Y > 0</b>, <b>preventArming=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">4</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">5</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">6</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">7</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
    </tbody>
</table>

### Controller-Klassen
Bit 0 - ist in der Regel immer eine Default Einstellung. <b>Fett dargestellte Werte</b> sind Änderungen zum Default

<table>
    <caption>Controller-Klassen / Tasks</caption>
    <thead>
        <tr>
            <th style="border: 1px solid black;">BIT</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_SBUS</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_HOVER</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_SURFACE</th>
            <th style="border: 1px solid black;">MOCK_DATA_MASK_MIXER</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td style="border: 1px solid black;">0</td>
            <td style="border: 1px solid black;">ROLL,PITCH,YAW=mid, <br>Throttle,Thrust=min, ARMING=false</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">1</td>
            <td style="border: 1px solid black;">ROLL,PITCH,YAW=mid, <br>Throttle,Thrust=min, <b>ARMING=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">2</td>
            <td style="border: 1px solid black;"><b>ROLL=min</b>,PITCH,YAW=mid, <br>Throttle,Thrust=min, <b>ARMING=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">3</td>
            <td style="border: 1px solid black;">ROLL,PITCH,YAW=mid, <br><b>Throttle,Thrust=mid</b>, <b>ARMING=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">4</td>
            <td style="border: 1px solid black;"><b>Sinus ROLL</b>, PITCH/YAW=mid, <br>Throttle=min, <b>Thrust=mid</b>, <b>ARMING=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">5</td>
            <td style="border: 1px solid black;"><b>Sinus ROLL</b>, PITCH/YAS=mid, <br>Throttle=min, Thrust=min, <b>ARMING=true</b></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">6</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">7</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
    </tbody>
</table>
