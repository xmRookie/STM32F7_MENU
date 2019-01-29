
#ifndef __SPI4_H__
#define __SPI4_H__

extern void MX_SPI4_Init(void);
extern void SPI4_TxRxCpltCallback(void);


extern void Test_Send_To_Fpga(void);
extern void Send_To_Fpga(uint8_t *Send_Data,uint16_t Send_Len);
extern void Power_On_Read_Reg_Once(void);


extern void Retry_SPI4(void);

extern void Send_SyncType_Mode(uint8_t ch);
extern void Send_ClockSync_Mode(uint8_t ch);
extern void Send_Src_Mode(uint8_t ch);
extern void Send_SyncFrame_Mode(uint8_t ch);
extern void Send_GPS_Sec_Num(uint8_t Sec);
extern void Send_GPS_Sec_Pulse(uint8_t Pulse);
extern void Send_Test_Pattern(uint8_t ch) ;
extern void Send_FPGA_Restart(uint8_t ch) ;
extern void Set_FPGA(void);
extern void Reset_FPGA(void);
extern void Send_GPS_Dirft(uint8_t addr,uint8_t data);
extern void Send_Format_2_0x05(uint8_t ch);
extern void Send_Format_2_0x11(uint8_t ch);

extern uint8_t Get_Jit_Reg_1(void);
extern uint8_t Get_Jit_Reg_2(void);
extern uint8_t* Get_FW(void);
extern uint8_t Get_Retry_SPI4_Flag(void);
extern uint8_t Get_Reg_0x82(void);
extern uint8_t Get_Reg_0x83(void);
extern uint8_t Get_Reg_0x84(void);
extern uint8_t Get_Reg_0x87(void);
extern uint8_t Get_Reg_0x88(void);
extern uint8_t Get_Reg_0x89(void);
extern uint8_t Get_Reg_0x8a(void);
extern uint8_t Get_Reg_0x8b(void);
extern uint8_t Get_Reg_0x8c(void);
extern uint8_t Get_Reg_0x8d(void);
#endif //__SPI4_H__
