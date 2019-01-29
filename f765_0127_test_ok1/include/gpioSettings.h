#ifndef __GPIOSETTINGS_H_
#define __GPIOSETTINGS_H_

#define PinTxInt	HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_2)

extern void HDMI_RX_Init(void);
extern void HDMI_TX_Init(void);
extern void HDMI_PinTxInt_Init(void);

#endif /*__GPIOSETTINGS_H_*/
