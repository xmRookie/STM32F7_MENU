/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

#include "defs.h"
#include "gpio.h"
#include "Initializations.h"

#define OSC			11059200        // Oscillator frequency of 11.0592 MHz
#define BAUD_RATE	19200

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
void SetUART(void)
{
    // Setup TIMER1 to generate the proper baud rate.
    SM0 = 0;
    SM1 = 1;            // serial port MODE 1
    SM2 = 0;
    REN = 1;            // enable serial receiver

    // Clear transmit interrupt and buffer.
    TI = 0;             // clear transmit interrupt
    RI = 0;

    // Set timer 1 up as a baud rate generator.
    TR1 = 0;            // stop timer 1
    ET1 = 0;            // disable timer 1 interrupt

    PCON |= 0x80;       // 0x80=SMOD: set serial baudrate doubler

    TMOD &= ~0xF0;      // clear timer 1 mode bits
    TMOD |= 0x20;       // put timer 1 into MODE 2

    TH1 = (unsigned char) (256 - (OSC / (16L * 12L * BAUD_RATE)));

    TR1 = 1;            // start timer 1
}

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
void InitMCU(void)
{
    PinRequest = 1;
    PinGrant = 1;

	TXHAL_InitMicroGpios ();

    SetUART();

    TI = 1;  // First symbol
    ES = 1;  // Serial interrupts enable
    IT1 = 1; // External interrupt INT1 caused by transition
    EX1 = 0; // External interrupts INT1 are disable

    IP = 0xA0; // Highest priority goes to T2 (VRefreshRate)
    EA = 1;
}
