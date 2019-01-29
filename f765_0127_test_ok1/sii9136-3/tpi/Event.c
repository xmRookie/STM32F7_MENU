//***************************************************************************
//!file     Event.c
//!brief    Functions to support Events capabilities.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2010, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

/***********************************************************************************/
/*  Copyright (c) 2002-2010, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/


#include "defs.h"
#include "Constants.h"
#include "TypeDefs.h"
#include "MCU_Regs.h"
#include "Externals.h"
#include "AV_Config.h"
#include "i2c_master_sw.h"
#include "TPI_Access.h"
#include "TPI_Regs.h"
#include "stdio.h"
#include "tpi.h"
#include "si_datatypes.h"
#include "si_apiConfigure.h"
#include "hal_timers.h"
#include "Packet.h"
#include "Event.h"
#include "SimConfig.h"

#ifdef IS_EVENT

#ifdef F_9136 
#include "ehdmi.h"
#endif

#define LINK_CHECK_TIME		5000	//5 sec
#define EVENT_RESPONSE_TIME	500		//200 mSecs

/*extern byte putbyte(byte);*/
extern void VideoModeRespHandler(uint8_t vMode);

//
// Globals
//
eventState_t		eventState;
simonLinkStatus_t	linkStatus;


//------------------------------------------------------------------------------
// Function:    eventSendPing
// Description: Send an PING message to Simon.  Does not wait for a reply.
//------------------------------------------------------------------------------

void eventSendPing()
{
	/*putbyte(START_OF_FW_PACKET);
	putbyte(FW_EVENT_PING);
	putbyte(0);*/		// No data bytes
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   EventInit ()
//
// PURPOSE          :   
//
// CALLED BY        :   
//
// INPUT PARAMS     :  
//
// OUTPUT PARAMS    :   None
//
// GLOBALS USED     :   
//
// RETURNED VALUE   :   None
//
///////////////////////////////////////////////////////////////////////////////

void EventInit()
{
	eventState = EVENT_STATE_IDLE;
	linkStatus = SIMON_LINK_DOWN;

	// sets-up Timer that tells us to check the Link Status
   	HalTimerSet(TIMER_LINK_CHECK, 1000);	// First time, ASAP
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   HandleSimonResponse ()
//
// PURPOSE          :   When Simon has sent us the response to a F/W Request,
//                      we handle the resonse message here.
//
// CALLED BY        :   SerCom()
//
// INPUT PARAMS     :   OpCode: The OpCode that defines the Event Response
//                      packet.
//
// OUTPUT PARAMS    :   None
//
// GLOBALS USED     :   
//
// RETURNED VALUE   :   None
//
///////////////////////////////////////////////////////////////////////////////

void HandleSimonResponse()
{
#ifdef NEVER	//%%%%
int i;
printf("Simon Resp type:0x%x len:%d ", (int)OpCode, (int)CommandLength);//%%%%%%%%%%%
for (i = 0; i < CommandLength; i++) 
{
	//Arg(i) = InCharBuff[IDX_ARG + i];
	putbyte(0x20);
	putbyte(Arg(i) + 0x30);
}
printf("\n");
#endif

	//
	// Why you ask, do we have hardcoded case values in the switch statement
	// below?  Why not use the defined values, which is good engineering 
	// practice?  Because the Keil compiler is a POS, that's why.  Our
	// response codes are all the original request codes with bit 7 set.  So
	// the Ping Request is 0x01 and the Ping Response is 0x81.  But Keil
	// does not do well with the high-order bit set; it will not handle the
	// numbers!  So we do this instead.
	//
	switch (OpCode & 0x7F)  // strip-off the high-order bit
	{
		//case FW_EVENT_PING_RESP:
		case 1:
			eventState = EVENT_STATE_DONE;
			break;

		//case FW_EVENT_SIMCONF_RESP:
		case 2:
#ifdef IS_SIMCONFIG
			SimConfHandler(Arg(0));
#endif
			break;

		//case FW_EVENT_EDID_RESP:
		case 3:
#ifdef IS_EDID_SIM
			SimEdidHandler(CommandLength, &Arg(0));
#endif
			break;

		//case FW_EVENT_VMODE_RESP:
		case 4:
#ifdef IS_VMODE_SIM
			SimVModeHandler(Arg(0), &Arg(1));
#endif
			break;

		//case FW_EVENT_POWERUP_RESP:
		case 5:
			break;
	}
}



////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   EventTask ()
//
// PURPOSE          :   Main loop of the Events Task.  This task handles comms
//                      that consists of F/W initiated messages to Simon and
//                      handling and responses back come back from Simon.
//
// INPUT PARAMETERS :   None
//
// OUTPUT PARAMETERS:   None.
//
// RETURNED VALUES:     None.
//
// GLOBALS USED     :   
//
////////////////////////////////////////////////////////////////////////////////

void EventTask (void) 
{
	//
	// This is a State Machine
	//
	switch(eventState)
	{
		case EVENT_STATE_IDLE:		// Not waiting for any response from Simon
			// If it is time for a link check, start it
			if (HalTimerExpired(TIMER_LINK_CHECK))
			{
				// Send a Ping to check the Link 
				eventSendPing();
				// Set timeout for ping response
   				HalTimerSet((uint8_t)TIMER_LINK_RESPONSE, (uint16_t)EVENT_RESPONSE_TIME);
				eventState = EVENT_STATE_WAITING;
			}
			// Check for any other actions to start goes here
			break;

		case EVENT_STATE_WAITING:
			// Check for timeout
			if (HalTimerExpired (TIMER_LINK_RESPONSE))
			{
				eventState = EVENT_STATE_TIMEOUT;
			}
			break;

		case EVENT_STATE_TIMEOUT:
			// Ping has timed-out, link is down
			if (linkStatus == SIMON_LINK_UP)
			{
				printf("Simon Link DOWN\n");//%%%%%%%%
    			TPI_TRACE_PRINT(("Simon Link Down\n"));
			}
			linkStatus = SIMON_LINK_DOWN;
			// Set state machine back to Idle
			eventState = EVENT_STATE_IDLE;
			// Reset timer for next link check
   			HalTimerSet(TIMER_LINK_CHECK, LINK_CHECK_TIME);
			break;

		case EVENT_STATE_DONE:
			// Ping response received
			if (linkStatus == SIMON_LINK_DOWN)
			{
				printf("Simon Link UP\n");//%%%%%%%%
    			TPI_TRACE_PRINT(("Simon Link UP\n"));
			}
			linkStatus = SIMON_LINK_UP;				// Set Link status
			eventState = EVENT_STATE_IDLE;
			// Reset timer for next link check
   			HalTimerSet(TIMER_LINK_CHECK, LINK_CHECK_TIME);
			break;

		default:
			eventState = EVENT_STATE_IDLE;
			// Reset timer for next link check
   			HalTimerSet(TIMER_LINK_CHECK, LINK_CHECK_TIME);
			break;
	}

}

#endif // IS_EVENT
