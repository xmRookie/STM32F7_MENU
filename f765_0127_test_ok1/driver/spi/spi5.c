#include "common.h"

SPI_HandleTypeDef hspi5;
#ifdef SUP_DMA_SPI5_RX
DMA_HandleTypeDef hdma_spi5_rx;
#endif

#ifdef SUP_DMA_SPI5_TX
DMA_HandleTypeDef hdma_spi5_tx;
#endif



static SPI_T spi5;
static uint8_t spi5_tx_buf[spi_tx_buf_Number];
static uint8_t spi5_rx_buf[spi_rx_buf_Number];
static uint8_t spi5HeartPackRx = 0;
static uint8_t spi5HeartPackTx = LAN_ACK;


/* SPI4 init function */
extern void MX_SPI5_Init(void)
{

  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_SLAVE;;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 7;
  hspi5.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

   memset(&spi5, 0, sizeof(spi5));
#ifdef SUP_DMA_SPI5_RX
    HAL_SPI_TransmitReceive_DMA(&hspi5, (uint8_t*)&spi5HeartPackTx,(uint8_t*)&spi5HeartPackRx, 1);
#else
    HAL_SPI_TransmitReceive_IT(&hspi5, (uint8_t*)&spi5HeartPackTx,(uint8_t*)&spi5HeartPackRx,1);
#endif
}



void SPI5_DealTSDat(void)
{
#ifdef SUP_DMA_SPI5_RX
	HAL_SPI_Receive_DMA(&hspi5, (uint8_t*)&spi5_rx_buf[spi5.rx.rxcount],1);
#else
	HAL_SPI_Receive_IT(&hspi5, (uint8_t *)&spi5_rx_buf[spi5.rx.rxcount],1);
#endif

	spi5.rx.rxcount = RBUF_NEXT_PT(spi5.rx.rxcount, 1, spi_rx_buf_Number);

}

extern void SPI5_RxCpltCallback(void)
{
//    if(spi5.rx.busy == 0) 
//		{
		
//        spi5.rx.timeout = 0;
		switch(spi5.mode)
		{
			case SPI_TS_ILDE:
				break;
			case SPI_TS_RX:
				if(spi5.rx.rxcount == TS_PACKET_SIZE)
				{
					//udp_send_to_host(spi5_rx_buf,spi5.rx.rxcount);
					spi5.rx.rxcount = 0;
					//Led_Test();
				}

				SPI5_DealTSDat();

				break;
		}
			
		//USART3_Putchar(0x01);
    //?車那?米?那y?Y
}



extern void SPI5_TxRxCpltCallback(void)
{

	if(spi5HeartPackRx == spi5HeartPackTx)
	{
		spi5.mode = SPI_TS_RX;
		spi5.rx.rxcount =0;
		spi5HeartPackRx=0;
		SPI5_DealTSDat();
	}
	else
	{
		spi5HeartPackRx = 0;
		spi5HeartPackTx = LAN_ACK;
#ifdef SUP_DMA_SPI5_RX
		HAL_SPI_TransmitReceive_DMA(&hspi5, (uint8_t*)&spi5HeartPackTx,(uint8_t*)&spi5HeartPackRx, 1);
#else
		HAL_SPI_TransmitReceive_IT(&hspi5, (uint8_t*)&spi5HeartPackTx,(uint8_t*)&spi5HeartPackRx,1);
#endif
	}
	
}

extern void SPI5_TxCpltCallback(void)
{
    if(spi5.tx.Write == spi5.tx.Read) {
        // 那y?Y辰??-﹞⊿?赤赤那
        spi5.tx.HaveData = 0;
		//Led_Test();
    } 
//	else {
//#ifdef SUP_DMA_SPI5_TX
//       HAL_SPI_Transmit_DMA(&hspi5, (uint8_t *)&spi5_tx_buf[spi5.tx.Read], 1);
//#else
//        HAL_SPI_Transmit_IT(&hspi5, (uint8_t *)&spi5_tx_buf[spi5.tx.Read], 1);
//#endif
//        spi5.tx.Read = RBUF_NEXT_PT(spi5.tx.Read, 1, sizeof(spi5_tx_buf));
//    }
}

extern void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
   if(hspi->Instance == SPI5)
   	{
		 //Led_Test();
   }
}


extern void SPI5_RxReaddy(void)
{
    if(spi5.rx.busy == 0) {
        if(spi5.rx.rxcount) {
            spi5.rx.timeout++;

            if(spi5.rx.timeout > SPI_RXTIMEOUT) {
                spi5.rx.busy = 1;
            }
        }
    }
}


static uint16_t SPI5_GetRxBufDat(uint8_t *buf)
{
    uint16_t len;
	if( spi5.rx.rxcount)
		{
    len = spi5.rx.rxcount;
    memcpy(buf, spi5_rx_buf, spi5.rx.rxcount);
		}
    spi5.rx.rxcount = 0;
    spi5.rx.timeout = 0;
    spi5.rx.busy = 0;

    return len;
}

extern void SPI5_Putchar(uint8_t ch)
{
    uint16_t head;

    head = RBUF_NEXT_PT(spi5.tx.Write, 1, sizeof(spi5_tx_buf));

    while(head == spi5.tx.Read) { //?迆
    }

    spi5_tx_buf[spi5.tx.Write] = ch;
    spi5.tx.Write = head;

    if(spi5.tx.HaveData == 0) {
        // ﹞⊿?赤?1??車D???‘
        spi5.tx.HaveData = 1;
#ifdef SUP_DMA_SPI5_TX
        HAL_SPI_Transmit_DMA(&hspi5, (uint8_t *)&spi5_tx_buf[spi5.tx.Read], 1);
#else
        HAL_SPI_Transmit_IT(&hspi5, (uint8_t *)&spi5_tx_buf[spi5.tx.Read], 1);
#endif
        spi5.tx.Read = RBUF_NEXT_PT(spi5.tx.Read, 1, sizeof(spi5_tx_buf));
    }

}

extern void WaitSPI5TxBufEmpty(void)
{
    while(spi5.tx.HaveData) {       //米豕﹞⊿?赤那y?Y赤那㊣?
    }
}


extern void SPI5_CheckSerial(void)
{
    uint16_t len;
    uint8_t rxbuf[spi_rx_buf_Number];

    if(spi5.rx.busy != 1) {
        return;
    }

    len = SPI5_GetRxBufDat(rxbuf);

    if((rxbuf[0] == 0x47) && (len == TS_PACKET_SIZE)) 
   {
        //Led_Test();
       //  SPI5_Putchar(LAN_ACK);
       spi5.rx.rxcount +=TS_PACKET_SIZE;
		if(spi5.rx.rxcount >= spi_rx_buf_Number)
			spi5.rx.rxcount = 0;
		
#ifdef SUP_DMA_SPI5_RX
		HAL_SPI_Receive_DMA(&hspi5, (uint8_t*)&spi5_rx_buf[spi5.rx.rxcount], TS_PACKET_SIZE);
#else
		HAL_SPI_Receive_IT(&hspi5, (uint8_t*)&spi5_rx_buf[spi5.rx.rxcount], TS_PACKET_SIZE);
#endif
		 //USART3_Putchar(LAN_ACK);
    }

	//if(spi5.tx.HaveData == 0)
	//{	
	//}
 }

