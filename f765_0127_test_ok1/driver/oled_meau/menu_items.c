#include "common.h"
//#include <stdbool.h>
#include <stdio.h>
//#include<windows.h>
#include<string.h>
//#include "lcd.h"
#include <math.h>


#define TRUE 1
#define FALSE 0

#define MENU_Y  3

#define UTC_OFFSET_NUM 8
//ip addr
#define STM32_IP_0  0xc0   //192  0xc0 
#define STM32_IP_1  0xa8   //168  0xa8  
#define STM32_IP_2  0x00   // 0   0x00
#define STM32_IP_3  0xbc    //188 0xbc
//mac addr
#define STM32_MAC_B0  0x00
#define STM32_MAC_B1  0x80
#define STM32_MAC_B2  0xE1
#define STM32_MAC_B3  0x00
#define STM32_MAC_B4  0x00
#define STM32_MAC_B5  0x00








       



unsigned char ip_char[4];//={0};                  
unsigned char mac_char[3];//={0};    
unsigned char utc_offset_char[3];

MENU_STATUS System_Feature_Arr[En_Or_Dis_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET};
MENU_STATUS Config_Format_Arr[CONFIG_FORMAT_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET,
																										 MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, 
																										 MENU_RESET};
MENU_STATUS Config_Video_Input_Arr[InputNum] = {MENU_SET, MENU_RESET, MENU_RESET};
MENU_STATUS Config_Video_Output_Arr[OutputNum] = {MENU_SET, MENU_RESET};
unsigned char Ip_Arr[IP_NUM] = {STM32_IP_0,STM32_IP_1,STM32_IP_2,STM32_IP_3}; 
unsigned char Mac_Arr[MAC_ARR_NUM] = {STM32_MAC_B0,STM32_MAC_B1,STM32_MAC_B2,STM32_MAC_B3,STM32_MAC_B4,STM32_MAC_B5};  
 char utc_offset_num = UTC_OFFSET_NUM;
MENU_STATUS Config_Src_Mode_Arr[SRC_MODE_NUM] = {MENU_SET, MENU_RESET};
MENU_STATUS Config_Src_Sync_Type_Arr[SRC_SYNV_TYPE_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET,
																																	 MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET};
MENU_STATUS Config_Src_Clock_Type_Arr[SRC_CLOCK_TYPE_NUM] = {MENU_SET, MENU_RESET, MENU_RESET};
MENU_STATUS Config_Src_Sync_Frame_Arr[SRC_SYNC_FRAME_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET};
MENU_STATUS Config_Display_Mode_Arr[DISPLAY_MODE_NUM] = {MENU_RESET, MENU_RESET, MENU_SET};
MENU_STATUS Config_Display_Brightness_Arr[DISPLAY_BRIGHTNESS_NUM] = {MENU_SET, MENU_RESET};

unsigned char submenu_number=0;


/*------------------------------------------Menu level 0-2------------------------------------------------------------*/

//level 0-2-4

static MENU_ITEM Status_Synch_menu[STATUS_SYNCH_MENU] = {
	{" Master_side >",				Status_Synch_PrintCallbackRender,							Status_Synch_PrintCallback,						0,				 NULL,  NONE},
	{" Slave_side  >",				Status_Synch_PrintCallbackRender,							Status_Synch_PrintCallback,						0,         NULL,  NONE},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
}; 



/*------------------------------------------Menu level 0-1------------------------------------------------------------*/
//level 0-1-0

