
#ifndef __KEY_H_
#define __KEY_H_

//按键初始化函数
extern void MX_KEY_Init(void);
extern void key_Scan(void);

extern uint8_t Is_1s_led(void);
extern uint8_t Is_100ms_led(void);
extern uint8_t Is_10ms_led(void);

extern void Set_10ms_led(void);
#endif /*__KEY_H_*/
