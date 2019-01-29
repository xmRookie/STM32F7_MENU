//***************************************************************************
//!file     Event.h
//!brief    Defines, structures and prototypes to support Events Comms.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2010, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/
#ifndef _SIMCONF_H_
#define _SIMCONF_H_

//#if (IS_SIMCONFIG == 1)

//
// Defines and Typedefs
//

#define SIMCONF_RESPONSE_TIME	2000		// 1000ms

// SimConf State Machine
typedef enum
{
    SIMCONF_STATE_UNINIT = 0,
    SIMCONF_STATE_WAITING_CONFIG,
	SIMCONF_STATE_PROCESS_CONFIG,
    SIMCONF_STATE_REQ_VMODE,
    SIMCONF_STATE_PROCESS_VMODE,
    SIMCONF_STATE_REQ_EDID,
    SIMCONF_STATE_PROCESS_EDID,
    SIMCONF_STATE_TIMEOUT,
    SIMCONF_STATE_DONE
} simConfState_t;

typedef enum
{
    SIMCONF_SUB_STATE_CONFIG = 0,
    SIMCONF_SUB_STATE_POWERUP,
    SIMCONF_SUB_STATE_VMODE,
    SIMCONF_SUB_STATE_EDID,
    SIMCONF_SUB_STATE_IDLE
} simConfSubState_t;


//
// Externs and prototypes
//
extern simConfState_t		simConfState;
extern simConfSubState_t		simConfSubState;

extern void SimConfInit(void);
extern void SimConfTask(void);
extern void SimConfHandler(uint8_t config);
extern void SimVModeHandler(uint8_t status, byte*);
extern void SimEdidHandler(uint8_t edSize, byte *eData);


#endif // IS_SIMCONF
//#endif // _SIMCONF_H_
