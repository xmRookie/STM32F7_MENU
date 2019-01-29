//***************************************************************************
//!file     hal_timers_wait.c
//!brief    AT80C51ID2 HAL timer TimerWait function.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#include "hal_timers.h"

//------------------------------------------------------------------------------
// Function:    HalTimerWait
// Description: Waits for the specified number of milliseconds, using timer 0.
//------------------------------------------------------------------------------

void HalTimerWait ( uint16_t m_sec )
{
    HalTimerSet( 0, m_sec );

    while ( !HalTimerExpired( 0 ) )
    {
        ;  //empty wait loop
    }
}
