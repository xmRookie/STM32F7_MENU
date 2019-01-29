//***************************************************************************
//!file     hal_timers.c
//!brief    AT80C51ID2 HAL timer support functions.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#include <stdio.h>
//#include <MCU_Regs.h>
//#include "hal_local.h"
#include "hal_timers.h"

//------------------------------------------------------------------------------
// Array of timer values
//------------------------------------------------------------------------------

uint16_t g_timerCounters[ TIMER_COUNT ];

uint16_t g_timerElapsed;
uint16_t g_elapsedTick;
uint16_t g_timerElapsedGranularity;

uint16_t g_timerElapsed1;
uint16_t g_elapsedTick1;
uint16_t g_timerElapsedGranularity1;

TIM_HandleTypeDef TIM3_Handler;	
//------------------------------------------------------------------------------
// Function: TimerTickHandler
// Description:
//------------------------------------------------------------------------------

static void TimerTickHandler ( TIM_HandleTypeDef *htim ) /*interrupt 1*/
{
    uint8_t i;
	/*
    //restart timer immediately
    TR0 = 0;        // stop timer tick
    TL0 = 0x66;     // reloader timer value  64614
    TH0 = 0xFC;

    TR0 = 1;        // start timer tick
	*/
    //decrement all active timers in array
	if(htim==(&TIM3_Handler))
	{	
    for ( i = 0; i < TIMER_COUNT; i++ )
    {
        if ( g_timerCounters[ i ] > 0 )
        {
            g_timerCounters[ i ]--;
        }
    }
    g_elapsedTick++;
    if ( g_elapsedTick == g_timerElapsedGranularity )
    {
        g_timerElapsed++;
        g_elapsedTick = 0;
    }
    g_elapsedTick1++;
    if ( g_elapsedTick1 == g_timerElapsedGranularity1 )
    {
        g_timerElapsed1++;
        g_elapsedTick1 = 0;
    }
	}
}

//------------------------------------------------------------------------------
// Function: HalTimerInit
// Description:
//------------------------------------------------------------------------------

void HalTimerInit ( void )
{
    uint8_t i;

    //initializer timer counters in array
    for ( i = 0; i < TIMER_COUNT; i++ )
    {
        g_timerCounters[ i ] = 0;
    }
    g_timerElapsed  = 0;
    g_timerElapsed1 = 0;
    g_elapsedTick   = 0;
    g_elapsedTick1  = 0;
    g_timerElapsedGranularity   = 0;
    g_timerElapsedGranularity1  = 0;

    // Set up Timer 0 for timer tick

    /*TMOD |= 0x01;   // put timer 0 in Mode 1 (16-bit timer)

    TL0 = 0x66;     // set timer count for interruput every 1ms (based on 11Mhz crystal)
    TH0 = 0xFC;     // each count = internal clock/12

    TR0 = 1;        // start the timer
    ET0 = 1;        // timer interrupts enable
	*/
	
		TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=40-1;                     //分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=10800-1;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
	
}




