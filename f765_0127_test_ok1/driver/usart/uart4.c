#include "common.h"

UART_HandleTypeDef huart4;

#define   usart4_tx_buf_Number  0x2000          // ·¢ËÍ»º³åÇø´óÐ¡,±ØÐëÎª2µÄn´Î·½
#define   usart4_rx_buf_Number  0x200          // ½Ó½Ó»º³åÇø´óÐ¡,±ØÐëÎª2µÄn´Î·½

#define Uart_Rx_Len 1

#define false 0
#define true 1

//¶¨ÒåÊý×é³¤¶È
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
//	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//ÊÇ·ñ»ñÈ¡µ½GPSÊý¾Ý
	char isParseData;	//ÊÇ·ñ½âÎöÍê³É
	char UTCTime[UTCTime_Length];		//UTCÊ±¼ä
	char UTCDate[UTCTime_Length];
	char latitude[latitude_Length];		//Î³¶È
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//¾­¶È
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//¶¨Î»ÐÅÏ¢ÊÇ·ñÓÐÐ§
} _SaveData;

_SaveData Save_Data;

typedef struct {
    uint16_t                 Write;
    volatile uint16_t        Read;
    volatile Boolean        HaveData;
} UART_TX_T;

typedef struct {
    volatile uint16_t        rxcount;
    volatile uint8_t        timeout;
    Boolean                 busy;
} UART_RX_T;

typedef struct {
    UART_TX_T               tx;
    UART_RX_T               rx;
    uint8_t                 AutoChgBRageTimeOut;
    Boolean                 NotIn9600;
} UART_T;

static UART_T uart4;

static uint8_t uart4_tx_buf[usart4_tx_buf_Number];
static uint8_t uart4_rx_buf[usart4_rx_buf_Number];

static uint8_t utc_time=0;
static uint8_t gps_value=0;

//GPS

/* USART4 init function */
extern void MX_UART4_Init(void)
{

    huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;  //115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
 // huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
 // huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	HAL_UART_Receive_IT(&huart4, (uint8_t*)&uart4_rx_buf[uart4.rx.rxcount], Uart_Rx_Len);
	
	SET_BIT(huart4.Instance->CR1, USART_ISR_IDLE);    //ÉèÖÃ´®¿Ú¿ÕÏÐÖÐ¶Ï  
	
}

extern void UART4_IRQ_Deal(UART_HandleTypeDef *huart)
{
	uint32_t isrflags   = READ_REG(huart->Instance->ISR);

	if( isrflags & (uint32_t)(USART_ISR_IDLE)){
		__HAL_UART_CLEAR_IT(huart, UART_CLEAR_IDLEF);    //Çå³þ¿ÕÏÐÖÐ¶Ï±êÖ¾Î»
 
		pollint_GPS();
	}
}

extern void UART4_RX_Deal(void)
{
		//Status_busy(1);
	 if(uart4.rx.busy == 0) {

        uart4.rx.timeout = 0;

        uart4.rx.rxcount = RBUF_NEXT_PT(uart4.rx.rxcount, Uart_Rx_Len, usart4_rx_buf_Number);

        HAL_UART_Receive_IT(&huart4, (uint8_t *)&uart4_rx_buf[uart4.rx.rxcount], Uart_Rx_Len); //??D???¨°?a????¨2?D??
    }
}


static uint16_t UART4_GetRxBufDat(char *buf)
{
    uint16_t len;
    uint8_t err;

    len = uart4.rx.rxcount;
    memcpy(buf, uart4_rx_buf, uart4.rx.rxcount);
    uart4.rx.rxcount = 0;

    huart4.RxState = HAL_UART_STATE_READY;    //Must be forced Preparation , Otherwise the next sentence is useless

    err = HAL_UART_Receive_IT(&huart4, (uint8_t *)&uart4_rx_buf[uart4.rx.rxcount], Uart_Rx_Len);
	
    if(err != HAL_OK) {
			printf("%d",err);
    }

    uart4.rx.timeout = 0;
    uart4.rx.busy = 0;

    return len;
}

extern void  pollint_GPS(void)
{
	uint16_t len;
	char GPS_Data[500];
	uint16_t i;
	
	uint16_t Gprmc_Len=0;
	char Gprmc_BUF[500];          //´óÒ»µã
	
	len = UART4_GetRxBufDat(GPS_Data);        ///¿ÉÄÜÕâÀï¿¨ËÀ£¿£¿£¿£¡£¡£¡£¡
	
	if(len){
		for(i=0; i<len; i++){
			if(GPS_Data[i] == '\n'){
					Gprmc_Len++;	
					parseGpsBuffer(Gprmc_BUF,Gprmc_Len);
					Gprmc_Len=0;
			}else{
					Gprmc_BUF[Gprmc_Len] = GPS_Data[i];   //save 
					Gprmc_Len++;			
			}
		}
	}
	
	//	printf("\r\n");	
	
}

extern uint8_t Get_gps_value(void)
{
	//printf(" %d ",Save_Data.isUsefull);
	return Save_Data.isUsefull;
}




