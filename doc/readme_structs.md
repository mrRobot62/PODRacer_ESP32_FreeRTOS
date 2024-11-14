# Strukturen

# TDataStatus

```

```

# TDataRC
```
typedef struct __attribute__ ((packed)) {
  uint16_t raw_channels[NUMBER_OF_CHANNELS] = {0};      // raw values from receiver
  uint16_t new_channels[MIN_NUMBER_OF_CHANNELS] = {0};  // adjusted values to FC
  uint16_t gimbal_min = 1000;             // lowest gimbal value (calibrated)
  uint16_t gimbal_max = 2000;             // largest gimbal value (calibrated)
  uint16_t gimbal_mid = 1500;             // center postion
  bool fail_safe = false;
  bool lost_frame = false;
  // bool is_armed = false; 
  // bool prevent_arming = true;             // default true, arming not possible
  uint8_t recvStatus = 0b00000000;       //
} TDataRC;
```
## recvStatus

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
            <td style="border: 1px solid black;">1=armed, 0=disarmed</td>
            <td style="border: 1px solid black;">Gibt den aktuellen Status aus raw_channels[ARMING]</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">1</td>
            <td style="border: 1px solid black;">1=prevent arming, 0=arming possible</td>
            <td style="border: 1px solid black;">errechneter Wert, wenn Arming nicht erlaubt wird</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">2</td>
            <td style="border: 1px solid black;">1=Status changed, 0=keine Änderung</td>
            <td style="border: 1px solid black;">Bezieht sich auf Bit0</td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">3</td>
            <td style="border: 1px solid black;"></td>
            <td style="border: 1px solid black;"></td>
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


<table>
    <caption>Error-Pattern</caption>
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
            <td style="border: 1px solid black;">1= Error in ReceiverTask</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">1</td>
            <td style="border: 1px solid black;">1= Error in MixerTask</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">2</td>
            <td style="border: 1px solid black;">1= Error in HoverTask</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">3</td>
            <td style="border: 1px solid black;">1= Error in SurfaceTask</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">4</td>
            <td style="border: 1px solid black;">free</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">5</td>
            <td style="border: 1px solid black;">free</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">6</td>
            <td style="border: 1px solid black;">free</td>
            <td style="border: 1px solid black;"></td>
        </tr>
        <tr>
            <td style="border: 1px solid black;">7</td>
            <td style="border: 1px solid black;">1=PreventArming</td>
            <td style="border: 1px solid black;"></td>
        </tr>
    </tbody>
</table>

