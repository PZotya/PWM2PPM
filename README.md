# PWM2PPM
How to turn old PWM receiver without failsafe to a PPM receiver with failsafe

The source is for an arduino pro mini

The PWM input pins are 4,5,6,7,8,9 ( for my HK TR6A V2 ).
If You want to use an 8 channel receiver You can modify the following things:
- change PPMMAXCHANNELNUM value to 10
- uncomment the commented lines in the ISR(PCINT0_vect)
- uncomment the commented lines in the setup(), and delete the original RSSI and Failsafe lines
- change the PCMSK0 setting with the commented line
- change the ISR(PCINT0_vect) currPins line with the commented line
-  change the setup() lastPinStateB line with the commented line
-  change the loop () lines with the commented lines

The PPM output pin is 2.
The RSSI signal is the A0 input, if You have an analog signal
