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
//  Description; Toggle the LED states on P1.0 and P1.6 based on a timer
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//            |             P1.6|-->LED
//
//  D. Dang
//  Texas Instruments, Inc
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
// Nate Hoffman
// 9/26/2018

#include <msp430.h>

int delayForHertz(int hertz) {
    // Uses a 12 kHz clock
    int delay = 6000 / hertz;

    return delay;
}

int main(void)
{
    BCSCTL3 = LFXT1S_2; // Enable ACLK (12 kHz signal)
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    // LED1
    P1DIR |= BIT0; // Set P1.0 to output direction
    P1SEL &= ~BIT0; // P1.0 as I/O
    P1SEL2 &= ~BIT0; // P1.0 as I/O
    P1OUT &= ~BIT0; // P1.0 turn off LED

    // LED2
    P1DIR |= BIT6; // Set P1.6 to output direction
    P1SEL &= ~BIT6; // P1.6 as I/O
    P1SEL2 &= ~BIT6; // P1.6 as I/O
    P1OUT &= ~BIT6; // P1.6 turn off LED

    // Setup timer to be 12 kHz
    TACTL = TASSEL_1 + MC_2 + TACLR; // ACLK, continuous, clear

    // Setup CCR1 for LED1
    TACCR1 = delayForHertz(1); // 1 Hz
    TACCTL1 = CCIE; // Enable interrupt

    // Setup CCR2 for LED2
    TACCR2 = delayForHertz(5); // 5 Hz
    TACCTL2 = CCIE; // Enable interrupt

    // Enter low power mode
    _BIS_SR(LPM3_bits + GIE);
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1(void)
{
    // Switch TAIV, automatically resets the flags
    switch (__even_in_range(TAIV, 10)) {
    case 2:
        // CCR1
        TACCR1 += delayForHertz(1);
        P1OUT ^= BIT0;
        break;
    case 4:
        // CCR2
        TACCR2 += delayForHertz(5);
        P1OUT ^= BIT6;
    case 10:
        break;
    }
}
