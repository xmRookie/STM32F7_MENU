/***********************************************************************************/
/*  Copyright (c) 2002-2009, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

#include <stdio.h>

#include "defs.h"
#include "TypeDefs.h"
//#include "gpio.h" //这里使用的是51的GPIO 移植后部分的GPIO功能移动到了app 中的gpiosetting.c中完成
#include "Initializations.h"
#include "SerialPort.h"
#include "util.h"
#include "TPI.h"
#include "HDCP.h"
#include "i2c_master_sw.h"
#include "si_apiConfigure.h"
//#include "AMF_Lib.h" //网口相关
#include "constants.h"
#include "externals.h"

#ifdef IS_EVENT
#include "Event.h"
#include "SimConfig.h"
#endif

#ifdef F_9334   //for Evita only features, such as CDC 开发板特有功能？
extern	void HalTimerInit(void);
#endif

/*#ifdef RX_SAMABETH_ONBOARD
extern void InitSamabeth(void);  //Samabeth 是对板子上的器件进行配置的代码
#endif*/

/*static void ServiceRequest (void);*/

void sii9136_3_main(void)
{

#ifdef RX_ONBOARD //定义了没有开启
	SYSTEM_STATE state;
	word R_Monitor;
#endif
#ifdef SYS_BOARD_FPGA
	word i;
#endif
	word T_Monitor;

	/*InitMCU();*/					// Initialize host microcontroller

	printf("\n\nCopyright Silicon Image\n");
	/*printf("Built: %s-%s\n", __DATE2__, __TIME__);*/

#ifdef F_9334
	HalTimerInit();				//Initialize Timer/Counters
#endif

	g_EDID_Simulation = FALSE;
	g_Video_Mode_Simulation = FALSE;	

#ifdef SYS_BOARD_FPGA
	printf("\nWaiting for FPGA init.");//等待FPGA初始化
	for (i=0; i < 6; i++) {
		DelayMS(250);
		printf(".");
		DelayMS(250);
		printf(".");
		DelayMS(250);
		printf(".");
	 	}
	printf("\n\n");
#endif

	/*if (!PinRequest)
	{
		ServiceRequest();
	}*/

	TPI_Init();

#ifdef SYS_BOARD_FPGA
	// Ekki FPGA Init routine
	Init_Ekki_FPGA();
#endif

	IDX_InChar = 0;
	NumOfArgs = 0;
	F_SBUF_DataReady = FALSE;
	F_CollectingData = FALSE;

#ifdef RX_ONBOARD
	state = SS_RX_NOT_READY;
	USRX_OutputChange = TRUE;

	#ifdef DEV_EMBEDDED
	// Works with 9223 and 9135
	I2C_WriteByte(0x60, 0xB5, 0x00);
	I2C_WriteByte(0x60, 0x4A, 0x80);
	#endif

	AMF_Init(!DEBUG);

#ifdef DEV_ACNU_BOARD
	I2C_WriteByte(0xE0, 0xFA, 0x02); // SCDT LED Programming
	I2C_WriteByte(0xE0, 0xFC, 0x02);
	I2C_WriteByte(0x60, 0x08, (I2C_ReadByte(0x60, 0x08) | 0x02)); // OCKINV
	I2C_WriteByte(0xE0, 0x01, 0x00);
#endif
	R_Monitor = R_INIT_PERIOD;    // AMF (RX) polling timer interval
#endif

#ifdef RX_SAMABETH_ONBOARD
	//InitSamabeth();
#endif

	T_Monitor = T_MONITORING_PERIOD;    // TPI (TX) polling timer interval

#ifdef IS_EVENT
	EventInit();
#ifdef IS_SIMCONFIG
	SimConfInit();
#endif
#endif

	while (1)
	{
/*#ifdef RX_ONBOARD
        if ((state > SS_FATAL_ERROR) && !R_Monitor)
        {
            if (AMF_Poll(!DEBUG) != 0)
            {
                state = SS_RX_NEW_VIDEO_MODE;

				AMF_GetVideoModeDescription(&VideoModeDescription);
				VideoModeDescription.HDCPAuthenticated = VMD_HDCP_AUTHENTICATED; // Klude for Bug#19532 - remove when fixed %%%

				printf("AMF sets video: V:%d", (int) VideoModeDescription.VIC);
				printf(" A:%x", (int) VideoModeDescription.AspectRatio);
				printf(" CS:%x", (int) VideoModeDescription.ColorSpace);
				printf(" CD:%x", (int) VideoModeDescription.ColorDepth);
				printf(" HA:%d", (int) VideoModeDescription.HDCPAuthenticated);
				printf(" HF:%d", (int) VideoModeDescription.HDMIVideoFormat);
				printf(" 3D:%d\n", (int) VideoModeDescription.ThreeDStructure);

   	            R_Monitor = R_MONITORING_PERIOD;
                T_Monitor = 0;                          // Go into T_Monitor right away
				switch(VideoModeDescription.VIC)
				{
					// These are the only video formats supported by Pebbles
					case 2:		// 480p
					case 4:		// 720p
					case 5:		// 1080i
					case 6:		// 480i
					case 16:	// 1080p
					case 17:	// 576p
					case 21:	// 576i
						break;

					default:
				 		state = SS_RX_NOT_READY;
						VideoModeDescription.VIC = 2; //480p
						VideoModeDescription.AspectRatio = VMD_ASPECT_RATIO_4x3;
						VideoModeDescription.ColorSpace = VMD_COLOR_SPACE_RGB;
						VideoModeDescription.ColorDepth = VMD_COLOR_DEPTH_8BIT;
						VideoModeDescription.HDCPAuthenticated = VMD_HDCP_NOT_AUTHENTICATED;
						VideoModeDescription.HDMIVideoFormat = VMD_HDMIFORMAT_CEA_VIC;
				 		printf("TPI Resolution Not Supported. Default to 480p\n");
				 		R_Monitor = R_INIT_PERIOD;
				}

				#ifdef DEV_EMBEDDED
				// Works with 9223 and 9135
				I2C_WriteByte(0x60, 0xB5, 0x00);
				I2C_WriteByte(0x60, 0x4A, 0x80);
				#endif
            }
        }
        else
        {
            R_Monitor--;
        }

        if ((state > SS_RX_NOT_READY) && !T_Monitor)
        {
            if (state == SS_RX_NEW_VIDEO_MODE)
            {
				printf("TPI Video Mode: %X\n", (int)VideoModeDescription.VIC);

				USRX_OutputChange = TRUE;
				TPI_Poll();

			    state = SS_TX_READY;
                T_Monitor = T_MONITORING_PERIOD;
            }
            else if (state == SS_TX_READY)
            {
                TPI_Poll();
                T_Monitor = T_MONITORING_PERIOD;
            }
        }
        else
        {
            T_Monitor--;
        }
#else // No RX Onboard*/

		// Setup for no RX on board
#ifndef RX_SAMABETH_ONBOARD
		VideoModeDescription.VIC = 2; //480p
		VideoModeDescription.AspectRatio = VMD_ASPECT_RATIO_4x3;
		VideoModeDescription.ColorSpace = VMD_COLOR_SPACE_RGB;
		VideoModeDescription.ColorDepth = VMD_COLOR_DEPTH_8BIT;
		VideoModeDescription.HDCPAuthenticated = VMD_HDCP_NOT_AUTHENTICATED;
		VideoModeDescription.HDMIVideoFormat = VMD_HDMIFORMAT_CEA_VIC;
#else
		VideoModeDescription.VIC = 16; //1080p
		VideoModeDescription.AspectRatio = VMD_ASPECT_RATIO_16x9;
		VideoModeDescription.ColorSpace = VMD_COLOR_SPACE_RGB;
		VideoModeDescription.ColorDepth = VMD_COLOR_DEPTH_8BIT;
		VideoModeDescription.HDCPAuthenticated = VMD_HDCP_AUTHENTICATED;
		VideoModeDescription.HDMIVideoFormat = VMD_HDMIFORMAT_CEA_VIC;
#endif

		if (!T_Monitor)
        {
            TPI_Poll();
            T_Monitor = T_MONITORING_PERIOD;
        }
        else
		{
            T_Monitor--;
		}
/*#endif

        if (!PinRequest)
        {
			ServiceRequest();
        }*/

        // Communicate with Host via serial port
        /*SerComm();*/
				// sercomm()是和串口相关的函数在serialport.c中
#ifdef IS_EVENT
		EventTask();
#ifdef IS_SIMCONFIG
		SimConfTask();
#endif  // IS_SIMCONF
#endif  // IS_EVENT
    }
}


/*static void ServiceRequest (void)
{
	PinGrant = 0;
	printf ("GNT\n");

	while (!PinRequest)
	{

		// Calling SerComm() is needed to process packets while in COMM mode.
		// However, this could be problematic as routines to handle packets may result in I2C traffic.
        SerComm();

		// uC must wait until released by SiIMon.
	}

	PinGrant = 1;
	printf ("!GNT\n");
}*/

