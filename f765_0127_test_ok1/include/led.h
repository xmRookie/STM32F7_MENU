#ifndef __LED_H_
#define __LED_H_

extern void led_Par_Init(void);
extern void MX_LED_Init(void);
extern void Led_Test(void);
extern void UserLed_Test(void);
//extern void Led1_Set(Boolean status);
//extern void Led2_Set(Boolean status);
extern void Led_Test1(void);







extern void UserLEDB(uint8_t status);
extern void UserLEDR(uint8_t status);
extern void HDMI_IN_LEDG(uint8_t status);
extern void HDMI_OUT_LEDG(uint8_t status)   ;
extern void HDSDI_IN_LEDG(uint8_t status)   ;
extern void HDSDI_OUT_LEDG(uint8_t status);





#endif /*__LED_H_*/
