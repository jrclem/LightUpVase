# LightUpVase
This is an Arduino based project to control light colors and animationd in a 3D printed vase and rose. I originally made it for my girlfriend as an anniversary gift, but I thought people might like to make their own.

The project uses the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) and it has been tested in an Arduino Uno board with an ATmega328P processor. It is compatible with the [Adafruit ProTrinket 5v](https://www.adafruit.com/products/2000).

The STL files for the [vase](../master/rose-vase_FINAL.stl) and [flower](../master/rose-petals_FINAL.stl) models are based on models from www.thingiverse.com. The vase is a remix of ["Wave Vase" by didierkl](http://www.thingiverse.com/thing:983438), and the flower is a remix of ["Make a rose for your girl" by hansliu](http://www.thingiverse.com/thing:663065). For more details about the 3D printing specifications visit my [thingiverse account](http://www.thingiverse.com/jrclem/designs). They will be available in the next few days.

A youtube playlist detailing the development process will be available [here](http://bit.ly/2cJCIvM).

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
3. 1x [Arduino RGB Neopixel](https://www.adafruit.com/products/1260)
4. 1x 5.5mmx2.1mm female barrel jack power adapter
5. 1x slide switch
6. 1x 6mmx6mm push button
7. 1x 1000&mu;F capacitor
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
 * Clockwise starting at the fin of the stem base: 5V, Din, and GND.
3. Glue the petals to the stem using plenty of CA glue.
4. Solder wires to the 5v, Din, and GND leads in the Neopixel Ring.
5. Solder wires to the 3 battery plates.
6. Cut and mold the plates to fit separately in the stem coupler.
7. Load [vase.ino](../master/vase.ino) into the ProTrinket.
8. Solder components to board according to the [schematics](../master/LightUpVase-Schematics.png).
 * Ensure that all components fit in the 3D printed insert while you trim and solder wires in place.
9. Glue the button, switch, power adaptor, and wired battery plates in place using CA glue or resin.
10. Screw the insert into the vase, and lightly glue in place.
11. Plug in and test the assmebly



----------
If you would like to donate to my Grad School Fund, you can do so via [GoFundMe](http://bit.ly/2dqhotO) or [Paypal](http://bit.ly/2d2jKQU).

Thank you very much for your support!
