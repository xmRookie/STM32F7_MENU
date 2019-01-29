#include "common.h"

//#define SYNCHPRRDIV 19999

uint32_t SYNCHPRRDIV=19999;   // AsynchPrediv =     0; 

RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef stimestructure;
RTC_DateTypeDef sdatestructure;

/* RTC init function */
extern void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  RTC_AlarmTypeDef sAlarm;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv =     0;        //124;    div8 1s                    // HSE  25M DIV25  1000KHZ   100                 0-127
  hrtc.Init.SynchPrediv = SYNCHPRRDIV;         //24999;  div8 1s                     // HSE  25M DIV25  1000KHZ   1000   较精准1s   0-32767
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
		printf("rtc init err ");
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;  
  sDate.Month = RTC_MONTH_NOVEMBER;    //11月
  sDate.Date = 0x1;      // 1号
  sDate.Year = 0;//0x18;    //2018年

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Enable the Alarm A 
    */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }
	printf("rtc init success \r\n");
}

extern void Get_RTC_Time(void)
{
		//HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
			HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
      /* Get the RTC current Date */
      HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);

      /* Display date Format : yy/mm/dd */

 //     printf("%02d/%02d/%02d\r\n",2000 + sdatestructure.Year, sdatestructure.Month, sdatestructure.Date);

      /* Display time Format : hh:mm:ss */
			printf("%02d:%02d:%02d:%d:%d:%d    \r\n",stimestructure.Hours, stimestructure.Minutes, stimestructure.Seconds,stimestructure.SubSeconds,stimestructure.SecondFraction,
					stimestructure.DayLightSaving);
}

extern uint32_t Get_RTC_clk(void)
{
	uint32_t clk;
	
			HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
      /* Get the RTC current Date */
      HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);
      /* Display date Format : yy/mm/dd */
			/* Display time Format : hh:mm:ss */
	//	printf("%02d:%02d:%02d:%d:%d\r\n",stimestructure.Hours, stimestructure.Minutes, stimestructure.Seconds,stimestructure.SubSeconds,stimestructure.SecondFraction);
	
	clk = (((stimestructure.Hours)*60+(stimestructure.Minutes))*60+(stimestructure.Seconds))*(SYNCHPRRDIV+1) + ((SYNCHPRRDIV+1) - (stimestructure.SubSeconds));
		//	printf(" %d ",clk);
	
	return clk;
	
}

extern void Set_RTC_Time(char Time[])   //hhmmss.00
{
		//HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
	RTC_TimeTypeDef sTime;
	uint8_t BCD_Time[6]={0};
	uint8_t i=0;
	
	for(i=0; i<6; i++){
		BCD_Time[i] = Time[i] - 48;           //char --> uint8_t
		if(i%2){ 
			BCD_Time[i-1] = BCD_Time[i-1] * 16 + BCD_Time[i];    //   uint8_t  ->  BCD
		}
	}
	
	sTime.Hours = BCD_Time[0];
  sTime.Minutes = BCD_Time[2];
  sTime.Seconds = BCD_Time[4];
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
}

extern void Set_RTC_Date(char Date[])   //ddmmyy
{
		//HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)
	RTC_DateTypeDef sDate;
	uint8_t BCD_Date[6]={0};
	uint8_t i=0;
	
	for(i=0; i<6; i++){    
		BCD_Date[i] = Date[i] - 48;           //char --> uint8_t
		if(i%2){ 
			BCD_Date[i-1] = BCD_Date[i-1] * 16 + BCD_Date[i];    //   uint8_t  ->  BCD
		}
	}	
	
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;  
  sDate.Month = BCD_Date[0];       //RTC_MONTH_NOVEMBER;   //月
  sDate.Date = BCD_Date[2];      // 日
  sDate.Year = BCD_Date[4];    //  年

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
}


extern void Get_RTCtime(uint8_t buf[][20])
{
	char Time[6]={0};
	
	
	
	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);     //  同时   	get uint8_t 
	HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);     //  同时    get uint8_t 
	//  uint8_t 型 数字 1  转成  char 型 字符 '1'
	Time[0] = (stimestructure.Hours / 10) + 48;    
	Time[1] = (stimestructure.Hours % 10) + 48;
	Time[2] = (stimestructure.Minutes / 10) + 48;
	Time[3] = (stimestructure.Minutes % 10) + 48;
	Time[4] = (stimestructure.Seconds / 10) + 48;
	Time[5] = (stimestructure.Seconds % 10) + 48;
	
	//printf(" %x ",Time[5]);printf(" %x ",stimestructure.Seconds );printf(" %c ",Time[5]);printf(" %c ",Time[5]);printf(" %c ",Time[5]);
	
		buf[0][0] = Time[0];
		buf[0][1] = Time[1];
		buf[0][2] = ':';
		buf[0][3] = Time[2];
		buf[0][4] = Time[3];
		buf[0][5] = ':';
		buf[0][6] = Time[4];
		buf[0][7] = Time[5];
		buf[0][8] = ' ';
		buf[0][9] = 'R';
		buf[0][10] = 'T';
		buf[0][11] = 'C';
	
}	



