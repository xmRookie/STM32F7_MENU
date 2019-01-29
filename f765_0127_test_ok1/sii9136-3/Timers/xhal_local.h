//***************************************************************************
//!file     hal_local.h
//!brief    Silicon Image AT80C51ID2 HAL local file header (non-public).
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2009, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __HAL_LOCAL_H__
#define __HAL_LOCAL_H__

#include "at89c51xd2.h"
#include "si_datatypes.h"
#include "si_hal.h"

#define HAL_VERSION_MAJOR               1
#define HAL_VERSION_MINOR               6

//------------------------------------------------------------------------------
//  Compile-time build options
//------------------------------------------------------------------------------

#define FPGA_BUILD          0           // 1 == FPGA, 0 == Silicon

#define HAL_STANDALONE      DISABLE     // Set to ENABLE only for test mode

//------------------------------------------------------------------------------
//  HAL error codes     (8-bit error codes)
//------------------------------------------------------------------------------

#define HALERR_I2C_SW_TIMEOUT           100
#define HALERR_I2C_NO_ACK               101

//------------------------------------------------------------------------------
//  Processor
//------------------------------------------------------------------------------

#define MCU_CRYSTAL_20M                 20
#define MCU_CRYSTAL_11M                 11
#define CONF__CRYSTAL                   MCU_CRYSTAL_11M

//------------------------------------------------------------------------------
//  UART
//------------------------------------------------------------------------------

    // UART Timer 2 divider values for baud rate generator
    //   65518 = 19,200 baud with 11.0592 MHz crystal
    //   65503 = 19,200 baud with 20 MHz crystal

#define HAL_UART_BAUD_DIVIDER           (65518)

#define HAL_UART_DEBUG_PRINT            (ENABLE)    //print debug messages
#define HAL_UART_DEBUG_CMD_HANDLER      (ENABLE)    //run debug command handler

//-------------------------------------------------------------------------------
//  REMOTE
//-------------------------------------------------------------------------------

#if (FPGA_BUILD == 1)
	#define BUS_REQUEST_PIN                 P0^3       //
	#define BUS_GRANT_PIN                   P2^0       //
	#define bool_t 							bool_t
#else
	#define BUS_REQUEST_PIN                 P0^0        //
	#define BUS_GRANT_PIN                   P2^3        // rev x02 only
#endif

//------------------------------------------------------------------------------
//  Timer
//------------------------------------------------------------------------------

#define TIMER_COUNT     4

//------------------------------------------------------------------------------
//  I2C - Legacy Software functions
//------------------------------------------------------------------------------

#if (FPGA_BUILD == 0)
    #define GPIO_PIN_I2C_SCL            P1^6
    #define GPIO_PIN_I2C_SDA            P1^7
#else
    #define GPIO_PIN_I2C_SCL            PI2^0
    #define GPIO_PIN_I2C_SDA            PI2^1
#endif
#define I2C_SW_SCL_TIMEOUT              32768       // 65535 is infinite

//------------------------------------------------------------------------------
//  I2C Bus oriented functions
//------------------------------------------------------------------------------

#if (FPGA_BUILD == 0)
    #define P0_SCL                          P1^6        // Local I2C Bus
    #define P0_SDA                          P1^7        // Local I2C Bus

    #define P1_SCL                          P2^4        // HDMI DDC - Rev x01
    #define P1_SDA                          P2^3

    #define P2_SCL                          P2^0        // VGA DDC
    #define P2_SDA                          P2^1

    #define P3_SCL                          P2^5        // HDMI DDC - Rev x02
    #define P3_SDA                          P2^4
#else
    #define P0_SCL                          PI2^0       // FPGA Local I2C Bus SCL
    #define P0_SDA                          PI2^1       // FPGA Local I2C Bus SDA

    #define P1_SCL                          PI2^0       // Same as P0
    #define P1_SDA                          PI2^1

    #define P2_SCL                          PI2^0       // Same as P0
    #define P2_SDA                          PI2^1

    #define P3_SCL                          PI2^0       // Same as P0
    #define P3_SDA                          PI2^1
#endif

#define I2C_BUS_SCL_TIMEOUT             32768       // 65535 is infinite

//------------------------------------------------------------------------------
//  External EEPROM
//------------------------------------------------------------------------------

#define EEPROM_I2C_ADDRESS              0xA8

//------------------------------------------------------------------------------
//  Internal EEPROM
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  GPIO
//------------------------------------------------------------------------------

    /* Define the Rotary switch GPIO pins for this board.   */

#if (FPGA_BUILD == 0)
    #define GPIO_ROTARY_SWITCH_PORT         P0
    #define GPIO_ROTARY_SWITCH_MASK         0x1C
    #define GPIO_ROTARY_SWITCH_XOR          0x1C    // All pins need invert for SK928x
    #define GPIO_ROTARY_SWITCH_SHIFT        2
#else
    #define GPIO_ROTARY_SWITCH_PORT         P3
    #define GPIO_ROTARY_SWITCH_MASK         0xF0
    #define GPIO_ROTARY_SWITCH_XOR          0x00    // No invert
    #define GPIO_ROTARY_SWITCH_SHIFT        4
#endif

    /* Define the settling time, in 100ms intervals, */
    /* of each position on the rotary switch.       */
    /* NOTE: These are SIGNED counts, and should    */
    /*       NOT be set to greater than 127!        */

#define DEM_POLLING_DELAY               100

#define DEBOUNCE_0                      (200 / DEM_POLLING_DELAY)
#define DEBOUNCE_1                      (200 / DEM_POLLING_DELAY)
#define DEBOUNCE_2                      (200 / DEM_POLLING_DELAY)
#define DEBOUNCE_3                      (200 / DEM_POLLING_DELAY)
#define DEBOUNCE_4                      (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_5                      (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_6                      (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_7                      (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_8                      (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_9                      (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_10                     (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_11                     (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_12                     (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_13                     (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_14                     (500 / DEM_POLLING_DELAY)
#define DEBOUNCE_15                     (500 / DEM_POLLING_DELAY)

//------------------------------------------------------------------------------
//  IR Interface
//------------------------------------------------------------------------------


#define IR_INT_PIN_AND_TIMER    1   // Uses interrupt and timer method.
#define IR_CAPTURE_MODULE       2   // Uses PCA module method

#define IR_IMPLEMENTATION       IR_CAPTURE_MODULE

#define TIMER_IR_UI             TIMER_2         // Use HAL Timer 2

//------------------------------------------------------------------------------
#endif  // __HAL_LOCAL_H__
