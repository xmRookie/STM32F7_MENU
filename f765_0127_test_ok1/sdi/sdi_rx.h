#ifndef __SDI_RX_H_
#define __SDI_RX_H_

#define SDI_RX_CD	HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_6)
extern void RX_SDI_Init(void);
extern void sdi_rx(void);
/*
extern void SDI_RX_OP_CTL(Boolean status);
extern void SDI_RX_SO_ADJ(Boolean status);
extern void SDI_RX_BYPS(Boolean status);
extern void SDI_RX_GS(Boolean status);
extern void SDI_RX_SLP(Boolean status);
*/
#endif /*__SDI_RX_H_*/