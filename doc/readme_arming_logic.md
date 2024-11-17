# Arming Logik / Prevent-Arming

Bit-Pattern, wird benötigt um einen Statuswechsel von ARMED/DISARMED zu bewerten um ggf. ein
Prevent-Arming zu definieren. ARMING darf nur dann möglich sein, wenn es gefahrlos ist.
Faktoren für ein gefahrloses ARMEN sind (Value: 1500)
- Roll/Pitch/Yaw stehen auf MID-Position (Value: 1500)
- Throttle(Gimbal) / Thrust (poti) stehen auf MIN-Position (Value: 1000)


<table>
    <caption>Status Maskierung</caption>
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
            <td style="border: 1px solid black;">0=starting mode, 1=ready</td>
            <td style="border: 1px solid black;">Für LED1-Signale</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">1</td>
            <td style="border: 1px solid black;">1=armed, 0=disarmed</td>
            <td style="border: 1px solid black;">Gibt den aktuellen Status aus raw_channels[ARMING]</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">2</td>
            <td style="border: 1px solid black;">1=prevent arming, 0=arming possible</td>
            <td style="border: 1px solid black;">errechneter Wert, wenn Arming nicht erlaubt wird</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">3</td>
            <td style="border: 1px solid black;">1=Status changed, 0=keine Änderung</td>
            <td style="border: 1px solid black;">Bezieht sich auf Bit0</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">4</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">5</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">6</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">7</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
        </tr>
    </tbody>
</table>