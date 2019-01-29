/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
/*#include "common.h"
#include "defs.h"
#include "Constants.h"
#include "TypeDefs.h"
//#include "MCU_Regs.h"
#include "Externals.h"
#include "AV_Config.h"
#include "i2c_master_sw.h"
#include "TPI_Access.h"
#include "TPI_Regs.h"
#include "stdio.h"
#include "SerialPort.h"
#include "tpi.h"
#include "si_datatypes.h"
#include "si_apiConfigure.h"
#include "Packet.h"

#ifdef IS_EVENT
#include "Event.h"
#endif

#ifdef F_9136 
#include "ehdmi.h"
#endif

#if (IS_CEC == 1)
extern	BOOL SI_CecOneTouchPlay (void);
#endif

extern	UART_HandleTypeDef huart3;
static void ExecuteGetCommand (); 
static byte ExecuteSetCommand ();


static byte putbyte (byte b);

byte InCharBuff[32];

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :               SerComm ()
//
// PURPOSE          :   Dequeue characters from the serial port input queue
//                      RxQueue. Parse the received characters. Build the
//                      command packet and run it.
//
// INPUT PARAMETERS :   None
//
// OUTPUT PARAMETERS:   None.
//
// RETURNED VALUES:     None.
//
// GLOBALS USED     :   F_SBUF_DataReady, InCharBuff
//
////////////////////////////////////////////////////////////////////////////////

void SerComm (void) 
{
	byte i;

	// Run SerComm only if a compete packet has been collected from the serial port
	if (F_SBUF_DataReady) 
	{	
		for (i = 0; i < GLOBAL_BYTE_BUF_BLOCK_SIZE; i++)
		{
			g_CommData[i] = 0;
		}				
		OpCode = InCharBuff[IDX_CMD];
		CommandLength = InCharBuff[IDX_LEN];

		for (i = 0; i < CommandLength; i++) 
        {
			Arg(i) = InCharBuff[IDX_ARG + i];
		}

		// Regular Get/Set command packets from Simon
		if (MessageType == START_OF_PACKET)
		{
			// Check if this command is a "Get" or a "Set"
			if (OpCode & BIT_7)
        	{			
				ExecuteGetCommand();
			}
			else 
        	{
				ExecuteSetCommand();
			}
		}
#ifdef IS_EVENT
		// Reply from Simon to a request from F/W
		else if (MessageType == START_OF_FW_PACKET)
		{
			// Handle F/W request response from Simon
			HandleSimonResponse();
		}
#endif //EVENT

		// Clear this flag so we can receive the next message
		F_SBUF_DataReady = FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////////
//
// FUNCTION     :   ExecuteSetCommand()
//
// PURPOSE      :   Run setup 9022A/4A setup functions
//
// INPUT PARAMS :   API_Cmd type structure
//
// OUTPUT PARAMS:   None
//
// GLOBALS USED :   global serial input buffer
//
// RETURNS      :   TRUE
//
//////////////////////////////////////////////////////////////////////////////
byte ExecuteSetCommand()
{
	int i;
	byte len = 0;
	byte regOffset = 0;

	switch (OpCode)
	{
		case WRITE_VIDEO_MODE_CONFIG:
			ChangeVideoMode();  
			break;

#ifdef F_9022A_9334
		case WRITE_XVYCC_CONFIG:
			SetGBD_InfoFrame();
			break;
#endif

		case WRITE_AUDIO_MODE_CONFIG:
			SetAudioMode();  
			break;

		case WRITE_I2S_MAPPING:
			MapI2S();  
			break;

		case WRITE_I2S_CONFIG:
			ConfigI2SInput();  
			break;

		case WRITE_I2S_STREAM_HEADER:
			SetI2S_StreamHeader();  
			break;

#ifdef DEV_SUPPORT_EHDMI
		case WRITE_EHDMI_MODE_CONFIG:
			switch (Arg(0))
			{
				case 0x00:
					EHDMI_Disable();
					break;
				case 0x01:
					EHDMI_ARC_Single_Enable();
					break;
				case 0x02:
					EHDMI_ARC_Common_Enable();
					break;
				case 0x03:
					EHDMI_HEC_Enable();
					break;
				case 0x04:
					EHDMI_ARC_Common_With_HEC_Enable();
					break;
			}
			break;
#endif

		case WRITE_AVI_INFOFRAME_DATA:
			// Calculate checksum
		  	Arg(0) = 0x82 + 0x02 +0x0D;
			Arg(13) = 0;

	    	for (i = 1; i < SIZE_AVI_INFOFRAME; i++)
			{
	        	Arg(0) += Arg(i);
			}

			Arg(0) = 0x100 - Arg(0);

			WriteBlockTPI(TPI_AVI_BYTE_0, SIZE_AVI_INFOFRAME, &Arg(0));
			break;

		case WRITE_AUDIO_INFOFRAME_DATA:
			// Calculate checksum
			switch(Arg(0))
			{
				case 0x81:	// Vendor specific
					Arg(4) = 0x81 + 0x01 + 0x07;
 				  	break;
//				case 0x83:	//Source Product Descriptor (SPD)
//					Command.Arg[4] = 0x83 + 0x01 + 0x19;
//   					break;
				case 0x84:	// Audio
					Arg(4) = 0x84 + 0x01 + 0x0A;
   					break;
				case 0x85:	// MPEG Source
					Arg(4) = 0x85 + 0x01 + 0x0A;
   					break;
			}

			for (i = 5; i < SIZE_AUDIO_INFOFRAME; i++)
			{
        		Arg(4) += Arg(i);
			}

			Arg(4) = 0x100 - Arg(4);

			WriteBlockTPI(TPI_AUDIO_BYTE_0, SIZE_AUDIO_INFOFRAME, &Arg(0));
			break;

		case WRITE_INFOFRAME_DATA:
			// A little Sanity check
			if (	Arg(0) == AVI
					||  Arg(0) == SPD
					||  Arg(0) == AUDIO
					||  Arg(0) == MPEG
					||  Arg(0) == GEN_1
					||  Arg(0) == GEN_2
					||  Arg(0) == HDMI_VISF
					||  Arg(0) == GBD)
			{
				if (Arg(0) == AVI)
				{
					// Calculate checksum
					// Brought to you from Programmer's Referece; Calculating the Checksum
					byte AVI_CRC = 0x82 + 0x02 + 13; // Identifier code for AVI InfoFrame, length
					for(i = 1; i <= SIZE_AVI_INFOFRAME; i++ )
					{
						AVI_CRC += Arg(i);
					}
					AVI_CRC = 0x100 - AVI_CRC;
					Arg(1)=AVI_CRC;
					regOffset = TPI_AVI_INFO_REG_ADDR;
					len = SIZE_AVI_INFOFRAME;

				}
				else if (Arg(0) == SPD)
				{
					len = SIZE_SPD_INFOFRAME;
				}
				else if (Arg(0) == AUDIO)
 				{
					len = SIZE_AUDIO_INFOFRAME;
				}
				else if (Arg(0) == MPEG)
				{
#ifdef F_9136 
					len = SIZE_MPEG_INFOFRAME;
#else
					len = SIZE_MPRG_HDMI_INFOFRAME;
#endif
				}
				else if (Arg(0) == GEN_1)
				{
					len = SIZE_GEN_1_INFOFRAME;
				}
				else if (Arg(0) == GEN_2)
				{
					len = SIZE_GEN_2_INFOFRAME;
				}
				else if (Arg(0) == HDMI_VISF)
 				{
					len = SIZE_HDMI_VISF_INFOFRAME;
				}
				else if (Arg(0) == GBD)
				{
					len = SIZE_GBD_INFOFRAME;
			   	}
				if (Arg(0) != AVI)
				{
					WriteByteTPI (TPI_OTHER_INFO_REG_ADDR, Arg(0));
					regOffset = TPI_INFO_FRAME_REG_OFFSET;
				}
				WriteBlockTPI(regOffset, len, &Arg(1));
			}
			else
			{
				printf(" ****** Invalid InfoFrame Argument encountered ********\n");
			}		 
		break;

#ifdef DEV_SUPPORT_HDCP
		case WRITE_HDCP_OVERRIDE_MODE:
			if (Arg(0) == 0)
			{
				g_HDCP_Override = FALSE;
			}
			else if (Arg(0) == 1)
			{
				g_HDCP_Override = TRUE;
			}
			else
			{
				printf(" ****** Invalid HDCP Override Argument encountered ********\n");
			}
			break;
#endif

#if (IS_CEC == 1)
		case WRITE_CEC_ACTION:
			if (Arg(0) == CEC_ACTION_ONETOUCH)
			{
				SI_CecOneTouchPlay ();
			}
			else
			{
				printf(" ****** Invalid CEC ACTION encountered ********\n");
			}
			break;
#endif // IS_CEC

		case WRITE_REGISTER_BLOCK:
			I2C_WriteBlock(Arg(0), Arg(1), &Arg(3), (word) Arg(2));
			break;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   ExecuteGetCommand ()
//
// PURPOSE          :   Write the "Get" packet byte by  byte to the
//                      serial Port. Use Busy Wait instead of Serial Port
//                      interrupts in order to avoid conflicts with printf()
//
// CALLED BY        :   RunTxCommand()
//
// INPUT PARAMS     :   C_OpCode: The OpCode that defines the info in the Get
//                      packet; V_Mode: the current video mode set (according to
//                      CEA-861-D).
//
// OUTPUT PARAMS    :   None
//
// GLOBALS USED     :   g_audio_Checksum
//
// RETURNED VALUE   :   TRUE if successful. FALSE if failed
//
///////////////////////////////////////////////////////////////////////////////

static void ExecuteGetCommand()
{
    byte RegValue;
    byte RegValue2;
    byte i;
	byte DataAvi[SIZE_AVI_INFOFRAME];
	byte DataMisc[SIZE_AUDIO_INFOFRAME];

	// Video Sync Data
	byte CurrentSyncState;
	byte DEsyncData[DeDataNumBytes];
	byte EmbSyncData[EmbDataNumBytes];

	byte len;

	putbyte(START_OF_PACKET);

	switch (OpCode)
	{

		case READ_VIDEO_MODE_CONFIG:
			putbyte(OpCode);
			putbyte(VIDEO_SETUP_CMD_LEN);

			// Arg[0]: 0x00..0x07, 0x62..0x6D
			// Video modes
            putbyte(VideoModeDescription.VIC);

			// Arg[1]: 0x08
			// TPI INput Bus and Pixel Repetition Data
            RegValue = ReadByteTPI(TPI_PIX_REPETITION);
            putbyte(RegValue);

			// Arg[2]: 0x09
			// TPI AVI Input Format Data
            RegValue = ReadByteTPI(TPI_INPUT_FORMAT_REG);
            putbyte(RegValue);

			// Arg[3]: 0x0A
			// TPI AVI Output Format Data
            RegValue = ReadByteTPI(TPI_OUTPUT_FORMAT_REG);
            putbyte(RegValue);

			// Arg[4]: 0x60 
			// Sync Generation Control Register
			RegValue = ReadByteTPI(TPI_SYNC_GEN_CTRL);
            putbyte(RegValue);

			// Arg[5]: 0x61 (Read Only)
			// Video Sync Polarity Detection Register 
			RegValue = ReadByteTPI(TPI_SYNC_POLAR_DETECT);
 			putbyte(RegValue);

#ifdef F_9022A_9334
			// Arg[6]: 0x0B
			// TPI YC Input Mode Select
			RegValue = ReadByteTPI(TPI_YC_Input_Mode);
 			putbyte(RegValue);
#endif
			// Arg[7]: 0x0E & 0x0F
			// AVI DBYTE 2 - Colorimetry, AVI DBYTE 3 - Extended Colorimetry
			RegValue = ReadByteTPI(TPI_AVI_BYTE_2);			// 0x0E[7:6] - Colorimetry
			RegValue >>= 4;
			RegValue2 = ReadByteTPI(TPI_AVI_BYTE_3); // 0x0F[6:4] - Extended Colorimetry
			putbyte((RegValue & BITS_3_2) | (RegValue2 & BITS_6_5_4));  // Arg[6][3:2] - Colorimetry
                                                                        // Arg[6][6:4] - Extended Colorimetry			
#ifdef F_9136 
			// Arg[8]: 0xC8[7:4] & 0xC9[7:4]
			WriteByteTPI(MISC_INFO_FRAMES_CTRL, EN_AND_RPT_MPEG);	// Select MPEG/GBD infoframe
			RegValue2 = ReadByteTPI(TPI_INFO_FRM_DBYTE5) >> 4;
			if (RegValue2 == 0x08)
			{
				RegValue = ReadByteTPI(TPI_INFO_FRM_DBYTE6) & BITS_6_5_4;
			}
			else
			{
				RegValue = 0;
			}

			RegValue |= RegValue2;
			putbyte(RegValue);	

#else
			putbyte(0x00);
#endif
            break;

		case READ_VIDEO_SYNC_CONFIG:

			// Preserve the current state of Device Vide Sync
			CurrentSyncState = ReadByteTPI(TPI_SYNC_GEN_CTRL);

			// Sync DE Generator
			ReadModifyWriteTPI(TPI_SYNC_GEN_CTRL, BIT_7, DE);
			ReadBlockTPI(TPI_SYNC_GEN_CTRL, DeDataNumBytes, DEsyncData);

			// Sync Embedded 
			ReadModifyWriteTPI(TPI_SYNC_GEN_CTRL, BIT_7, EMB);
			ReadBlockTPI(TPI_SYNC_GEN_CTRL, EmbDataNumBytes, EmbSyncData);

			// Restore Sync State
			WriteByteTPI(TPI_SYNC_GEN_CTRL, CurrentSyncState);

			putbyte(OpCode);
			putbyte(VIDEO_SYNC_SETUP_CMD_LEN);
			putbyte(DeDataNumBytes + EmbDataNumBytes);

			// DE-Generator data; Arg[0]-Arg[11]
			for (i = 0; i < DeDataNumBytes; i++)
			{
				putbyte(DEsyncData[i]);
			}
			
			// Embedded Sync data; Arg[12]-Arg[19]
			for (i = 0; i < EmbDataNumBytes; i++)
			{
				putbyte(EmbSyncData[i]);
			}
			break;

		case READ_AUDIO_MODE_CONFIG:
			putbyte(OpCode);
			putbyte(AUDIO_SETUP_CMD_LEN);

#ifdef F_9136 
			RegValue = ReadByteTPI(MISC_INFO_FRAMES_TYPE);	//	Check if it's audio infoframe type
			if (RegValue != 0x84)	
				WriteByteTPI(TPI_AUDIO_BYTE_0, EN_AUDIO_INFOFRAMES); 	// if not audio infoframe type, write 0xC2 to 0xBF to select Audio InfoFrames
#endif
			RegValue = ReadByteTPI(TPI_CODING_TYPE_CHANNEL_COUNT); 		// Arg[0][3:0] - 0xC4[7:4]
            RegValue >>= 4;
           	RegValue2 = ReadByteTPI(TPI_AUDIO_INTERFACE_REG);        	// Arg[0][7:4]: 0x26[7:4]
   			putbyte((RegValue & LOW_NIBBLE) | (RegValue2 & HI_NIBBLE));

			RegValue = ReadByteTPI(TPI_AUDIO_SAMPLE_CTRL);				// Arg[1]: 0x27
			putbyte(RegValue);

            RegValue = ReadByteTPI(TPI_SPEAKER_CFG);            // Arg[2]: 0xC7 (read Audio InfoFrame, Byte 4)
            putbyte(RegValue);

			RegValue = ReadByteTPI(TPI_AUDIO_HANDLING);					// Arg[3]: 0x25
			putbyte(RegValue & TWO_LSBITS);

#ifndef F_9022A_9334
			RegValue = ReadIndexedRegister(0x02, AUDIO_INPUT_LENGTH);	// Arg[4]:
			putbyte(RegValue & LOW_NIBBLE);
#else
			putbyte(0x00);												// dummy value
#endif

            RegValue = ReadByteTPI(TPI_CODING_TYPE_CHANNEL_COUNT);         // Arg[5]: 0xC4
            putbyte(RegValue & THREE_LSBITS);

			// Arg[6] - Checksum
			putbyte(g_audio_Checksum);
			break;
		
		case READ_I2S_CONFIG:
			putbyte(OpCode);
			putbyte(I2S_INPUT_CFG_CMD_LEN);

			RegValue = ReadByteTPI(TPI_I2S_IN_CFG);	// Arg[0]:0x20
			putbyte(RegValue);
			break;

		case READ_I2S_STREAM_HEADER:
			putbyte(OpCode);
			putbyte(I2S_STREAM_HDR_CMD_LEN);

			for (i = 0; i < I2S_STREAM_HDR_CMD_LEN; i++)
			{
				RegValue = ReadByteTPI(TPI_I2S_CHST_0 + i);
				putbyte(RegValue);
			}
			break;

		case READ_AVI_INFOFRAME_DATA:
			putbyte(OpCode);
			putbyte(SIZE_AVI_INFOFRAME);

			ReadBlockTPI(TPI_AVI_BYTE_0, SIZE_AVI_INFOFRAME, DataAvi);

			for (i=0; i < SIZE_AVI_INFOFRAME; i++)
			{
				putbyte(DataAvi[i]);
			}
			break;

		case READ_AUDIO_INFOFRAME_DATA:
			putbyte(OpCode);
			putbyte(SIZE_AUDIO_INFOFRAME);

			ReadBlockTPI(TPI_AUDIO_BYTE_0, SIZE_AUDIO_INFOFRAME, DataMisc);

			for (i=0; i < SIZE_AUDIO_INFOFRAME; i++)
			{
				putbyte(DataMisc[i]);
			}
			break;

		case READ_INFOFRAME_DATA:
			// A little Sanity check
			if (	Arg(0) == AVI
					||  Arg(0) == SPD
					||  Arg(0) == AUDIO
					||  Arg(0) == MPEG
					||  Arg(0) == GEN_1
					||  Arg(0) == GEN_2
					||  Arg(0) == HDMI_VISF
					||  Arg(0) == GBD )
			{			
				putbyte(OpCode);

				if (Arg(0) == AVI)
				{
					len = SIZE_AVI_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , AVI);
					putbyte(len);
					I2C_ReadBlock(TPI_BASE_ADDR, AVI_INFOFRM_OFFSET, &Arg(1), len);
				}
				else if (Arg(0) == SPD)
				{
					len = SIZE_SPD_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , SPD);
				}
				else if (Arg(0) == AUDIO)
				{
					len = SIZE_AUDIO_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , AUDIO);
    			}
				else if (Arg(0) == MPEG)
				{
 #ifdef F_9136 
					len = SIZE_MPEG_INFOFRAME+1;
 #else
					len = SIZE_MPRG_HDMI_INFOFRAME+1;
 #endif
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , MPEG);
			    }
				else if (Arg(0) == GEN_1)
				{
					len = SIZE_GEN_1_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , GEN_1);
			    }
				else if (Arg(0) == GEN_2)
				{
					len = SIZE_GEN_2_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , GEN_2);
			    }
				else if (Arg(0) == HDMI_VISF)
				{
					len = SIZE_HDMI_VISF_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , HDMI_VISF);
			    }
				else if (Arg(0) == GBD)
				{
					len = SIZE_GBD_INFOFRAME+1;
					I2C_WriteByte(TPI_BASE_ADDR, TPI_INFOFRAME_ACCESS_REG , GBD);
			    }
				if (Arg(0) != AVI)
				{
					putbyte(len);
					I2C_ReadBlock(TPI_BASE_ADDR, OTHER_INFOFRM_OFFSET, &Arg(1), len);
				}
				putbyte (Arg(0));
				for (i = 1; i < len; i++)
 				{
					putbyte(Arg(i));
				}
			}
			else
			{
				printf(" ****** Invalid InfoFrame Argument encountered ********\n");
			}
			break;

 #ifdef DEV_SUPPORT_HDCP
		case READ_HDCP_OVERRIDE_MODE:
 			putbyte(OpCode);
			putbyte(HDCP_OVERRIDE_MODE_CMD_LEN);
			
			putbyte( ReadByteTPI (TPI_HDCP_CONTROL_DATA_REG) );
		break;
#endif

#ifdef DEV_SUPPORT_EHDMI
		case READ_EHDMI_MODE_CONFIG:
			putbyte(OpCode);
			putbyte(EHDMI_MODE_CMD_LEN);

			putbyte(EHDMI_MODE);
			break;
#endif

		case READ_REGISTER_BLOCK:
			putbyte(OpCode);

			putbyte(Arg(2));	// Send command length

			I2C_ReadBlock(Arg(0),  Arg(1), & Arg(3), (word) Arg(2));

			for (i = 0; i < Arg(2); i++)
			{
				putbyte(Arg(i + 3));
			}
			break;

		case READ_PRODUCT_CODE:
			putbyte(OpCode);
			putbyte(PRODUCT_CODE_CMD_LEN);

			putbyte(TPI_PRODUCT_CODE);

			break;

		case READ_FIRMWARE_VERSION:
			putbyte(OpCode);
			putbyte(TPI_FW_VERSION_CMD_LEN);

			for (i = 0; i < TPI_FW_VERSION_CMD_LEN; i++)
			{
				putbyte(TPI_FW_VERSION[i]);
			}

			break;

		case READ_CONTROL_COMMAND_VERSION:
			putbyte(OpCode);
			putbyte(CTRL_CMD_VERSION_CMD_LEN);
			putbyte(CONTROL_COMMAND_VERSION);
			break;

		default:
			putbyte(UNKNOWN_COMMAND);
			break;
		}
}


///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   SIO_InterruptHandler ()
//
// PURPOSE          :   Interrupt Service Routine for the simple serial port
// 						of the 89C51 processor.
// 						
// DESIGN			:	The firmware ignores incoming serial unless it is part
// 						of a message as defined in the
// 						FirmwareCommunicationsPacket document.  
//
// 						This has two states: receiving a message - the incomming
// 						byte is saved in the InCharBuf buffer; or not receiving a
// 						message - the byte goes in the bit bucket.
//
// 						The state is changed to receiving when a 0xFF or 0xFE
// 						byte is received AND we are not currently in receiving
// 						state.
//
// 						The state changes back to non-receiving when:
// 						1) We have recived to length of the message
// 						2) We timeout since the start of the receiving state.
//
// CALLED BY        :	N/A  
//
// INPUT PARAMS     :	None 
//
// OUTPUT PARAMS    :	InCharBuff[] will contain the message MINUS the START OF
// 						PACKET byte.
//
// GLOBALS USED     :   
//
// RETURNED VALUE   :   
//
//////////////////////////////////////////////////////////////////////////////

void SIO_InterruptHandler(void) interrupt 4
{

	// SCON0.0, receive interrupt flag bit.  Did we get a serial receive interrupt?
	if (RI)
	{
		// Are we waiting for a new packet start?  If so, then check to see if
		// the received byte is a start of packet.
		if (!F_CollectingData && !F_SBUF_DataReady)
		{
			// Read the next byte from the Serial Register
			MessageType = SBUF;

			// waiting for a new packet
			if ((MessageType == START_OF_PACKET) || (MessageType == START_OF_FW_PACKET))
			{
				// a new packet arrives. Discard this byte
				// now collecting incoming bytes
				F_CollectingData = TRUE;

#ifdef HALTIMER
				// sets Serial timer's granularity and kicks the timer off to free running    
    			        HalTimerSet(TIMER_SERIAL, 2000);	//2 Sec for a message to complete
#else
				// Move to using timer after porting of 9234 code complete.
#endif
			}
		}

		// We are already in the middle of receiving a message packet.  Let's
		// get the next byte.  Note: At this point we are in binary mode, so we
		// cannot test for a new "start", only continue to read the byte until
		// the read pointer reaches the calculated end OR we time-out
		else
		{
			// Special case: According to the protocol, byte 3 will contain the
			// count of bytes REMAINING in this message.
			if (IDX_InChar == 1)
			{
				NumOfArgs = SBUF;
			}

			InCharBuff[IDX_InChar++] = SBUF;		// Save incoming byte

			// Have we reached the end OR timed-out?
#ifdef HALTIMER
			if ((IDX_InChar == (NumOfArgs + 2)) || (HalTimerExpired(TIMER_SERIAL)))
#else
			// Move to using timer after porting of 9234 code complete
			if (IDX_InChar == (NumOfArgs + 2))
#endif

			{
				// Yes - reset for next packet
				F_SBUF_DataReady = TRUE;
				F_CollectingData = FALSE;
				IDX_InChar = 0;
			}
		}

		RI = 0;										// clear receive interrupt flag
	}

	// SCON0.1. Prev. byte transmission completed bit
    if (TI)
    {
        TXBusy = 0;
        TI = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION         :   putchar() & putbyte()
//
// PURPOSE          :   Serial port byte output functions
// 						
// DESIGN			:	
//
// CALLED BY        :	Serial and Event code
//
// INPUT PARAMS     :	char or byte to output
//
// OUTPUT PARAMS    :	
//
// GLOBALS USED     :   
//
// RETURNED VALUE   :   Contents of the Serial Register
//
//////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------
char putchar (char c)
{
    if (c == '\n')
    {
        while ( TXBusy )
        {
            ;           //wait for buffer free
        }
        TXBusy = 1;
        /*SBUF = 0x0d;*/    // output CR before LF
				/*HAL_UART_Transmit_IT(&huart3, (uint8_t *)0x0d, 1);
    }

    while ( TXBusy )
    {
        ;               //wait for buffer free
    }
    TXBusy = 1;
		/*SBUF = c;*/
    /*HAL_UART_Transmit_IT(&huart3, (uint8_t *)c, 1);

    return (char) c;
}


byte putbyte (byte b)
{
	/*while (TXBusy) { ; }	//wait for buffer free

	TXBusy = 1;
	SBUF = b;*/
	/*HAL_UART_Transmit_IT(&huart3, (uint8_t *)b, 1);

	return b;
}*/

