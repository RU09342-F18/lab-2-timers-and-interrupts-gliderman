# Nate Hoffman's Button Interrupt

Toggle the state of the LED each time that a button is pressed.

## Operation

Each time that a button is pressed, an interrupt is triggered in the processor to handle the event. This event toggles the LED state and then the processor goes back to sleep.

## Devices

Code was written that runs on an MSP430G2553 and MSP430F5529. The G2553 board uses the LED connected to P1.0 and the button connected to P1.3. The F5529 board uses the LED connected to P1.0 and the button connected to P2.1.