static MENU_ITEM Config_Format_menu[CONFIG_FORMAT_MENU] = {
	{"    Auto      ",			    Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,							 NULL,  MENU_SET},
	{"  720p50HZ    ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  720p59HZ    ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  720p60HZ    ",		    	Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080i50HZ   ",	    		Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080i59HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,							 NULL,  MENU_RESET},
	{"  1080i60HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p23HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p24HZ   ",		  	  Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p25HZ   ",		   		Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p29HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,				 			 NULL,  MENU_RESET},
	{"  1080p30HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p50HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p59HZ   ",		  	  Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"  1080p60HZ   ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,				 			 NULL,  MENU_RESET},
	{" 1080pSF23HZ  ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{" 1080pSF24HZ  ",					Config_Format_ActionCallbackRender,							Config_Format_ActionCallback,						0,               NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
};
//level 0-1-1
static MENU_ITEM Config_Video_Output_menu[CONFIG_VIDEO_OUTPUT_MENU] = {
	{"    HDMI      ",			Config_Video_Output_ActionCallbackRender,							Config_Video_Output_ActionCallback,						0,				 				NULL,  MENU_SET},
	{"    HDSDI     ",			Config_Video_Output_ActionCallbackRender,							Config_Video_Output_ActionCallback,						0,                NULL,  MENU_RESET},
	{"<   Exit      ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
}; 

static MENU_ITEM Config_Video_Input_menu[CONFIG_VIDEO_INPUT_MENU] = {
	{"     HDMI     ",				Config_Video_Input_ActionCallbackRender,							Config_Video_Input_ActionCallback,						0,							 NULL,  MENU_SET},
	{"     HDSDI    ",				Config_Video_Input_ActionCallbackRender,							Config_Video_Input_ActionCallback,						0,               NULL,  MENU_RESET},
	{"   TEST_PAT   ",				Config_Video_Input_ActionCallbackRender,							Config_Video_Input_ActionCallback,						0,               NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
};

static MENU_ITEM Config_Video_menu[CONFIG_VIDEO_MENU] = {
	{"    Input    >",			NULL,							NULL,						CONFIG_VIDEO_INPUT_MENU,								Config_Video_Input_menu,   NONE},
	{"    Output   >",			NULL,							NULL,						CONFIG_VIDEO_OUTPUT_MENU,               Config_Video_Output_menu,  NONE},
	{"<   Exit      ",   		Exit_ActionCallbackRender,      Exit_ActionCallback,                     0,                    NULL    ,  NONE      }	,
	
};
//level 0-1-2

static MENU_ITEM Config_Audio_menu[CONFIG_AUDIO_MENU] = {
	{"    Input    >",			NULL,							NULL,						0,				 				NULL,  NONE},
	{"    Output   >",			NULL,							NULL,						0,                NULL,  NONE},
	{"<   Exit      ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
};
//level 0-1-3

static MENU_ITEM Config_Network_IP_menu[CONFIG_NETWORK_IP_MENU] = {
	{"192      ",		    	Config_Network_IP_ActionCallbackRender,							Config_Network_IP_ActionCallback,						0,				 			NULL,  NONE},
	{"168      ",					Config_Network_IP_ActionCallbackRender,							Config_Network_IP_ActionCallback,						0,               NULL,  NONE},
	{"000      ",		    	Config_Network_IP_ActionCallbackRender,							Config_Network_IP_ActionCallback,						0,							 NULL,  NONE},
	{"190      ",					Config_Network_IP_ActionCallbackRender,							Config_Network_IP_ActionCallback,						0,               NULL,  NONE},
	{"<Exit    ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
};
static MENU_ITEM Config_Network_MAC_menu[CONFIG_NETWORK_MAC_MENU] = {
	{"00      ",		    	Config_Network_MAC_ActionCallbackRender,							Config_Network_MAC_ActionCallback,						0,							 NULL,  NONE},
	{"80      ",					Config_Network_MAC_ActionCallbackRender,							Config_Network_MAC_ActionCallback,						0,               NULL,  NONE},
	{"E1      ",		    	Config_Network_MAC_ActionCallbackRender,							Config_Network_MAC_ActionCallback,						0,							 NULL,  NONE},
	{"00      ",					Config_Network_MAC_ActionCallbackRender,							Config_Network_MAC_ActionCallback,						0,               NULL,  NONE},
	{"00      ",		    	Config_Network_MAC_ActionCallbackRender,							Config_Network_MAC_ActionCallback,						0,				 			 NULL,  NONE},
	{"00      ",					Config_Network_MAC_ActionCallbackRender,							Config_Network_MAC_ActionCallback,						0,               NULL,  NONE},
	{"<Exit   ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
}; 

static MENU_ITEM Config_Network_menu[CONFIG_NETWORK_MENU] = {
	{"     IP      >",		    	NULL,							NULL,						CONFIG_NETWORK_IP_MENU,				 					 Config_Network_IP_menu,   NONE},
	{"     MAC     >",					NULL,							NULL,						CONFIG_NETWORK_MAC_MENU,                 Config_Network_MAC_menu,  NONE},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
};


//level 0-1-4
/**
static MENU_ITEM Config_GPS_menu[CONFIG_GPS_MENU] = {
	{"UTC_offset:",			Config_GPS_ActionCallbackRender,							Config_GPS_ActionCallback,						0,				 NULL,  NONE},
};
*/


//level 0-1-5

static MENU_ITEM Config_SRC_Mode_menu[CONFIG_SRC_MODE_MENU] = {
	{"    Master    ",		    	Config_SRC_Mode_ActionCallbackRender,							Config_SRC_Mode_ActionCallback,						0,				 NULL,  MENU_SET},
	{"    Slave     ",					Config_SRC_Mode_ActionCallbackRender,							Config_SRC_Mode_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"<   Exit      ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
}; 
static MENU_ITEM Config_SRC_Sync_Type_menu[CONFIG_SRC_SYNC_TYPE_MENU] = {
	{"   Bot_Greg   ",		    	Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_SET},
	{"   Bot_Black  ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Bot_White  ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Top_Greg   ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Top_Black  ",		    	Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Top_White  ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{" Four_corners ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Obvious    ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Hidden_1   ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Hidden_2   ",		    	Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Hidden_3   ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Hidden_4   ",					Config_SRC_Sync_Type_ActionCallbackRender,							Config_SRC_Sync_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
}; 
static MENU_ITEM Config_SRC_Clock_Type_menu[CONFIG_SRC_CLOCK_TYPE_MENU] = {
	{"   Wrie_Mode  ",		    	Config_SRC_Clock_Type_ActionCallbackRender,							Config_SRC_Clock_Type_ActionCallback,						0,				 NULL,  MENU_SET},
	{"   OSC_Mode   ",					Config_SRC_Clock_Type_ActionCallbackRender,							Config_SRC_Clock_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   GPS_Mode   ",					Config_SRC_Clock_Type_ActionCallbackRender,							Config_SRC_Clock_Type_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
};
static MENU_ITEM Config_SRC_Sync_Frame_menu[CONFIG_SRC_SYNC_FRAME_MENU] = {
	{"  25_Frames   ",		    Config_SRC_Sync_Frame_ActionCallbackRender,							Config_SRC_Sync_Frame_ActionCallback,						0,				 NULL,  MENU_SET},
	{"  30_Frames   ",				Config_SRC_Sync_Frame_ActionCallbackRender,							Config_SRC_Sync_Frame_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"  50_Frames   ",				Config_SRC_Sync_Frame_ActionCallbackRender,							Config_SRC_Sync_Frame_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"  60_Frames   ",				Config_SRC_Sync_Frame_ActionCallbackRender,							Config_SRC_Sync_Frame_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"  100_Frames  ",		    Config_SRC_Sync_Frame_ActionCallbackRender,							Config_SRC_Sync_Frame_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"  120_Frames  ",				Config_SRC_Sync_Frame_ActionCallbackRender,							Config_SRC_Sync_Frame_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
	
}; 
static MENU_ITEM Config_SRC_menu[CONFIG_SRC_MENU] = {
	{"    Mode    >",		  NULL,							NULL,						CONFIG_SRC_MODE_MENU,							 Config_SRC_Mode_menu,		    NONE},
	{" Sync_Type  >",			NULL,							NULL,						CONFIG_SRC_SYNC_TYPE_MENU,				 Config_SRC_Sync_Type_menu,   NONE},
	{" Clock_Type >",			NULL,							NULL,						CONFIG_SRC_CLOCK_TYPE_MENU,				 Config_SRC_Clock_Type_menu,  NONE},
	{" Sync_Frame >",			NULL,							NULL,						CONFIG_SRC_SYNC_FRAME_MENU,				 Config_SRC_Sync_Frame_menu,  NONE},
	{"<   Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
};





static MENU_ITEM Config_Display_Mode_menu[CONFIG_DISPLAY_MODE_MENU] = {
	{"Video_Display ",		    		Config_Display_Mode_ActionCallbackRender,							Config_Display_Mode_ActionCallback,						0,				 NULL,  MENU_SET},
	{"Audio_Display ",						Config_Display_Mode_ActionCallbackRender,							Config_Display_Mode_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"   Detail     ",						Config_Display_Mode_ActionCallbackRender,							Config_Display_Mode_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
}; 


static MENU_ITEM Config_Display_Brightness_menu[CONFIG_DISPLAY_BRIGHTNESS_MENU] = {
	{"  High_Light  ",		    	Config_Display_Brightness_ActionCallbackRender,							Config_Display_Brightness_ActionCallback,						0,				 NULL,  MENU_SET},
	{"  Low_Light   ",					Config_Display_Brightness_ActionCallbackRender,							Config_Display_Brightness_ActionCallback,						0,				 NULL,  MENU_RESET},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }	
};
//level 0-1-6

static MENU_ITEM Config_Display_menu[CONFIG_DISPLAY_MENU] = {
	{"     Mode    >",		    	NULL,							NULL,						CONFIG_DISPLAY_MODE_MENU,													 Config_Display_Mode_menu, 				 NONE},
	{" Brightness  >",					NULL,							NULL,						CONFIG_DISPLAY_BRIGHTNESS_MENU,										 Config_Display_Brightness_menu, 	 NONE},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    },
	
}; 

/*------------------------------------------Menu level 0-0------------------------------------------------------------*/


//level 0-0-4

static MENU_ITEM System_Feature_menu[SYSTEM_FEATURE_MENU] = {
	{"     GPS      ",					NULL,            NULL,          0,               NULL,  MENU_SET},
	{"   OVEN_XO    ",			    NULL,            NULL,          0,               NULL,  MENU_RESET},
	{"     HDMI     ",			    NULL,						 NULL,					0,               NULL,  MENU_RESET},
	{"     HDSDI    ",          NULL,						 NULL,					0,               NULL,  MENU_RESET},
	{"   TEST_PAT   ",          NULL,            NULL,          0,               NULL,  NON_OPERATE},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
};

//-----------------------------------------------------------------------level 0-0

static MENU_ITEM System_menu[SYSTEM_MENU] = {
	{"   Version   >",    System_UniversalCallbackRender,						   System_UniversalCallback,					0,							 			 NULL,  								NONE     },
	{"   Restart    ",    System_UniversalCallbackRender,              System_UniversalCallback,          0,										 NULL,  								NONE},
	{"   Restore    ",    System_UniversalCallbackRender,              System_UniversalCallback,          0,										 NULL,  								NONE},
	{"   Update     ",    System_UniversalCallbackRender,              System_UniversalCallback,          0,										 NULL,  								NONE},
	{"   Feature   >",   	NULL,						   													 NULL,                              SYSTEM_FEATURE_MENU,	 System_Feature_menu,   NONE},
	{"<    Exit     ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
}; 
//-----------------------------------------------------------------------level 0-1

static MENU_ITEM Config_menu[CONFIG_MENU] = {
	{"   Format    >",				   NULL,						  					 				 NULL,				   										CONFIG_FORMAT_MENU,       Config_Format_menu,  					NONE},
	{"   Video     >",					 NULL,                         				 NULL,                   						CONFIG_VIDEO_MENU,				Config_Video_menu,  					NONE},
	{"   Audio     >",					 NULL,                         			   NULL,                   						CONFIG_AUDIO_MENU,				Config_Audio_menu,  					NONE},
	{"   Network   >",			     NULL,                        				 NULL,                  					  CONFIG_NETWORK_MENU,			Config_Network_menu,  				NONE},
	{"   GPS       >",				   Config_GPS_ActionCallbackRender,      Config_GPS_ActionCallback,         0,							   				NULL, 											  NONE},
	{"   SRC/SWK   >",					 NULL,                        				 NULL,                 					    CONFIG_SRC_MENU,					Config_SRC_menu,						  NONE},
	{"   Display   >",			     NULL,                         				 NULL,                   						CONFIG_DISPLAY_MENU,			Config_Display_menu,  				NONE},
	{"<  Exit       ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
}; 


static MENU_ITEM Status_menu[STATUS_MENU] = {
	{"Video_Format >",					 Status_PrintCallbackRender,            Status_PrintCallback,         0,						    				NULL,  							NONE    },
	{"Audio_Format >",					 Status_PrintCallbackRender,            Status_PrintCallback,         0,						  				  NULL,  							NONE},
	{"   Ethernet  >",					 Status_PrintCallbackRender,            Status_PrintCallback,         0,						    				NULL,  							NONE},
	{"   Error     >",					 Status_PrintCallbackRender,            Status_PrintCallback,         0,						   				  NULL,  							NONE},
	{"   Synch     >",					 NULL,																	NULL,                         STATUS_SYNCH_MENU,        Status_Synch_menu,  NONE},
	{"<  Exit       ",   			Exit_ActionCallbackRender,                                  Exit_ActionCallback,                     0,                 NULL      ,  NONE    }
}; 

/*
** HOME menu items definition          
*/
/*------------------------------------------ Menu  ------------------------------------------------------------*/
//level 0

static MENU_ITEM home_items[MENU_HOME_ITEMS] = {
	{"    System   >",    NULL,                           NULL,                     SYSTEM_MENU,                        System_menu,  NONE          },
	{"    Config   >",    NULL,                           NULL,                     CONFIG_MENU,                        Config_menu,  NONE          },    //menuUniversalCallbackRender
	{"    Status   >",    NULL,                           NULL,                     STATUS_MENU,                        Status_menu,  NONE         },
  {"<   Exit      ",    Exit2MainMenu_ActionCallbackRender,                           Exit2MainMenu_ActionCallback,                     0,                    NULL,  NONE          }	
};




















void Config_Video_Output_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	if(Config_Video_Output_menu[which].status != NON_OPERATE){
		OLED_Clear();
		for(i=0; i<OutputNum; i++){
			if(Config_Video_Output_menu[i].status != NON_OPERATE){
				if(i == which){
					Config_Video_Output_menu[i].status = MENU_SET;
				}else{
					Config_Video_Output_menu[i].status = MENU_RESET;
				}
			}
		}
		
		OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
		Arr_Update();
		////HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
		OLED_Clear();
		OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
	}
}
unsigned char Config_Video_Output_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	if(Config_Video_Output_menu[column].status != NON_OPERATE)
		OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}

	return FALSE;//false;
}

void Config_Video_Input_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	if(Config_Video_Input_menu[which].status != NON_OPERATE){
		OLED_Clear();
		for(i=0; i<InputNum; i++){
			if(Config_Video_Input_menu[i].status != NON_OPERATE){
				if(i == which){
					Config_Video_Input_menu[i].status = MENU_SET;
				}else{
					Config_Video_Input_menu[i].status = MENU_RESET;
				}
			}
		}
		OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
		Arr_Update();
		Set_FPGA_Reg_Config(InputNum_ADDR,0);
		////HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
		OLED_Clear();
		OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
	}
}
unsigned char Config_Video_Input_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	if(Config_Video_Input_menu[column].status != NON_OPERATE)
		OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}

	return FALSE;//false;
}

void Config_Network_MAC_ActionCallbackRender(unsigned char which)
{
	char *buf;
	OLED_Clear();
  buf = my_mac_int_to_char(Mac_Arr[which]);
	OLED_ShowString(0,MENU_Y,buf,12);
}
unsigned char Config_Network_MAC_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	static unsigned char temp=0;
	char *buf;
	OLED_Clear();
	if( temp != Mac_Arr[column]){
		if(once == 1 )
			temp=Mac_Arr[column];
	}
	switch(button->role){
		case MENU_UP:
			once=0;
			//return TRUE;//true;
			++temp;
			break;
		case MENU_DOWN:
			once=0;
			//return TRUE;//true;
			--temp;
			break;
		case MENU_CONFIRM:
			if(once == 0){
				Mac_Arr[column] = temp;
				OLED_ShowString(0,MENU_Y,"Save To Flash...",12);
				Arr_Update();
				//HAL_Delay(1000);
				OLED_Clear();
				ip_mac_char();
				OLED_ShowString(0,MENU_Y,"  Save To Flash",12);
				OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
				//HAL_Delay(1000);
				once=1;
			}
			return TRUE;//true;
			break;
		case MENU_CANCEL:
			once=0;
			temp = Mac_Arr[column];
			return TRUE;//true;
		default:
			temp = Mac_Arr[column];
			break;
	}
	buf = my_mac_int_to_char(temp);
	OLED_ShowString(0,MENU_Y,buf,12);
	return FALSE;//false;
}

void Config_Network_IP_ActionCallbackRender(unsigned char which)
{
	char *buf;
	OLED_Clear();
	buf = ip_int_to_char(Ip_Arr[which]);
	OLED_ShowString(0,MENU_Y,buf,12);
}
unsigned char Config_Network_IP_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	static unsigned char temp=0;
	char *buf;
	OLED_Clear();
	if( temp != Ip_Arr[column]){
		if(once == 1 )
			temp=Ip_Arr[column];
	}
	switch(button->role){
		case MENU_UP:
			once=0;
			//return TRUE;//true;
			++temp;
			break;
		case MENU_DOWN:
			once=0;
			//return TRUE;//true;
			--temp;
			break;
		case MENU_CONFIRM:
			if(once == 0){
				printf("MENU_CONFIRM temp %d ",temp);
				Ip_Arr[column] = temp;
				OLED_ShowString(0,MENU_Y,"Save To Flash...",12);
				Arr_Update();
				//HAL_Delay(1000);
				OLED_Clear();
				OLED_ShowString(0,MENU_Y,"  Save To Flash",12);
				OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
				ip_mac_char();
				//HAL_Delay(1000);
				once=1;
			}
			return TRUE;//true;
			break;
		case MENU_CANCEL:
			once=0;
			temp = Ip_Arr[column];
			return TRUE;//true;
		default:
			temp = Ip_Arr[column];
			break;
	}
	buf = ip_int_to_char(temp);
	OLED_ShowString(0,MENU_Y,buf,12);
	return FALSE;//false;
}


void Config_GPS_ActionCallbackRender(unsigned char which)
{
	char *buf;
	int8_t temp;
	OLED_Clear();
	OLED_ShowString(0,MENU_Y,"UTC_offset:",12);
	temp = utc_offset_num;
	if(temp >= 0){
		buf = ip_int_to_char(temp);
		buf[0] = '+';
	}else{
		temp = abs(temp);
		buf = ip_int_to_char(temp);
		buf[0] = '-';
	}
	
	OLED_ShowString(96,MENU_Y,buf,12);
}
unsigned char Config_GPS_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	static  int8_t temp=0;
	int8_t temp1;
	char *buf;
	OLED_Clear();
	if( temp != utc_offset_num){
		if(once == 1 )
			temp=utc_offset_num;
	}
	switch(button->role){
		case MENU_UP:
			once=0;
			//return TRUE;//true;
			++temp;
			break;
		case MENU_DOWN:
			once=0;
			//return TRUE;//true;
			--temp;
			break;
		case MENU_CONFIRM:
			if(once == 0){
				utc_offset_num = temp;
				OLED_ShowString(0,MENU_Y,"Save To Flash...",12);
				Arr_Update();
				//HAL_Delay(1000);
				OLED_Clear();
				OLED_ShowString(0,MENU_Y,"  Save To Flash",12);
				OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
				//HAL_Delay(1000);
				once=1;
			}
			return TRUE;//true;
			break;
		case MENU_CANCEL:
			once=0;
			temp = utc_offset_num;
			return TRUE;//true;
		default:
			temp = utc_offset_num;
			break;
	}
	OLED_ShowString(0,MENU_Y,"UTC_offset:",12);
	if(temp >= 0){
		buf = ip_int_to_char(temp);
		buf[0] = '+';
	}else{
		temp1 = temp;
		temp1 = abs(temp1);
		buf = ip_int_to_char(temp1);
		buf[0] = '-';
	}
	OLED_ShowString(96,MENU_Y,buf,12);
	return FALSE;//false;
}


void Config_SRC_Mode_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<SRC_MODE_NUM; i++){
		if(i == which){
			Config_SRC_Mode_menu[i].status = MENU_SET;
		}else{
			Config_SRC_Mode_menu[i].status = MENU_RESET;
		}
	}
	
	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	Set_FPGA_Reg_Config(SRC_MODE_NUM_ADDR,0);
	//HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}
unsigned char Config_SRC_Mode_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}

	return FALSE;//false;
}



void Config_SRC_Sync_Type_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<SRC_SYNV_TYPE_NUM; i++){
		if(i == which){
			Config_SRC_Sync_Type_menu[i].status = MENU_SET;
		}else{
			Config_SRC_Sync_Type_menu[i].status = MENU_RESET;
		}
	}

	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	Set_FPGA_Reg_Config(SRC_SYNV_TYPE_NUM_ADDR,0);
	//HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}
unsigned char Config_SRC_Sync_Type_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}
	return FALSE;//false;
}



void Config_SRC_Clock_Type_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<SRC_CLOCK_TYPE_NUM; i++){  
		if(i == which){
			Config_SRC_Clock_Type_menu[i].status = MENU_SET;  
		}else{
			Config_SRC_Clock_Type_menu[i].status = MENU_RESET;
		}
	}
	
	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	Set_FPGA_Reg_Config(SRC_CLOCK_TYPE_NUM_ADDR,0);
	//HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}
