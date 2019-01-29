//***************************************************************************
//!file     hal_timers_set.c
//!brief    AT80C51ID2 HAL timer TimerSet functions.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#include "defs.h"
#include "stdio.h"
//#include <MCU_Regs.h>

#include <si_apiConfigure.h>
#include "hal_timers.h"

//------------------------------------------------------------------------------
// Function:    HalTimerSet
// Description:
//------------------------------------------------------------------------------

void HalTimerSet (uint8_t index, uint16_t m_sec)
{
    __disable_irq();                            // Disable interrupts while updating

    switch (index)
    {
    	case TIMER_0:
    	case TIMER_POLLING:
    	case TIMER_2:
    	case TIMER_3:
		case TIMER_SERIAL:
#ifdef IS_EVENT
		case TIMER_LINK_CHECK:
		case TIMER_LINK_RESPONSE:
#endif // IS_EVENT
        	g_timerCounters[index] = m_sec;
        	break;

    	case ELAPSED_TIMER:
        	g_timerElapsedGranularity = m_sec;
        	g_timerElapsed = 0;
        	g_elapsedTick = 0;
        	break;

    	case ELAPSED_TIMER1:
        	g_timerElapsedGranularity1 = m_sec;
        	g_timerElapsed1 = 0;
        	g_elapsedTick1 = 0;
        	break;
    }

    __enable_irq();
}
