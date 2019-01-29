#include "common.h"

SPI_HandleTypeDef hspi1;

DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

static SPI_T spi1;

static uint8_t spi1_tx_buf[spi_tx_buf_Number];
//static uint8_t spi1_rx_buf[spi_rx_buf_Number];

static uint8_t spiHeartPackRX = 0;
static uint8_t spiHeartPackTX = LAN_ACK;
static uint8_t spicheck = 0;


#if 1
static uint8_t ts_stream[]={0x47,0x00 ,0x40 ,0x12 ,0x02 ,0x92 ,0xBC ,0x30 ,0xE0 ,0x53 ,0x31 ,0xD8 ,0xA7 ,0xF0 ,0x17 ,0x01
,0xE2 ,0xE4 ,0x07 ,0xBE ,0x9B ,0x3B ,0xAE ,0x03 ,0xA3 ,0xAE ,0x3C ,0xA3 ,0x34 ,0xAC ,0x13 ,0xB4
,0x46 ,0xC7 ,0x4D ,0x76 ,0xD0 ,0x9D ,0x95 ,0xBA ,0x56 ,0x8D ,0x48 ,0x4B ,0xAE ,0xC4 ,0xBA ,0xAC
,0xBD ,0x2B ,0x4E ,0x74 ,0x66 ,0x60 ,0x49 ,0x1A ,0xAE ,0x1D ,0xA9 ,0x6C ,0x1D ,0x79 ,0x9B ,0x42
,0xC9 ,0xAB ,0x2F ,0x13 ,0xF0 ,0x36 ,0xFE ,0x9F ,0x8D ,0x61 ,0xC9 ,0x30 ,0x3A ,0x1A ,0x0E ,0x47
,0xA6 ,0xA9 ,0xAA ,0x66 ,0x68 ,0x9A ,0xD0 ,0xF3 ,0xD3 ,0x52 ,0xF6 ,0x2A ,0xEF ,0x4E ,0xD0 ,0x1E
,0x00 ,0x63 ,0x22 ,0x50 ,0x66 ,0xC9 ,0xCB ,0x3B ,0xB0 ,0xB2 ,0x8D ,0xDC ,0x48 ,0xAE ,0x6E ,0xD2
,0xED ,0xD2 ,0x6B ,0xDC ,0x2E ,0x51 ,0x0D ,0xD6 ,0x7A ,0xDE ,0x7C ,0x06 ,0xEC ,0xC8 ,0xED ,0x26
,0xD2 ,0xF0 ,0xD5 ,0x26 ,0xAE ,0x9C ,0x74 ,0xE6 ,0x1F ,0xE6 ,0x46 ,0x72 ,0x62 ,0x93 ,0xFC ,0xCD
,0x9F ,0x37 ,0xA8 ,0x23 ,0x89 ,0x23 ,0x8F ,0x8E ,0xD5 ,0x8F ,0x08 ,0x24 ,0xE7 ,0x99 ,0xAF ,0xA0
,0x65 ,0xA9 ,0xA8 ,0xCA ,0x2C ,0xB4 ,0xD6 ,0x62 ,0x7E ,0xD8 ,0xFB ,0x55 ,0x0E ,0x14 ,0x7E ,0x5F
,0xCD ,0xB3 ,0xDC ,0xBF ,0x82 ,0xFD ,0xEA ,0x10 ,0xD7 ,0x56 ,0x81 ,0x6A};

#endif
/* SPI1 init function */
extern void MX_SPI1_Init(void)
{

    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;//SPI_NSS_HARD_OUTPUT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

    if(HAL_SPI_Init(&hspi1) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
	  memset(&spi1, 0, sizeof(spi1));
	  spiHeartPackRX =0;
	  spiHeartPackTX = LAN_ACK;
	  spicheck =1;
}

extern void SPI1_RxCpltCallback(void)
{
       
    //?車那?米?那y?Y
}




extern void SPI1_TxRxCpltCallback(void)
{
		if(spiHeartPackRX == spiHeartPackTX)
		{	
			spiHeartPackRX =0;
			spicheck = 0;
			spi1.mode =SPI_TS_TX;
			//Led_Test();
		}
		else
		{
			spicheck = 1;
		}

    //?車那?米?那y?Y
}

extern void SPI1_TxCpltCallback(void)
{
	if(spi1.tx.Write == spi1.tx.Read) 
	{
		// 那y?Y辰??-﹞⊿?赤赤那
		spi1.tx.HaveData = 0;
				
	} 
	else {
		HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)&spi1_tx_buf[spi1.tx.Read], 1);
	   spi1.tx.Read = RBUF_NEXT_PT(spi1.tx.Read, 1, sizeof(spi1_tx_buf));
	}


}



extern void SPI1_RxReaddy(void)
{
    if(spi1.rx.busy == 0) {
        if(spi1.rx.rxcount) {
            spi1.rx.timeout++;

            if(spi1.rx.timeout > SPI_RXTIMEOUT) {
                spi1.rx.busy = 1;
            }
        }
    }
}


static Boolean SPI1_GetRxBufDat(void)
{
	uint16_t head,i;
	uint16_t mRead;

   if(spi1.tx.Write != spi1.tx.Read)
   {	
   		mRead = spi1.tx.Read;
		
   		//for(i=0;i<TS_PACKET_SIZE;i++)
		{
			head = RBUF_NEXT_PT(mRead, 1, spi_tx_buf_Number);

	    	if(head == spi1.tx.Write) { //?迆
	    		return FALSE;
	    	}
			//ts_pack[TS_PACKET_SIZE] = spi1_tx_buf[mRead];
			mRead = head;
			HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)&spi1_tx_buf[spi1.tx.Read], 1);
        
			spi1.tx.Read = mRead;	
   		}
		
		return TRUE;
   }
   return FALSE;
}


extern void SPI1_SendTS_Stream(uint8_t *buf,uint16_t len)
{
	//uint8_t flag;
	uint16_t head,i;
	//uint16_t mWrite;
	//flag = len % TS_PACKET_SIZE;
	
	//if(buf[0] == 0x47  && len > 0 && flag ==0)
	{
		//mWrite = spi1.tx.Write;
		for(i=0;i<len;i++)
		{
			head = RBUF_NEXT_PT(spi1.tx.Write, 1, sizeof(spi1_tx_buf));
	    	while(head == spi1.tx.Read) { //?迆
	    	}
			
			spi1_tx_buf[spi1.tx.Write] = buf[i];
			spi1.tx.Write = head;
		}
		if(spi1.tx.HaveData == 0) {
	        // ﹞⊿?赤?1??車D???‘
	        spi1.tx.HaveData = 1;
	        HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)&spi1_tx_buf[spi1.tx.Read], 1);

	        spi1.tx.Read = RBUF_NEXT_PT(spi1.tx.Read, 1, sizeof(spi1_tx_buf));
   		 }
		//spi1.tx.Write = mWrite;
	}
}

extern void SPI1_CheckSerial(void)
{

    switch(spi1.mode)
    {
		case SPI_TS_ILDE:
			if(spicheck)
			{
				spiHeartPackRX =0;
	  			spiHeartPackTX = LAN_ACK;
				HAL_SPI_TransmitReceive_DMA(&hspi1,&spiHeartPackTX,&spiHeartPackRX,1);
			}
			break;
		case SPI_TS_TX:
			SPI1_SendTS_Stream(ts_stream,TS_PACKET_SIZE);
			break;
	}

 }

