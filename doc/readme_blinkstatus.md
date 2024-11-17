# Blink-Status
Verfügbar stehen drei LEDs. 1x blau, 2x rot

## Hardware

| LED        |  PIN  | Farbe | Info                  |
| ---------- | :---: | ----- | --------------------- |
| LED_STATUS |  12   | blau  | Status für ARM/DISARM |
| LED_ERR1   |  14   | rot   | Fehler-Indikator 1    |
| LED_ERR2   |  27   | rot   | Fehler-Indikator 2    |
|            |       |       |                       |

## Blink-Pattern

| FREQUENCY | SPEED  | Info                           |
| :-------: | :----: | ------------------------------ |
|  1000ms   |  slow  | langsames Blinken im 1sek Takt |
|   500ms   | medium | 2x Sekunden                    |
|   200ms   | quick  | 5x Sekunde                     |
|   50ms    |  fast  | 20x Sekunde                    |
|           |        |                                |


| LED_STATUS | LED_ERR1 | LED_ERR2 | Info                                                         |
| :--------: | :------: | :------: | ------------------------------------------------------------ |
|    slow    |   off    |   off    | PODRacer disarmed                                            |
|    slow    |   fast   |   off    | PODRacer kann nicht gearmed werden. Falsche Grundeinstellung |
|    fast    |   off    |   off    | PODRacer gearmed                                             |
|            |          |          |                                                              |

## Software
BitMuster / BitMaskierung, 8Bit pro LED


<table>
    <caption>BlinkMask (8Bit) </caption>
    <thead>
        <tr>
            <th style="border: 1px solid black;">BIT</th>
            <th style="border: 1px solid black;">Value</th>
            <th style="border: 1px solid black;">Info</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td style="border: 1px solid black;">0</td>
            <td style="border: 1px solid black;">1=50ms</td>
            <td style="border: 1px solid black;">Blinkfrequenz fast LED1</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">1</td>
            <td style="border: 1px solid black;">1=200ms</td>
            <td style="border: 1px solid black;">Blinkfrequenz 200ms LED1</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">2</td>
            <td style="border: 1px solid black;">1=500ms</td>
            <td style="border: 1px solid black;">Blinkfrequenz 500ms LED1</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">3</td>
            <td style="border: 1px solid black;">1=1000ms</td>
            <td style="border: 1px solid black;">Blinkfrequenz 1000ms LED1</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">4</td>
            <td style="border: 1px solid black;">1=on, 0=off</td>
            <td style="border: 1px solid black;">LED_STATUS</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">5</td>
            <td style="border: 1px solid black;">1=on, 0=off</td>
            <td style="border: 1px solid black;">LED_ERR1</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">6</td>
            <td style="border: 1px solid black;">1=on, 0=off</td>
            <td style="border: 1px solid black;">LED_ERR2</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">7</td>
            <td style="border: 1px solid black;">1=on, 0=off</td>
            <td style="border: 1px solid black;">alle LEDs</td>
        </tr>
    </tbody>
</table>

