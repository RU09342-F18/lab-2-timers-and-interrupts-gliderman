/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  Description; Use the button on P1.3 to set the rate of the P1.0 LED blinking
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//   Button-->|P1.3         P1.0|-->LED
//
//  D. Dang
//  Texas Instruments, Inc
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
// Nate Hoffman
// 9/27/2018

#include <msp430.h>

int delayForHertz(int hertz) {
    // Uses a 1.5 kHz clock
    int delay = 750 / hertz;

    return delay;
}

int main(void)
{
    BCSCTL3 = LFXT1S_2; // Enable ACLK (12 kHz signal)
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    // LED
    P1DIR |= BIT0; // Set P1.0 to output direction
    P1SEL &= ~BIT0; // P1.0 as I/O
    P1SEL2 &= ~BIT0; // P1.0 as I/O
    P1OUT &= ~BIT0; // P1.0 turn off LED

    // Button
    P1DIR &= ~BIT3; // Set P1.3 to input
    P1SEL &= ~BIT3; // P1.3 as I/O
    P1SEL2 &= ~BIT3; // P1.3 as I/O
    P1REN |= BIT3; // P1.3 enable resistor
    P1OUT |= BIT3; // P1.3 pullup resistor

    // Configure button interrupts
    P1IES |= BIT3; // P1.3 high-to-low transition
    P1IE |= BIT3; // Enable P1.3 interrupt

    // Setup timer to be 1.5 kHz
    TACTL = TASSEL_1 + ID_3 + MC_1 + TACLR + TAIE; // ACLK, /8, up, clear, enable interrupt

    // Setup CCR0 for LED
    TACCR0 = delayForHertz(10); // 10 Hz

    // Enter low power mode
    _BIS_SR(LPM3_bits + GIE);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IES & BIT3) {
        // Button is pressed
        TACTL = TASSEL_1 + ID_3 + MC_2 + TACLR; // ACLK, /8, continuous, clear
    } else {
        // Button is not pressed
        TACCR0 = TAR >> 1; // Update the timer length
        TACTL = TASSEL_1 + ID_3 + MC_1 + TACLR + TAIE; // ACLK, /8, up, clear, enable interrupt
    }

    P1IES ^= BIT3; // P1.3 toggle transition
    P1IFG &= ~BIT3; // Clear the P1.3 interrupt
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void)
{
    // Switch TAIV, automatically resets the flags
    switch (__even_in_range(TAIV, 10)) {
    case 10:
        // Timer overflow
        P1OUT ^= BIT0;
        break;
    }
}
