//***************************************************************************
//!file     hal_timers_expired.c
//!brief    AT80C51ID2 HAL timer TimerExpired function.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/
//#include <MCU_Regs.h>

#include <si_apiConfigure.h>
#include "hal_timers.h"

//------------------------------------------------------------------------------
// Function:    HalTimerExpired
// Description: Returns > 0 if specified timer has expired.
//------------------------------------------------------------------------------

uint8_t HalTimerExpired (uint8_t timer)
{
    if (timer < TIMER_COUNT)
    {
        return(g_timerCounters[timer] == 0);
    }

    return(0);
}

