# OpenDarkCore

OpenDarkCore is an open source CLI for Corsair's Dark Core and Dark Core SE.

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

To use these operation modes, OpenDarkCore needs **root privileges**.

A guide to the different operation modes can be displayed using:
```
opendarkcore --help
```

#### Experimental features
- The `--zones` argument does not work consistently.

#### To be implemented
- The option to disable dpi profiles.

#### Lighting
OpenDarkCore provides the same lighting modes as Corsair's iCUE, those being:
- Static
- Rainbow
- Color Shift
- Color Pulse

##### Static
The static mode is dead simple, to make your mouse a static color, use
```
sudo opendarkcore --static <R> <G> <B>
```
for the color white, one would use
```
sudo opendarkcore --static 255 255 255
```

##### Rainbow
The rainbow mode is even simpler. To make your mouse look like a christmas ornament on acid, use
```
sudo opendarkcore --rainbow
```
Additionally, the speed of the rainbow can be specified using `--speed <s>`. The speed value can be an integer from 1 to 3,
3 being the fastest and 1 being the slowest. The `--speed` argument is also valid for the color shift and color pulse mode.

```
sudo opendarkcore --rainbow --speed 3
```

##### Color Shift
The color shift mode works like rainbow, but requires the 2 colors to be shifted between, or random for randomized colors.

```
sudo opendarkcore --shift random --speed 3 #Shifts between random colors with the speed 3.
sudo opendarkcore --shift 255 0 0 0 0 255 --speed 1 #Shifts between red and blue with the speed 1
```

##### Color Pulse
The color pulse mode works like exactly like shift.

```
sudo opendarkcore --pulse random --speed 3 #Pulses randomly with the speed 3.
sudo opendarkcore --pulse 255 0 0 0 0 255 --speed 1 #Pulses red and blue with the speed 1
```

#### DPI

To use the DPI mode, specify a profile, ranging from 0 to 3 and a DPI value between 100 and 16000.

The DPI profiles are:
0. Sniper mode
1. Profile 1
2. Profile 2
3. Profile 3

Example:
```
sudo opendarkcore --dpi 1 800 #Sets the DPI of the first profile to 800
```
