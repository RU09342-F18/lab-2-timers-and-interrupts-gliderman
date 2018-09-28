# Nate Hoffman's TIMER A Blink

A timer is used to blink two LEDs at separate rates.

## Operation

A single timer in continuous mode (looping from 0x0000 to 0xFFFF continuously) was used. Two capture compare registers were used to trigger the two different LED toggles at different rates. Each time they triggered, the next trigger time was moved according to the cooresponding frequency the LED should operate at. A function was created that calculates the delay needed to acheive a specific frequency.

## Devices

Code was written that runs on an MSP430G2553 and MSP430F5529. The G2553 board uses LEDs connected to pins P1.0 and P1.6. The F5529 board uses LEDs connected to pins P1.0 and P4.7.
