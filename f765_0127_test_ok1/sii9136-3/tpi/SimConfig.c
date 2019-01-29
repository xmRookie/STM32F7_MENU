//***************************************************************************
//!file     simConfig.c
//!brief    Simulation Configuration & Control Task
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
#include "common.h"
//#include "string.h"
#include "defs.h"
#include "Constants.h"
#include "TypeDefs.h"
//#include "MCU_Regs.h"
#include "Externals.h"
#include "AV_Config.h"
#include "i2c_master_sw.h"
#include "TPI_Access.h"
#include "TPI_Regs.h"
//#include "stdio.h"
#include "tpi.h"
#include "si_datatypes.h"
#include "si_apiConfigure.h"
#include "hal_timers.h"
#include "edid.h"
#include "Packet.h"
#include "Event.h"
#include "SimConfig.h"

#ifdef F_9136 
#include "ehdmi.h"
#endif

#ifdef IS_SIMCONFIG

//
// External
//
extern	simonLinkStatus_t	linkStatus;
extern	byte				putbyte(byte);

//
// Local defines
//
typedef enum
{
	VMODE_STATE_IDLE = 0,
	VMODE_STATE_WAITING_REQ,
	VMODE_STATE_PROCESS,
	VMODE_STATE_ERROR,
	VMODE_STATE_DONE
} vmode_state_t;

typedef enum
{
	POWERUP_STATE_IDLE = 0,
	POWERUP_STATE_WAITING_REQ,
	POWERUP_STATE_PROCESS,
	POWERUP_STATE_ERROR,
	POWERUP_STATE_DONE
} powerup_state_t;

typedef enum
{
	EDID_STATE_IDLE = 0,
	EDID_STATE_WAITING_REQ,
	EDID_STATE_PROCESS,
	EDID_STATE_ERROR,
	EDID_STATE_DONE
} edid_state_t;


//
// Globals
//
simConfState_t		simConfState;
simConfSubState_t	simConfSubState;
uint8_t				simConfig;
byte				*pEDID;					// pointer to the EDID data in play
uint8_t				edidSize;

//
// Locals
//
static	powerup_state_t	PowerupState;
static	vmode_state_t	VModeState;
static	edid_state_t	EdidState;
static	int				ExpectedEdid;		// Block ID of the current block in play
static	byte			numExt;				// Number of extensions on the EDID

extern byte Parse861LongDescriptors(byte *Data);
///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   SimConfInit ()
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

