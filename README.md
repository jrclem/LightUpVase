# LightUpVase
This is an Arduino based project to control light colors and animationd in a 3D printed vase and rose.

The project uses the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) and it has been tested in an Arduino Uno board with an ATmega328P processor. It is compatible with the [Adafruit ProTrinket 5v](https://www.adafruit.com/products/2000).

The STL files for the vase and flower models will be available in www.thingiverse.com in the next few weeks. A youtube playlist detailing the development process will be available shortly as well.

###Operation:

1. Connect the power supply to the vase. (In this case using 5V, 2A)
2. Flip the slide switch to the "on" position. This will power the board and the NeoPixel Ring.
3. Click the pushbutton for less than 0.5s to flip through the vase's light animations.
4. Click the pushbutton for  more than 0.5s to flip through the flower's light animations.

*Note:*
Do not leave lights turned off while the slide switch in the "on" position for an extended period of time. The board will still be powered during this time.

###Supplies

1. 1x [Arduino ProTrinket 5v](https://www.adafruit.com/products/2000)
2. 1x [Arduino RGB Neopixel Ring 16x](https://www.adafruit.com/products/1463)
3. 1x Arduino RGB Neopixel (single)
4. 1x 5.5mmx2.1mm female barrel jack power adapter
5. 1x slide switch
6. 1x 6mmx6mm push button
7. 4. 1x 1000&mu;F capacitor
8. 2x 510&ohm; resistor
9. 3x battery contact plates (or a similar thin conductive metal)
10. 1x 5v, 2A power supply with 5.5mmx2.1mm plug
11. \~250g translucent PLA
12. 22ga wire (green for the stem, the rest is up to you)
13. Cyanoacrylate glue and/or 5second Fix resin

###Assembly

1. Solder 3 green wires to the 5v, Din, and GND leads in the single Neopixel.
 * Make sure the wires are at least 180cm long to account for the twisting.
2. Insert Neopixel and cable through stem. Strip wires and glue exposed wires to the groves in the stem.
 * Clockwise starting at the fin of the stem place: 5V, Din, and GND.
3. Glue the petals to the stem using plenty of CA glue.
4. Solder wires to the 5v, Din, and GND leads in the Neopixel Ring.
5. Load [vase.ino](../master/vase.ino) into the ProTrinket
6. Solder components to board according to the schema while ensuring that they fit in the 3D printed insert.
7. Screw the insert into the vase, and lightly glue in place.
8. Plug in and test the assmebly
