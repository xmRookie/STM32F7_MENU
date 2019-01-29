//***************************************************************************
//!file     hal_timers_elapsed.c
//!brief    AT80C51ID2 HAL timer elapsed time function.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

//#include <MCU_Regs.h>
#include <si_apiConfigure.h>
//#include "hal_local.h"
#include "hal_timers.h"

//------------------------------------------------------------------------------
// Function:    HalTimerElapsed
// Description: Returns current timer tick.  Rollover depends on the
//              granularity specified in the SetTimer() call.
//------------------------------------------------------------------------------

uint16_t HalTimerElapsed ( uint8_t index )
{
    uint16_t elapsedTime;
    
    __disable_irq();                              // Disable interrupts while updating
    if ( index == ELAPSED_TIMER )
        elapsedTime = g_timerElapsed;
    else
        elapsedTime = g_timerElapsed1;
    __enable_irq();
    
    return( elapsedTime );
}