void SimConfInit()
{
	simConfig = 0;		// Start with all simulations off
	simConfState = SIMCONF_STATE_UNINIT;
	simConfSubState = SIMCONF_SUB_STATE_IDLE;
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         : SimConfHandler(),  SimVModeHandler()
//
// PURPOSE          :	These functions are the Handlers for Simulation Response
// 						messages from Simon.  They parse-out any data, check
// 						for errors and then set the State for the task that is
// 						waiting for this response message.
//
// CALLED BY        :   Event code
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

// Handles incoming response message to Simulation Config request
void SimConfHandler (uint8_t config)
{
	simConfig = config;
	simConfState = SIMCONF_STATE_PROCESS_CONFIG;
}

// Handles incoming response message to Simulation Video Mode request
void SimVModeHandler (uint8_t status, byte * modeData)
{
	if (!status)
	{
		// Copy the contents of the message into the VideoModeDescription
		byte *pTable = (byte*)&VideoModeDescription;
		memcpy(pTable, modeData, sizeof(VideoModeDescription_t));
		printf("Video Sim has set: ");
		printf(" V:%d", (int) VideoModeDescription.VIC);
		printf(" A:%x", (int) VideoModeDescription.AspectRatio);
		printf(" CS:%x", (int) VideoModeDescription.ColorSpace);
		printf(" CD:%x", (int) VideoModeDescription.ColorDepth);
		printf(" HA:%d", (int) VideoModeDescription.HDCPAuthenticated);
		printf(" HF:%d", (int) VideoModeDescription.HDMIVideoFormat);
		printf(" 3D:%d\n", (int) VideoModeDescription.ThreeDStructure);
		VModeState = VMODE_STATE_PROCESS;
	}
	else
	{
		VModeState = VMODE_STATE_ERROR;
	}
}

// Handles incoming response message to edid Simulation request
void SimEdidHandler (uint8_t edSize, byte *eData)
{
	if (edSize)
	{
		// Set to process this EDID block
		EdidState = EDID_STATE_PROCESS;
		pEDID = eData;
		edidSize = edSize;
	}
	else
	{
		// An EDID data size of Zero indicates an error!  We fail!
		EdidState = EDID_STATE_ERROR;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   powerupeConfTask ()
//
// PURPOSE          :   TBD
//
// CALLED BY        :   SimConfTask()
//
// INPUT PARAMS     :	None 
//
// OUTPUT PARAMS    :   None
//
// GLOBALS USED     :   
//
// RETURNED VALUE   :   TRUE if done
//
///////////////////////////////////////////////////////////////////////////////

static bool powerupeConfTask()
{
#ifdef IS_POWERUP_SIM
	// STUB %%%
	
	// Request Power-Up Configuration Data message from Simon
	
	// Wait for response
	
	// Process Response
	
#endif // IS_SIM_VMODE

	// Done
	return(true);
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   vmodeConfTask ()
//
// PURPOSE          :   Handles simulated Video Mode configuration.  If will
// 						send a Video Mode Request message to Simon.  When it
// 						gets and answer, it then does the approreate setup.
//
// CALLED BY        :   SimConfTask()
//
// INPUT PARAMS     :	None 
//
// OUTPUT PARAMS    :   None
//
// GLOBALS USED     :   simConfigsimConfigCommandLength
//
// RETURNED VALUE   :   TRUE if done
//
///////////////////////////////////////////////////////////////////////////////

static bool vmodeConfTask()
{
#ifdef IS_VMODE_SIM
	bool	doneFlag = false;

	if (!(simConfig & FW_SIM_VMODE_ON))
	{
		// Video Simulation is not turned ON
		VModeState = VMODE_STATE_DONE;
	}

	//
	// This is a state machine where we do the following:
	// * Ask Simon for a Video Mode Configuration Message
	// * Wait for message or timeout.
	// * Process response, or detect timeout, then set state DONE
	//
	switch(VModeState)
	{
		case VMODE_STATE_IDLE:
			// Send a Video Mode Request
			/*putbyte(START_OF_FW_PACKET);
			putbyte(FW_EVENT_VMODE_REQ);
			putbyte(0);*/		// No data bytes
			// Set the response timeout timer
   			HalTimerSet(TIMER_LINK_RESPONSE, SIMCONF_RESPONSE_TIME);
			// Goto next state
			VModeState = VMODE_STATE_WAITING_REQ;
			break;

		case VMODE_STATE_WAITING_REQ:
			// Check for timeout
			if (HalTimerExpired (TIMER_LINK_RESPONSE))
			{
				// No response - Abort!
				printf("FW_EVENT_VMODE_REQ response timeout!\n");
				VModeState = VMODE_STATE_ERROR;
			}
			break;

		case VMODE_STATE_PROCESS:
			// We have received a response from Simon
			InitVideo(X1);
			VModeState = VMODE_STATE_DONE;	// assume it was successfull for now
			break;

		case VMODE_STATE_DONE:
		case VMODE_STATE_ERROR:
		default:
			doneFlag = true;
			break;
	}

#else
	doneFlag = true;
#endif // IS_SIM_VMODE

	return(doneFlag);
}



///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   edidConfTask ()
//
// PURPOSE          : 	Handled simulated EDID configuration.
//
// CALLED BY        :   SimConfTask()
//
// INPUT PARAMS     :   None
//
// OUTPUT PARAMS    :   Completed flag
//
// GLOBALS USED     :   
//
// RETURNED VALUE   :   true if done
//
///////////////////////////////////////////////////////////////////////////////

static bool edidConfTask()
{
#ifdef IS_EDID_SIM
	bool	doneFlag = false;
	byte	Result;

	if (!(simConfig & FW_SIM_EDID_ON))
	{
		// EDID Simulation is not turned ON
		EdidState = EDID_STATE_DONE;
	}

	//
	// This is a state machine where we do the following:
	// * Ask Simon for a Video Mode Configuration Message
	// * Wait for message or timeout.
	// * Process response Block 0, or detect timeout.
	// * If Block zero shows more blocks, then read the next
	//   until done with all.
	//
	switch(EdidState)
	{
		case EDID_STATE_IDLE:
			ExpectedEdid = 0;
			// Send a EDID Block 0 Request
			/*putbyte(START_OF_FW_PACKET);
			putbyte(FW_EVENT_EDID_REQ);
			putbyte(1);					// 1 data byte
			putbyte(ExpectedEdid);*/		// EDID Block Zero
			// Set the response timeout timer
   			HalTimerSet(TIMER_LINK_RESPONSE, SIMCONF_RESPONSE_TIME);
			// Goto next state
			EdidState = EDID_STATE_WAITING_REQ;
			break;

		case EDID_STATE_WAITING_REQ:
			// Check for timeout
			if (HalTimerExpired (TIMER_LINK_RESPONSE))
			{
				// No response - Abort!
				printf("EDID %d response timeout!\n", ExpectedEdid);
				EdidState = EDID_STATE_ERROR;
			}
			break;

		case EDID_STATE_PROCESS:
printf("starting processing of EDID:%d\n", (int)ExpectedEdid);//%%%%%%%%%%%
			// Do EDID processing
			if (ExpectedEdid == 0)
			{
				// Block 0 has its own seperate processing.  The value returned
				// in nemExt is the number of extension block to process.
				Result = ParseEDID(pEDID, &numExt);
				if (Result != EDID_OK)
				{
					if (Result == EDID_NO_861_EXTENSIONS)
					{
						EDID_Data.HDMI_Sink = FALSE;
						//TPI_DEBUG_PRINT (("EDID -> No 861 Extensions\n"));
						printf("EDID -> No 861 Extensions\n");
					}
					else
					{
						//TPI_DEBUG_PRINT (("EDID -> Parse FAILED\n"));
						printf("EDID -> Parse FAILED\n");
						EdidState = EDID_STATE_ERROR;
						break;
					}
				}
				else
				{
					//TPI_DEBUG_PRINT (("EDID -> Parse OK\n"));
					printf("EDID -> Parse OK. Num Extensions = %d\n", (int)numExt);//%%%
				}
			}
			else
			{
				// Block 1 and up are 861 blocks
        		Result = Parse861ShortDescriptors(pEDID);
        		if (Result != EDID_SHORT_DESCRIPTORS_OK)
        		{
					printf("EDID -> Block %d Short Desc Parse FAILED. Error: %d\n", (int)ExpectedEdid, (int)Result);//%%%%
					EdidState = EDID_STATE_ERROR;
					break;
        		}
        		Result = Parse861LongDescriptors(pEDID);
        		if (Result != EDID_LONG_DESCRIPTORS_OK)
        		{
					printf("EDID -> Block %d Long Desc Parse FAILED\n", (int)ExpectedEdid);//%%%%
					EdidState = EDID_STATE_ERROR;
					break;
        		}
			}
			// Was this the last block in the EDID?
			if (ExpectedEdid == numExt)
			{
printf("EDID processing completed\n");//%%%%%%%%%%%
				EdidState = EDID_STATE_DONE;	// if this was last block
				break;
			}
			++ExpectedEdid;
			// Send a request for the next EDID block
			/*putbyte(START_OF_FW_PACKET);
			putbyte(FW_EVENT_EDID_REQ);
			putbyte(1);					// 1 data byte
			putbyte(ExpectedEdid);*/		// Next EDID Block
			// Set the response timeout timer
   			HalTimerSet(TIMER_LINK_RESPONSE, SIMCONF_RESPONSE_TIME);
			// Goto next state
			EdidState = EDID_STATE_WAITING_REQ;
			break;

		case EDID_STATE_ERROR:
		case EDID_STATE_DONE:
		default:
			doneFlag = true;
			break;
	}
#else
	bool	doneFlag = true;
#endif // IS_SIM_VMODE

	return(doneFlag);
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   SimConfigTask ()
//
// PURPOSE          :   Main loop of the Simulation and Configuration Task.  This
// 						task handles the remote configuration and simulation
// 						features supported by Simon.  These consist of:
//                      * EDID Simulation - when a real EDID is not enough
//                      * Video Mode Simulation - When no AMF is available
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

void SimConfTask (void) 
{
	//
	// This is a State Machine
	//
	switch(simConfState)
	{
		case SIMCONF_STATE_UNINIT:
			//
			// This state is where we wait for a link to Simon.  Once
			// a link is detected, we start the configuration sequence.
			// 
			if (linkStatus == SIMON_LINK_UP)
			{
				// The link has come up.  Prepare to do the Simulation
				// setups and set to the next state.
				simConfState = SIMCONF_STATE_WAITING_CONFIG;
				simConfSubState = SIMCONF_SUB_STATE_CONFIG;

				// Send a Simulation Configuration Request
				/*putbyte(START_OF_FW_PACKET);
				putbyte(FW_EVENT_SIMCONF_REQ);
				putbyte(0);*/		// No data bytes

				// Set the response timeout timer
   				HalTimerSet(TIMER_LINK_RESPONSE, SIMCONF_RESPONSE_TIME);
			}
			break;

		case SIMCONF_STATE_WAITING_CONFIG:	// Simon has been sent a request, not yet answered
			// Check for timeout
			if (HalTimerExpired (TIMER_LINK_RESPONSE))
			{
				// No response - perhapes the link went down.  Let's try again when the link is up
				printf("FW_EVENT_SIMCONF_REQ response timeout!\n");
				simConfState = SIMCONF_STATE_DONE;
			}
			break;

		case SIMCONF_STATE_PROCESS_CONFIG:
			// We now know our simulation configuration. Go through each seperate configuation
			// Task.  We stay in this state until all these tasks conplete.  A Task will signal
			// complettion, be it successful or with an error, by returning true.
			switch(simConfSubState)
			{
				case SIMCONF_SUB_STATE_CONFIG:
					// Set sub-state for our first task
					simConfSubState = SIMCONF_SUB_STATE_POWERUP;
					// Initialize States for seperate tasks
					PowerupState = POWERUP_STATE_IDLE;
					VModeState = VMODE_STATE_IDLE;
					EdidState = EDID_STATE_IDLE;
					break;

				// Currently executing the PowerUp Task
				case SIMCONF_SUB_STATE_POWERUP:
					if (powerupeConfTask())
					{
						// Done - Go to next task
						simConfSubState = SIMCONF_SUB_STATE_VMODE;
					}
					break;

				// Currently executing the Video Mode Task
				case SIMCONF_SUB_STATE_VMODE:
					if (vmodeConfTask())
					{
						// Done - Go to next task
						simConfSubState = SIMCONF_SUB_STATE_EDID;
					}
					break;

				// Currently executing the Simulated EDID Task
				case SIMCONF_SUB_STATE_EDID:
					if (edidConfTask())
					{
						// Done - Last task so go to IDLE
						simConfSubState = SIMCONF_SUB_STATE_IDLE;
					}
					break;

				// All Tasks completed, go to DONE state
				case SIMCONF_SUB_STATE_IDLE:
				default:
					simConfState = SIMCONF_STATE_DONE;
					break;
			}
			break;


		case SIMCONF_STATE_TIMEOUT:
			// Simon has failed to send us a configuration!
			break;


		case SIMCONF_STATE_DONE:
#ifdef NEVER
			// Test for the link going dowm - if so....
			if (linkStatus == SIMON_LINK_DOWN)
			{
				// Woops, the link has gone down, so we must prepare to
				// do it all again.
				simConfState = SIMCONF_STATE_UNINIT;
			}
#endif
			break;

		default:
			simConfState = SIMCONF_STATE_DONE;
			simConfSubState = SIMCONF_SUB_STATE_IDLE;
			break;
	}

}

#endif // IS_SIMCONFIG