unsigned char Config_SRC_Clock_Type_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}
	return FALSE;//false;
}





void Config_SRC_Sync_Frame_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<SRC_SYNC_FRAME_NUM; i++){
		if(i == which){
			Config_SRC_Sync_Frame_menu[i].status = MENU_SET;
		}else{
			Config_SRC_Sync_Frame_menu[i].status = MENU_RESET;
		}
	}

	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	Set_FPGA_Reg_Config(SRC_SYNC_FRAME_NUM_ADDR,0);
	//HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}
unsigned char Config_SRC_Sync_Frame_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	char buf[80];
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}
	return FALSE;//false;
}

void Config_Format_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<CONFIG_FORMAT_NUM; i++){
		if(i == which){
			Config_Format_menu[i].status = MENU_SET;
		}else{
			Config_Format_menu[i].status = MENU_RESET;
		}
	}

	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	Set_FPGA_Reg_Config(CONFIG_FORMAT_NUM_ADDR,0);
	////HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}
unsigned char Config_Format_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	char buf[80];
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}
	return FALSE;//false;
}
void Status_Synch_PrintCallbackRender(unsigned char which)
{
	char buf[80];
	OLED_Clear();
	switch(which){
	case 0: 
		OLED_ShowString(0,MENU_Y,"Lock/No_Lock",12);
		break;
	case 1:  
		OLED_ShowString(0,MENU_Y,"Lock_No_Lock",12);
		break;
	}
}
unsigned char Status_Synch_PrintCallback(MENU_BUTTON *button, unsigned char column)
{
	switch(button->role){
		case MENU_UP:
			break;
		case MENU_DOWN:
			break;
		case MENU_CONFIRM:
			return TRUE;//true;
		case MENU_CANCEL:
			return TRUE;//true;
	}
	Status_Synch_PrintCallbackRender(column);
	return FALSE;//false;
}
void Config_Display_Mode_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<DISPLAY_MODE_NUM; i++){
		if(i == which){
			Config_Display_Mode_menu[i].status = MENU_SET;
		}else{
			Config_Display_Mode_menu[i].status = MENU_RESET;
		}
	}

	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	////HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}
