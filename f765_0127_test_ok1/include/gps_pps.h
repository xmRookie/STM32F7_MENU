
#ifndef __OLED_H
#define __OLED_H	

extern void GPS_PPS_Interrupt(void);

extern void GPS_PPS_Init(void);

extern uint8_t Get_GPS_sec(void);

extern uint32_t Get_cpuclk(void);

extern void cpuclk_cnt_add(void);

extern void Set_Send_Sec_Plus(uint8_t flag,uint8_t Sec)
extern uint8_t Get_Send_Sec_Plus(void);
extern uint8_t Get_GPS_Sec(void);
extern void Send_GPS_info(void);


extern void Set_Send_Dirft_Flag(uint8_t flag);
extern uint8_t Get_Send_Dirft_Flag(void);

#endif

