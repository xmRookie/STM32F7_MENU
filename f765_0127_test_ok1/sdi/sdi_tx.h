#ifndef __SDI_TX_H_
#define __SDI_TX_H_

#define SDI_TX_SP	HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_10)
extern void TX_SDI_Init(void);
extern void sdi_tx(void);
/*
extern void SDI_TX_EQ(Boolean status);
extern void SDI_TX_SD(Boolean status);
extern void SDI_TX_DSAB(Boolean status);
*/
#endif /*__SDI_TX_H_*/