unsigned char Config_Display_Mode_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}
	return FALSE;//false;
}
void Config_Display_Brightness_ActionCallbackRender(unsigned char which)
{
	uint8_t i;
	OLED_Clear();
	for(i=0; i<DISPLAY_BRIGHTNESS_NUM; i++){
		if(i == which){
			Config_Display_Brightness_menu[i].status = MENU_SET;
		}else{
			Config_Display_Brightness_menu[i].status = MENU_RESET;
		}
	}
	OLED_ShowString(0,MENU_Y,"TX Data To FPGA",12);
	Arr_Update();
	////HAL_Delay(1000);  //延迟 这里可以判断spi是否发送完毕
	if(Config_Display_Brightness_menu[0].status == MENU_SET){
	  OLED_WR_Byte(0x81,OLED_CMD); //--set contrast control register
		OLED_WR_Byte(0xff,OLED_CMD); // Set SEG Output Current Brightness 
	}else{
		OLED_WR_Byte(0x81,OLED_CMD); //--set contrast control register
		OLED_WR_Byte(0x0f,OLED_CMD); // Set SEG Output Current Brightness 
	}
	OLED_Clear();
	OLED_ShowString(0,MENU_Y+1,"The key to exit",12);
}

unsigned char Config_Display_Brightness_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	char buf[80];
	OLED_Clear();
	switch(button->role){
		case MENU_UP:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_DOWN:
			once=0;
			return TRUE;//true;
			//break;
		case MENU_CONFIRM:
			if(once == 0){
				once=1;
			}
			return TRUE;//true;
			//break;
		case MENU_CANCEL:
			once=0;
			return TRUE;//true;
	}
	return FALSE;//false;
}
void System_UniversalCallbackRender(unsigned char which)
{
	char buf[60];

	OLED_Clear();
	switch(which){
		case 0: 
				Display_Version();						
			break;
		case 1: 
				Restart_Action();
			break;
		case 2: 
				Restore_Action();
			break;
		case 3: 
				Update_Action();
			break;
	}
}
unsigned char System_UniversalCallback(MENU_BUTTON *button, unsigned char column)
{
	switch(button->role){
		case MENU_UP:
			//break;
			return TRUE;
		case MENU_DOWN:
			//break;
			return TRUE;
		case MENU_CONFIRM:
			return TRUE;
			//break;
		case MENU_CANCEL:
			return TRUE;//true;
	}

	System_UniversalCallbackRender(column);
	return FALSE;//false;
}
void Status_PrintCallbackRender(unsigned char which)
{
	char buf[80];
//	lcd_clrscr();
	OLED_Clear();
	switch(which){
	case 0: 
		Display_Video_Format();
		break;
	case 1:  
		Display_Audio_Format();
		break;
	case 2:  
		Display_Ethernet_Status();
		break;
	case 3:  
		Display_Error_Status();
		break;
	}
}
unsigned char Status_PrintCallback(MENU_BUTTON *button, unsigned char column)
{
	switch(button->role){
		case MENU_UP:
			//break;
			return TRUE;//true;
		case MENU_DOWN:
			//break;
			return TRUE;//true;
		case MENU_CONFIRM:
			return TRUE;//true;
		case MENU_CANCEL:
			return TRUE;//true;
	}
	Status_PrintCallbackRender(column);
	return FALSE;//false;
}

