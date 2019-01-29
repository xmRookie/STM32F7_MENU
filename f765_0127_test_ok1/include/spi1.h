
#ifndef __SPI1_H__
#define __SPI1_H__

extern void MX_SPI1_Init(void);

extern void SPI1_TxCpltCallback(void);

extern void SPI1_RxCpltCallback(void);

extern void SPI1_TxRxCpltCallback(void);

extern void SPI1_SendTS_Stream(uint8_t *buf,uint16_t len);

extern void SPI1_CheckSerial(void);

extern void SPI1_RxReaddy(void);


#endif //__SPI1_H__
