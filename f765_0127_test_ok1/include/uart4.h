
#ifndef __USART_UART4_H__
#define __USART_UART4_H__

extern uint8_t Get_gps_value(void);


extern void MX_UART4_Init(void);

extern void UART4_IRQ_Deal(UART_HandleTypeDef *huart);

extern void UART4_RX_Deal(void);

extern void  pollint_GPS(void);

void parseGpsBuffer(char *buf,uint16_t len);

void Display_UTCTime(char *UTCTime);

void Display_UTCDate(char *UTCDate);   //ddmmyy
	
void Display_Latitude(char *Latitude); //dddd.mmmmm

void Display_Longitude(char *Longitude) ; //ddddd.mmmmm

extern void Get_UTCDate(uint8_t buf[][20]);

extern void Get_UTCTime(uint8_t buf[][20],uint8_t line);

extern void Get_Latitude(uint8_t buf[][20]);
	
#endif