void Exit_ActionCallbackRender(unsigned char which)
{

	SetWorkMode(MODE_KEY);
	PushEvt_EvtPar(evKeyCancal,MODE_KEY);

}

unsigned char Exit_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	char buf[80];
	//system("cls");
	//OLED_Clear();
	Set_Exit();
	Set_button_num('d' );
	
	switch(button->role){
		case MENU_UP:
			printf(" MENU_UP\r\n");
			once=0;
			//return TRUE;//true;
			break;
		case MENU_DOWN:
			printf(" MENU_DOWN\r\n");
			once=0;
			//return TRUE;//true;
			break;
		case MENU_CONFIRM:
			printf(" MENU_CONFIRM\r\n");
			//return TRUE;//true;
			break;
		case MENU_CANCEL:
			printf(" MENU_CANCEL\r\n");
		//printf("Exit_ActionCallback Set_button_num('d' );\r\n");
			Set_button_num('d' );
			//menuPollButtons();
			scrollMenu(button); 
			Set_Display_Once_Flag(0);
			once=0;
			return TRUE;//true;
	}
//	lcd_clrscr();
	
	return FALSE;//false;
}



void Exit2MainMenu_ActionCallbackRender(unsigned char which)
{
	Set_cnt_down(0);
	SetWorkMode(MODE_KEY);
	PushEvt_EvtPar(evKeyCancal,MODE_KEY);

}