void parseGpsBuffer(char *buf,uint16_t len)
{
	uint16_t i=0;
	char *subString;
	char *subStringNext;

	
	if( buf[0] == '$' && buf[4] == 'M' && buf[5] == 'C'){    // GPRMC	
				for (i = 0 ; i <= 9 ; i++)
				{
					if (i == 0)
					{
						if ((subString = strstr(buf, ",")) == NULL)
							printf("err1");							//½âÎö´íÎó
					}
					else
					{
						subString++;
						if ((subStringNext = strstr(subString, ",")) != NULL)
						{
							char usefullBuffer[2]; 
							switch(i)
							{
								case 1: memcpy(Save_Data.UTCTime, subString, subStringNext - subString);   //UTC
												Display_UTCTime(Save_Data.UTCTime);  //hhmmss.00
												utc_time=1;
										break;	//»ñÈ¡UTCÊ±¼ä
								case 2: memcpy(usefullBuffer, subString, subStringNext - subString);
										break;	//»ñÈ¡UTCÊ±¼ä
								case 3: memcpy(Save_Data.latitude, subString, subStringNext - subString);
										//			Display_Latitude(Save_Data.latitude);
										break;	//»ñÈ¡Î³¶ÈÐÅÏ¢
								case 4: memcpy(Save_Data.N_S, subString, subStringNext - subString);									
										//		printf("    ");
												if(Save_Data.N_S[0] != 0)
													//OLED_ShowChar(116,4,Save_Data.N_S[0]);
										break;	//»ñÈ¡N/S
								case 5: memcpy(Save_Data.longitude, subString, subStringNext - subString);
											//		Display_Longitude(Save_Data.longitude);
										break;	//»ñÈ¡¾­¶ÈÐÅÏ¢
								case 6: memcpy(Save_Data.E_W, subString, subStringNext - subString);
										//		printf(Save_Data.E_W);   //E  W
										//		printf("    ");
												if(Save_Data.E_W[0] != 0)
													//OLED_ShowChar(48,4,Save_Data.E_W[0]);
										break;	//»ñÈ¡E/W
								case 7:
										break;
								case 9: memcpy(Save_Data.UTCDate, subString, subStringNext - subString);
												Display_UTCDate(Save_Data.UTCDate);
											//	Get_UTCDate(Save_Data.UTCDate);
												//printf(Save_Data.UTCDate);     //ddmmyy
										break;	//»ñÈ¡E/W

								default:
										break;
							}

							subString = subStringNext;
							Save_Data.isParseData = true;
							if(usefullBuffer[0] == 'A'){
								Save_Data.isUsefull = true;
							}
							else if(usefullBuffer[0] == 'V'){
								Save_Data.isUsefull = false;
							}

						}
						else
						{
							printf("err2");  //½âÎö´íÎó
						}
					}
				}
	}
	
}




void Display_UTCTime(char *UTCTime)   //hhmmss.00
{
	int i;
	int hour_1,hour_2,hour;
	int x,y;
	y=2;
	x=16;
	

if(UTCTime[0] != 0){
#if 0	
		hour_1 = (UTCTime[0] & 0x0f) * 10;
		hour_2 = (UTCTime[1] & 0x0f) * 1;
		hour = hour_1 + hour_2 + Get_UTCoffset_Num();    //16
		if(hour>=24) 
			hour -= 24;
		
		printf("%d",hour);
	//	printf("%c",UTCTime[0]);
	//	printf("%c",UTCTime[1]);
		printf("Ê±");
		printf("%c",UTCTime[2]);
		printf("%c",UTCTime[3]);
		printf("·Ö");
		printf("%c",UTCTime[4]);
		printf("%c",UTCTime[5]);
		printf("Ãë     ");
#endif				
	}
}



extern void Get_UTCTime(uint8_t buf[][20],uint8_t line)
{
	static uint8_t rtc_sync_utc=1;
	uint8_t Sec_Num=0;
	int8_t time;
	
	if(Save_Data.UTCTime[0] != 0){   //£¡=0ËµÃ÷ÓÐÊý¾Ý £» =0 ¼´¿Õ  ²»²Ù×÷
		if(utc_time){
/*			if(Save_Data.UTCTime[1] < '2'){
				Save_Data.UTCTime[1] += 8;
			}else{
				Save_Data.UTCTime[1] -= 2;
				Save_Data.UTCTime[0] += 1;
			}
*/
			time = (Save_Data.UTCTime[0] - 0x30) * 10 + (Save_Data.UTCTime[1] - 0x30);
			time += Get_UTCoffset_Num();
			
			while(time > 24)
				time -= 24;
			while(time < 0)
				time += 24;
			
			Save_Data.UTCTime[0] = time / 10 + 0x30;
			Save_Data.UTCTime[1] = time % 10 + 0x30;
			
			utc_time=0;
		}
		//printf(" gpstime ");     //ÈçºÎ  ÈÃ   ¶Ïµô  gps ºó  ²»ÔÙ ½øÀ´  ´Ëº¯Êý
#if 0		//rtc  30sÐ£×¼Ò»´Î gps
		if( (Save_Data.UTCTime[4] == '3' && Save_Data.UTCTime[5] == '0' ) || (Save_Data.UTCTime[4] == '0' &&  Save_Data.UTCTime[5] == '0') ){
			Set_RTC_Time(Save_Data.UTCTime);     //  set RTC time                    char
		}else if(rtc_sync_utc){
			rtc_sync_utc=0;
			Set_RTC_Time(Save_Data.UTCTime); 
		}
#endif		
		buf[line][0] = Save_Data.UTCTime[0];
		buf[line][1] = Save_Data.UTCTime[1];
		buf[line][2] = 'h';
		buf[line][3] = Save_Data.UTCTime[2];
		buf[line][4] = Save_Data.UTCTime[3];
		buf[line][5] = 'm';
		buf[line][6] = Save_Data.UTCTime[4];
		buf[line][7] = Save_Data.UTCTime[5];
		buf[line][8] = 's';

		Sec_Num = Save_Data.UTCTime[5] - 0x30;
		Sec_Num += (Save_Data.UTCTime[4] - 0x30) * 10;
//		Set_Send_Sec_Plus(1,Sec_Num);
		
		
		memset(Save_Data.UTCTime,0,sizeof(Save_Data.UTCTime));    //  ½«Êý×éÇå0£¬ ±£Ö¤£¬  ¶Ïµôgpsºó   ÅÐ¶Ï ²»ÔÙ½øÈë´Ëº¯Êý
	}
}


