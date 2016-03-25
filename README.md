# LightUpVase
This is an Arduino based project to control light colors and animationd in a 3D printed vase and rose.

The project uses the Adafruit NeoPixel library and it has been tested in an Arduino Uno board with an ATmega328P processor. It is compatible with the Adafruit ProTrinket 5v.

The STL files for the vase and flower models will be available in thingiverse.com in the next few weeks. A youtube playlist detailing the development process will be available shortly as well.

Operation:
1-Connect the power supply to the vase. (In this case using 5V, 2A)
2-Flip the slide switch to the "on" position. This will power the board and the NeoPixel Ring.
3-Click the pushbutton for less than 0.5s to flip through the vase's light animations.
3-Click the pushbutton for  more than 0.5s to flip through the flower's light animations.

Note:
Do not leave lights turned off whilr the slide switch in the "on" position for an extended period of time. The board will still be powered during this time.