unsigned char Exit2MainMenu_ActionCallback(MENU_BUTTON *button, unsigned char column)
{
	static unsigned char once=1;
	char buf[80];
	//system("cls");
	//OLED_Clear();
	Set_Exit();
	Set_button_num('d' );
	
	switch(button->role){
		case MENU_UP:
			printf(" MENU_UP\r\n");
			once=0;
			//return TRUE;//true;
			break;
		case MENU_DOWN:
			printf(" MENU_DOWN\r\n");
			once=0;
			//return TRUE;//true;
			break;
		case MENU_CONFIRM:
			printf(" MENU_CONFIRM\r\n");
			//return TRUE;//true;
			break;
		case MENU_CANCEL:
			printf(" MENU_CANCEL\r\n");
		//printf("Exit_ActionCallback Set_button_num('d' );\r\n");
			Set_button_num('d' );
			//menuPollButtons();
			scrollMenu(button); 
			Set_Display_Once_Flag(0);
			once=0;
			return TRUE;//true;
	}
//	lcd_clrscr();
	
	return FALSE;//false;
}







char * ip_int_to_char(unsigned char ip_num)  //??intDí×a3échar
{
	unsigned char first_char,second_char,third_char;
	
	first_char = ip_num/100 + 0x30;
	second_char = (ip_num%100)/10 + 0x30;
	third_char = ip_num%10 + 0x30;
	
	ip_char[0]=(unsigned char)first_char;
	ip_char[1]=(unsigned char)second_char;
	ip_char[2]=(unsigned char)third_char;
	
	return ip_char;
}