void Display_UTCDate(char *UTCDate)   //ddmmyy
{
	int x,y;
	x=16;
	y=0;
#if 0
	if(UTCDate[0] != 0){
		printf("%c",UTCDate[4]);
		printf("%c",UTCDate[5]);
		printf("Äê");
		printf("%c",UTCDate[2]);
		printf("%c",UTCDate[3]);
		printf("ÔÂ");
		printf("%c",UTCDate[0]);
		printf("%c",UTCDate[1]);
		printf("ÈÕ    ");
	}
#endif	
}


extern void Get_UTCDate(uint8_t buf[][20])   //ddmmyy
{
	if(Save_Data.UTCDate[0] != 0){
		
		Set_RTC_Date(Save_Data.UTCDate);
		
		buf[0][0] = '2';
		buf[0][1] = '0';
		buf[0][2] = Save_Data.UTCDate[4];   // 1
		buf[0][3] = Save_Data.UTCDate[5];   // 8
		buf[0][4] = '/';
		buf[0][5] = Save_Data.UTCDate[2];
		buf[0][6] = Save_Data.UTCDate[3];
		buf[0][7] = '/';
		buf[0][8] = Save_Data.UTCDate[0];
		buf[0][9] = Save_Data.UTCDate[1];
		
		memset(Save_Data.UTCDate,0,sizeof(Save_Data.UTCDate));    //  ½«Êý×éÇå0£¬ ±£Ö¤£¬  ¶Ïµôgpsºó   ÅÐ¶Ï ²»ÔÙ½øÈë´Ëº¯Êý
	}
}

/*
void Display_Latitude(char *Latitude)   //dddd.mmmmm
{
	int x,y;
	x=0;
	y=4;
	
	if(Latitude[0] != 0){
#if 0			
		printf("%c",Latitude[0]);
		printf("%c¡ã",Latitude[1]);

		printf("%c",Latitude[2]);
		printf("%c'",Latitude[3]);
		
		
		OLED_ShowChar(x+0,y,Latitude[0]);
		OLED_ShowChar(x+8,y,Latitude[1]);	
		OLED_ShowChar(x+16,y,'\'');
		OLED_ShowChar(x+20,y,'\'');
		OLED_ShowChar(x+24,y,Latitude[2]);
		OLED_ShowChar(x+32,y,Latitude[3]);
		OLED_ShowChar(x+40,y,'\'');
#endif			
	}
	
}
*/
extern void Get_Latitude(uint8_t buf[][20])
{
	if(Save_Data.UTCDate[0] != 0){
		buf[2][0] = Save_Data.latitude[0];
		buf[2][1] = Save_Data.latitude[1];
		buf[2][2] = '\'';
		buf[2][3] = Save_Data.latitude[2];
		buf[2][4] = Save_Data.latitude[3];
		buf[2][5] = '\'';

		
	}
}

/*
void Display_Longitude(char *Longitude)   //ddddd.mmmmm
{
	int x,y;
	x=60;
	y=4;
	
	if(Longitude[0] != 0){
#if 0		
		printf("%c",Longitude[0]);
		printf("%c",Longitude[1]);
		printf("%c¡ã",Longitude[2]);

		printf("%c",Longitude[3]);
		printf("%c'",Longitude[4]);
			
		OLED_ShowChar(x+0,y,Longitude[0]);
		OLED_ShowChar(x+8,y,Longitude[1]);	
		OLED_ShowChar(x+16,y,Longitude[2]);	
		OLED_ShowChar(x+24,y,'\'');
		OLED_ShowChar(x+28,y,'\'');
		OLED_ShowChar(x+32,y,Longitude[3]);
		OLED_ShowChar(x+40,y,Longitude[4]);
		OLED_ShowChar(x+48,y,'\'');
#endif			
	}
}
*/






