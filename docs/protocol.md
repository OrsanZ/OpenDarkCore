# Corsair DARK CORE and DARK CORE SE Protocol

This protocol was tested on a DARK CORE SE on firmware version 3.22.

## Init

The very first packet that gets sent is a 64 bytes long package with just zeroes
Probably make that the first packet you send aswell.

## Reads

### Known Reads

#### 0e 01

Device Information, pattern of the response can be found [here](https://github.com/ckb-next/ckb-next/wiki/Corsair-Protocol).

#### 0e a6

**Response: 0e a6 ON 00 00 03 TI**
<br>
Reads the time it takes for the mouse to go to sleep.
<br>

| Abbreviation | Values                                      |
| ------------ | ------------------------------------------- |
| ON           | 00 Awake, 01 Sleeping                       |
| TI           | Time until sleep (0 for power savings mode) |

#### 0e 50

**Response: 0e 50 00 00 BA 01**
<br>
Reads the battery status.
<br>

| Abbreviation | Values                                                    |
| ------------ | --------------------------------------------------------- |
| BA           | 00 Not connected, 01 Critical, 02 Low, 03 Medium, 04 High |

### Speculation

#### 0e ae

_Response: **0e ae 00 00 1c 1b 51 1b 22 03 20 52 43 01 01**_
<br>
Probably some specific information about the status of the DARK CORE, once the 0e 01 request is done. Needs more research.

#### 0e 17 ...

<br>
These are packages related to hardware DPI profiles.

#### 0e 51

_Response: **0e 51 00 00 ST**_
<br>
Might read whether the mouse is connected to the dongle, as it is not sent when directly connected via cable.

| Abbreviation | Values                         |
| ------------ | ------------------------------ |
| ST           | 01 Not connected, 02 Connected |

### Unknown Reads

- 0e 4a
- 0e 17 01 (Only in wired)

One of these has to be battery status, the others may be DPI info or brightness, or polling rate (which is currently broken in iCUE for me).

## Writes

### Known writes

#### 07 AA 00 00 ZO EF SP RA 64 RR GG BB RR GG BB 05

Lighting effects
<br>

| Abbreviation | Values                                                                                           |
| ------------ | ------------------------------------------------------------------------------------------------ |
| ZO           | Lighting zones to be affected. <br> scroll zone: +4, thumb zone: +2, logo zone: +1               |
| EF           | The desired effect.<br>0 for shift, 1 for pulse, 3 for rainbow and 7 for static                  |
| SP           | Speed of the effect. **NEEDS DEFAULT VALUE FOR STATIC**<br>01 Slow, 02 Medium, 03 Fast           |
| RA           | Byte to randomize colors with pulse and shift. 00: unrelated mode, 01: random, 03: alternating   |
| RR           | The first and second shade of red for the effect. If no color is needed, 255 should be supplied. |
| GG           | The first and second shade of green for the effect. If no color is needed, 0 should be supplied. |
| BB           | The first and second shade of blue for the effect. If no color is needed, 0 should be supplied.  |

This seems to be slighting incorrect, as this breaks when trying to layer effects for different zones. All unaffected zones return to the rainbow effect.

#### 07 AD 00 00 BR

Controls brightness
<br>

| Abbreviation | Values                                                         |
| ------------ | -------------------------------------------------------------- |
| BR           | Brightness value. Either 0 for off or 64 for on. No inbetween. |

#### 07 13 DNUM 00 00 DPIX1 DPIX2 DPIY1 DPIY2 RR GG BB

Changes DPI
<br>

| Abbreviation | Value                                                  |
| ------------ | ------------------------------------------------------ |
| DNUM         | D (decimal 13) + the profile value, e.g. D0 for sniper |
| DPIX1        | Low Byte of DPI value on X-Axis                        |
| DPIX2        | High Byte of DPI value on Y-Axis                       |
| DPIY1        | Low Byte of DPI value on X-Axis                        |
| DPIY2        | High Byte of DPI value on Y-Axis                       |

#### 07 13 05 00 ST

Enables/Disables DPI Profiles.
<br>

| Abbreviation | Values                                          |
| ------------ | ----------------------------------------------- |
| ST           | 3 for DPI1 and sniper, +4 for DPI2, +8 for DPI3 |

#### 07 a6 ON 00 00 03 TI

Controls the time to sleep and power savings mode
<br>

| Abbreviation | Values                   |
| ------------ | ------------------------ |
| ON           | 0 Awake, 1 Sleeping      |
| TI           | Time to sleep in minutes |

#### 07 0A 00 00 DL

Dongle Polling Rate - Not showing up in WireShark or iCUE anymore
<br>

| Abbreviation | Values      |
| ------------ | ----------- |
| DL           | Delay in ms |

#### 07 13 04 00 ON 05

Controls the angle snapping. Just like in the normal protocol.
<br>

| Abbreviation | Values        |
| ------------ | ------------- |
| ON           | 0: Off, 1: On |

#### 07 13 03 00 HE

Controls the lift height. Also just like in the normal protocol.
<br>

| Abbreviation | Values                     |
| ------------ | -------------------------- |
| HE           | 3: Low, 4: Normal, 5: High |

#### 07 04 CT

Controls whether special functions are handled in software or hardware
<br>

| Abbreviation | Values                   |
| ------------ | ------------------------ |
| CT           | 1: Hardware, 2: Software |

### Speculation

#### 07 17 ...

These are packages related to hardware DPI profiles.

### Unknown writes

- 07 04 02
- 07 05 04 00 03
- 07 13 02 00 01
- 07 40 09 00 01 c0 02 c0 03 c0 04 40 05 40 06 40 07 40 08 40 19 40
- 07 aa 00 00 07 07 00 00 64 ff ff ff ff ff ff 05

## Weird Things

### Transfers and interfaces

OUT transfers to the mouse seem to only work via interface 1, which seems to be the
entire purpose of that interface, yet IN transfers only work on interface 0, which
is the same interface that is used for regular mouse movement, meaning that to
recieve data from the mouse, I have to detach the kernel drive and also separate
mouse movements from RGB/Control related packages.

Maybe I'm just missing something but I haven't figured out how to deal with this.
Wireshark also seems to confirm that IN transfers only occur on Interface 0.

### Keepalive packages or something

Also, iCUE just sends a bunch of empty packages to the mouse, sometimes the first
bit is 1, but the rest is always zeros. I can't imagine why you would do this other
than to ensure that the mouse didn't turn off.