char* my_mac_int_to_char(unsigned char mac_num)  //??intDí×a3échar
{
	unsigned char temp=0;
	
	temp = mac_num / 16 + 0x30;
	if(temp > 0x39)                       // ×?·?9  μ?  ×?·?A   μ?ascii2??μ
		temp += 7;
	mac_char[0] = temp;
	
	temp = mac_num % 16 + 0x30;
	if(temp > 0x39)
		temp += 7;
	mac_char[1] = temp;
	
	
	return mac_char;

	
}

void ip_mac_char(void)
{
	char *temp_str=NULL;
	int i=0; 
	for(i=0; i<4; i++){
		temp_str = ip_int_to_char(Ip_Arr[i]);
		memcpy(Config_Network_IP_menu[i].title ,temp_str,sizeof(ip_char));
	}
	for(i=0; i<6; i++){
		temp_str = my_mac_int_to_char(Mac_Arr[i]);
		memcpy(Config_Network_MAC_menu[i].title ,temp_str,sizeof(mac_char));
	}
}

extern void Arr_Init(void)
{
	uint8_t i;

	
	
	for(i=0; i<En_Or_Dis_NUM; i++){
		System_Feature_menu[i].status = System_Feature_Arr[i];
	}
	
	for(i=0; i<CONFIG_FORMAT_NUM; i++){
		Config_Format_menu[i].status = Config_Format_Arr[i];
	}
	
	for(i=0; i<InputNum; i++){
		Config_Video_Input_menu[i].status = Config_Video_Input_Arr[i];
	}
	
	for(i=0; i<OutputNum; i++){
		Config_Video_Output_menu[i].status = Config_Video_Output_Arr[i];
	}
/*
	for(i=0; i<CONFIG_NETWORK_IP_MENU; i++){
		Config_Network_IP_menu[i].status = Ip_Arr[i];
	}
	
	for(i=0; i<CONFIG_NETWORK_MAC_MENU; i++){
		Config_Network_MAC_menu[i].status = Mac_Arr[i];
	}
*/	
	for(i=0; i<SRC_MODE_NUM; i++){
		Config_SRC_Mode_menu[i].status = Config_Src_Mode_Arr[i];
	}
	
	for(i=0; i<SRC_SYNV_TYPE_NUM; i++){
		Config_SRC_Sync_Type_menu[i].status = Config_Src_Sync_Type_Arr[i];
	}
	
	for(i=0; i<SRC_CLOCK_TYPE_NUM; i++){
		Config_SRC_Clock_Type_menu[i].status = Config_Src_Clock_Type_Arr[i];
	}
	
	for(i=0; i<SRC_SYNC_FRAME_NUM; i++){
		Config_SRC_Sync_Frame_menu[i].status = Config_Src_Sync_Frame_Arr[i];
	}
	
	for(i=0; i<DISPLAY_MODE_NUM; i++){
		Config_Display_Mode_menu[i].status = Config_Display_Mode_Arr[i];
	}
	
	for(i=0; i<DISPLAY_BRIGHTNESS_NUM; i++){
		Config_Display_Brightness_menu[i].status = Config_Display_Brightness_Arr[i];
	}
	
	
		ip_mac_char();
}

