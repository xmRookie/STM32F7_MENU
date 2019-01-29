
#ifndef __SPI5_H__
#define __SPI5_H__

extern void MX_SPI5_Init(void);
extern void SPI5_TxCpltCallback(void);
extern void SPI5_RxCpltCallback(void);
extern void SPI5_TxRxCpltCallback(void);
extern void SPI5_CheckSerial(void);
extern void SPI5_RxReaddy(void);

#endif //__SPI5_H__
