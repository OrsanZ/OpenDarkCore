# OpenDarkCore

OpenDarkCore is an open source CLI for Corsair's Dark Core and Dark Core SE on **Linux**.

### Dependencies

- libusb-1.0
- make (make)
- gcc (make)

### Building OpenDarkCore

To build the application, simply clone this repo and cd into it. Then use make.

```
git clone https://github.com/OrsanZ/OpenDarkCore.git
cd OpenDarkCore
make
```

## Using OpenDarkCore

OpenDarkCore has 5 different operation modes, 4 of those for lighting and 1 for the mouses DPI.

To use these operation modes, OpenDarkCore needs permission to write to the mouse.

You can either run OpenDarkCore as root (which is far from recommended), or install the udev rule _99-opendarkcore.rules_ into your _/etc/udev/rules.d/_, and then add yourself to the group _plugdev_, using:

```
sudo usermod -a -G plugdev <your user>
```

After doing so, you should be able to use OpenDarkCore as a non-root user. Keep in mind that for now, the CORSAIR Dark Core RGB **Non-SE** is not supported. If you have that mouse, please open an issue, so I can get its ProductID and make it compatible.

A guide to the different operation modes can be displayed using:

```
opendarkcore --help
```

#### Lighting

OpenDarkCore provides the same lighting modes as Corsair's iCUE, those being:

- Static
- Rainbow
- Color Shift
- Color Pulse

Colors a specified in classic hex fassion, like ff00ff for purple. Spare the **#** please.

##### Static

The static mode is dead simple, to make your mouse a static color, use

```
opendarkcore --static --color=RRGGBB
```

for the color white, one would use

```
opendarkcore --static --color=ffffff
```

##### Rainbow

The rainbow mode is even simpler. To make your mouse look like a christmas ornament on acid, use

```
opendarkcore --rainbow
```

Additionally, the speed of the rainbow can be specified using `--speed=SPEED`. The speed value can be an integer from 1 to 3,
3 being the fastest and 1 being the slowest. The `--speed` argument is also valid for the color shift and color pulse mode.

```
opendarkcore --rainbow --speed=3
```

##### Color Shift

The color shift mode works like rainbow, but requires the 2 colors to be shifted between, or random for randomized colors.

```
opendarkcore --shift --random --speed=3 #Shifts between random colors with the speed 3.
opendarkcore --shift --color=ff0000 --color2=0000ff --speed=1 #Shifts between red and blue with the speed 1
```

##### Color Pulse

The color pulse mode works like exactly like shift.

```
opendarkcore --pulse --random --speed=3 #Pulses randomly with the speed 3.
opendarkcore --pulse --color=ff0000 --color2=0000ff --speed=1 #Pulses red and blue with the speed 1
```

#### DPI

To use the DPI mode, a DPI value between 100 and 16000, and optionally, a profile, ranging from 0 to 3.

The DPI profiles are:

- 0. Sniper mode
- 1. Profile 1
- 2. Profile 2
- 3. Profile 3

Example:

```
opendarkcore --dpi=800 #Sets the DPI of the first profile to 800
opendarkcore --dpi=361 --profile=0 #Sets the dpi value for sniper mode to 361
```

#### Angle Snapping

I'm not sure if this works, since I don't notice the difference between angle snapping on and off,
but if you want to enable it, you can use:

```
opendarkcore --anglesnapping=on
```

I'll let you figure out how to disable it.

#### Lift Height

You can change your lift height from low to high, by using:

```
opendarkcore --liftheight=1 #Sets lift height to low
opendarkcore --liftheight=2 #Sets lift height to medium
opendarkcore --liftheight=3 #Sets lift height to high
```

#### Combining options

You can combine options to take care of all your customization needs in one command, like so:

```
opendarkcore --rainbow --dpi=900 --liftheight=2
```