extern void Arr_Update(void)
{
	uint8_t i;
	uint8_t Update_Flash_Flag=0;
	
	for(i=0; i<En_Or_Dis_NUM; i++){
		System_Feature_Arr[i] = System_Feature_menu[i].status;
	}
	
	for(i=0; i<CONFIG_FORMAT_NUM; i++){
		Config_Format_Arr[i] = Config_Format_menu[i].status;
	}
	
	for(i=0; i<InputNum; i++){
		Config_Video_Input_Arr[i] = Config_Video_Input_menu[i].status;
	}
	
	for(i=0; i<OutputNum; i++){
		Config_Video_Output_Arr[i] = Config_Video_Output_menu[i].status;
	}
/*
	for(i=0; i<CONFIG_NETWORK_IP_MENU; i++){
		Ip_Arr[i] = Config_Network_IP_menu[i].status ;
	}
	
	for(i=0; i<CONFIG_NETWORK_MAC_MENU; i++){
		Mac_Arr[i] = Config_Network_MAC_menu[i].status;
	}
*/
	for(i=0; i<SRC_MODE_NUM; i++){
		Config_Src_Mode_Arr[i] = Config_SRC_Mode_menu[i].status;
	}
	
	for(i=0; i<SRC_SYNV_TYPE_NUM; i++){
		Config_Src_Sync_Type_Arr[i] = Config_SRC_Sync_Type_menu[i].status;
	}
	
	for(i=0; i<SRC_CLOCK_TYPE_NUM; i++){       //!!!!!!!!!!!!!!!!!!!!!!
		Config_Src_Clock_Type_Arr[i] = Config_SRC_Clock_Type_menu[i].status;
	}
/*
	if(Config_Src_Clock_Type_Arr[0] == MENU_SET){
		Update_Flash_Flag=1;
		Config_Src_Sync_Type_Arr[7] = MENU_RESET;
		Config_Src_Sync_Type_Arr[8] = MENU_RESET;
		Config_Src_Sync_Type_Arr[9] = MENU_RESET;
		Config_Src_Sync_Type_Arr[10] = MENU_RESET;
		Config_Src_Sync_Type_Arr[11] = MENU_RESET;
	}else if(Config_Src_Clock_Type_Arr[1] == MENU_SET || Config_Src_Clock_Type_Arr[2] == MENU_SET){
		Update_Flash_Flag=1;
		Config_Src_Sync_Type_Arr[0] = MENU_RESET;
		Config_Src_Sync_Type_Arr[1] = MENU_RESET;
		Config_Src_Sync_Type_Arr[2] = MENU_RESET;
		Config_Src_Sync_Type_Arr[3] = MENU_RESET;
		Config_Src_Sync_Type_Arr[4] = MENU_RESET;
		Config_Src_Sync_Type_Arr[5] = MENU_RESET;
		Config_Src_Sync_Type_Arr[6] = MENU_RESET;
	}
*/
	
	for(i=0; i<SRC_SYNC_FRAME_NUM; i++){
		Config_Src_Sync_Frame_Arr[i] = Config_SRC_Sync_Frame_menu[i].status;
	}
	
	for(i=0; i<DISPLAY_MODE_NUM; i++){
		Config_Display_Mode_Arr[i] = Config_Display_Mode_menu[i].status;
	}
	
	for(i=0; i<DISPLAY_BRIGHTNESS_NUM; i++){
		Config_Display_Brightness_Arr[i] = Config_Display_Brightness_menu[i].status;
	}

	//if(Update_Flash_Flag)
	//	Set_Update_Flash_Flag(1);
	Write2Flash();
	
}
/*
** Public methods to access home view and home view size
*/

MENU_ITEM *menuItemsGetHomeMenu(){
	return home_items;	
}

unsigned char menuItemsGetHomeMenuSize(){
	return MENU_HOME_ITEMS;
}
