//***************************************************************************
//!file     hal_timers.h
//!brief    AT80C51ID2 HAL timer header.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include <defs.h>
#include <si_datatypes.h>


//------------------------------------------------------------------------------
// Array of timer values
//------------------------------------------------------------------------------
extern TIM_HandleTypeDef TIM3_Handler;      //¶¨Ê±Æ÷3PWM¾ä±ú 
extern uint16_t g_timerCounters[TIMER_COUNT];

extern uint16_t g_timerElapsed;
extern uint16_t g_elapsedTick;
extern uint16_t g_timerElapsedGranularity;

extern uint16_t g_timerElapsed1;
extern uint16_t g_elapsedTick1;
extern uint16_t g_timerElapsedGranularity1;

void HalTimerInit ( void );
void HalTimerSet ( uint8_t index, uint16_t m_sec );
uint8_t HalTimerExpired ( uint8_t index );
void HalTimerWait ( uint16_t m_sec );
uint16_t HalTimerElapsed( uint8_t index );

#endif  // __HAL_TIMER_H__
