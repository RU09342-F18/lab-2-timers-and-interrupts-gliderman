# Nate Hoffman's Button Based Delay

Use a button to set the period of a blinking LED.

## Operation

The code listens for two different interrupts, a button and a timer. The timer interrupt toggles the state of LED each time the timer reaches a certain value, which gives the blinking function. When the button is pressed, the timer resets. When the button is released the current count of the timer is stored as the value to reset at with the LED toggling.

## Devices

Code was written for the MSP430G2553 and MSP430F5529. The G2553 board uses the LED connected to P1.0 and the button connected to P1.3. The F5529 board uses the LED connected to P1.0 and the button connected to P2.1.
