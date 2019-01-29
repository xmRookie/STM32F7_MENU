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
#ifndef _EVENT_H_
#define _EVENT_H_

#ifdef IS_EVENT

//
// Defines and Typedefs
//
typedef enum
{
    SIMON_LINK_DOWN = 0,
    SIMON_LINK_UP
} simonLinkStatus_t;

// Event State Machine
typedef enum
{
    EVENT_STATE_IDLE = 0,
    EVENT_STATE_WAITING,
    EVENT_STATE_TIMEOUT,
    EVENT_STATE_DONE
} eventState_t;

typedef enum
{
    EVENT_SUB_STATE_IDLE = 0,
    EVENT_SUB_STATE_BUSY_PING,
    EVENT_SUB_STATE_BUSY_EDID_SIM,
    EVENT_SUB_STATE_BUSY_VIDEO_SIM
} eventSubState_t;

// Simulation bits
#define	SIM_EDID_ON	0x80
#define SIM_EDID_MASK	(~SIM_EDID_ON)
#define	SIM_VMODE_ON	0x40
#define SIM_VMODE_MASK	(~SIM_VMODE_ON)


//
// Externs and prototypes
//
extern eventState_t		eventState;
extern eventSubState_t		eventSubState;
extern simonLinkStatus_t	linkStatus;

extern void EventCheckLink(void);
extern void EventInit(void);
extern void HandleSimonResponse(void);
extern void EventTask (void);



#endif // IS_EVENT
#endif // _EVENT_H_
