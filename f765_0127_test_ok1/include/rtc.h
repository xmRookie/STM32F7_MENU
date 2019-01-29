
#ifndef __RTC_H__
#define __RTC_H__


//时间结构体
typedef struct 
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;            
    //公历 年 月 日 周
    uint16_t w_year;
    uint8_t  w_month;
    uint8_t  w_date;
    uint8_t  week;     
}_calendar_obj;                     
extern _calendar_obj calendar;
void RCC_Configuration(void);
void RTC_Init(void);
uint8_t RTC_Set(uint16_t year,uint8_t mon,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Get(void);

extern void MX_RTC_Init(void);

extern void Get_RTC_Time(void);

extern void Set_RTC_Time(char Time[]);

extern void Set_RTC_Date(char Date[]);

extern void Get_RTCtime(uint8_t buf[][20]);

extern uint32_t Get_RTC_clk(void);

#endif
