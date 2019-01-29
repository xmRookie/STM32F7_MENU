/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

//
// Serial Communication Buffer assignments and macros
//
#define Command						g_CommData[0]
#define OpCode 						g_CommData[0]
#define CommandLength				g_CommData[1] 
#define Arg(a)                      g_CommData[(2+a)]

#define WRITE_VIDEO_MODE_CONFIG			0x01
#define READ_VIDEO_MODE_CONFIG			(0x80 | WRITE_VIDEO_MODE_CONFIG)

#define READ_VIDEO_SYNC_CONFIG			(0x80 | 0x02)

#define WRITE_XVYCC_CONFIG				0x03

#define WRITE_AUDIO_MODE_CONFIG			0x08
#define READ_AUDIO_MODE_CONFIG			(0x80 | WRITE_AUDIO_MODE_CONFIG)

// This command does not support readback because of the channel mapping architecture of the Tx chip
#define WRITE_I2S_MAPPING				0x09

#define WRITE_I2S_CONFIG				0x0A
#define READ_I2S_CONFIG					(0x80 | WRITE_I2S_CONFIG)

#define WRITE_I2S_STREAM_HEADER			0x0B
#define READ_I2S_STREAM_HEADER			(0x80 | WRITE_I2S_STREAM_HEADER)

#define WRITE_INFOFRAME_DATA			0x10
#define READ_INFOFRAME_DATA				(0x80 | WRITE_INFOFRAME_DATA)
//
#define WRITE_AVI_INFOFRAME_DATA		0x10+100
#define READ_AVI_INFOFRAME_DATA			(0x80 | WRITE_AVI_INFOFRAME_DATA)

#define WRITE_AUDIO_INFOFRAME_DATA 		0x11
#define READ_AUDIO_INFOFRAME_DATA 		(0x80 | WRITE_AUDIO_INFOFRAME_DATA)

#define WRITE_EHDMI_MODE_CONFIG			0x20
#define READ_EHDMI_MODE_CONFIG			(0x80 | WRITE_EHDMI_MODE_CONFIG)

#define WRITE_HDCP_OVERRIDE_MODE		0x30
#define READ_HDCP_OVERRIDE_MODE			(0x80 | WRITE_HDCP_OVERRIDE_MODE)

#define WRITE_CEC_ACTION				0x40
#define READ_CEC_ACTION					(0x80 | WRITE_CEC_ACTION)
#define	CEC_ACTION_ONETOUCH				0

#define WRITE_REGISTER_BLOCK			0x60
#define READ_REGISTER_BLOCK				(0x80 | WRITE_REGISTER_BLOCK)

#define READ_PRODUCT_CODE				(0x80 | 0x61)
#define READ_FIRMWARE_VERSION			(0x80 | 0x62)
#define READ_CONTROL_COMMAND_VERSION    (0x80 | 0x63)		


// F/W Event OpCodes
#define FW_EVENT_PING					0x01
#define FW_EVENT_PING_RESP				(0x80 | FW_EVENT_PING)
#define FW_EVENT_SIMCONF_REQ			0x02
#define FW_EVENT_SIMCONF_RESP			(0x80 | FW_EVENT_SIMCONF_REQ)
#define FW_EVENT_EDID_REQ				0x03
#define FW_EVENT_EDID_RESP				(0x80 | FW_EVENT_VMODE_REQ)
#define FW_EVENT_VMODE_REQ				0x04
#define FW_EVENT_VMODE_RESP				(0x80 | FW_EVENT_EDID_REQ)
#define FW_EVENT_POWERUP_REQ			0x05
#define FW_EVENT_POWERUP_RESP			(0x80 | FW_EVENT_POWERUP_REQ)

//
// Simulation Configuration Bits
//
#define	FW_SIM_EDID_ON					0x80
#define	FW_SIM_VMODE_ON					0x40
#define	FW_SIM_POWERUP_ON				0x20

// Command indexes:
//=====================
#define IDX_CMD							0
#define IDX_LEN							1
#define IDX_ARG							2

// Misc Constants
//===============
#define START_OF_PACKET					0xFF
#define UNKNOWN_COMMAND					0xFF
#define START_OF_FW_PACKET				0xFE


