#include "common.h"

#define DEBUG  1

UART_HandleTypeDef huart2;

#define   RXTIMEOUT             10             // 接收超时时间,如10mSec内未接收任何数据,当作一个接收包

#define   usart2_tx_buf_Number  16//0x400         // ·￠?í?o3???′óD?,±?D??a2μ?n′?·?
#define   usart2_rx_buf_Number  16//0x400          // ?ó?ó?o3???′óD?,±?D??a2μ?n′?·?


uint16_t Spi_Trans_Count;
uint8_t Spi_Trans_Flag;

//extern  uint8_t Fpga_Write_Data[8];
//extern  uint8_t Fpga_Read_Data[8];
extern uint8_t Write_Data[16];
extern uint8_t Read_Data[16];


typedef struct {
    uint16_t                 Write;
    volatile uint16_t        Read;
    volatile Boolean        HaveData;
} UART_TX_T;

typedef struct {
    volatile uint16_t        rxcount;
		volatile uint16_t       Read;
		volatile uint16_t       Write;
    volatile uint8_t        timeout;
    Boolean                 busy;
} UART_RX_T;

typedef struct {
    UART_TX_T               tx;
    UART_RX_T               rx;
    uint8_t                 AutoChgBRageTimeOut;
    Boolean                 NotIn9600;
} UART_T;

static UART_T usart2;
static uint8_t usart2_tx_buf[usart2_tx_buf_Number];
static uint8_t usart2_rx_buf[usart2_rx_buf_Number];

/* USART2 init function */
extern void MX_USART2_UART_Init(void)
{

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    //huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    //huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if(HAL_UART_Init(&huart2) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
		Spi_Trans_Count=0;
		Spi_Trans_Flag=0;
		memset(&usart2, 0, sizeof(usart2));
		
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&usart2_rx_buf[usart2.rx.rxcount], 1);
		
	//usart2.rx.rxcount = RBUF_NEXT_PT(usart2.rx.rxcount, 1, usart2_rx_buf_Number);
}


extern void USART2_Rx_Deal(void )
{
	static uint8_t i=0;
	  if(usart2.rx.busy == 0) {     
			
       usart2.rx.timeout = 0;
			
				usart2.rx.rxcount = RBUF_NEXT_PT(usart2.rx.rxcount, 1, usart2_rx_buf_Number);	
			
       HAL_UART_Receive_IT(&huart2, (uint8_t *)&usart2_rx_buf[usart2.rx.rxcount], 1); //??D?′ò?a′??ú?D??
			 
			 
     }
}
	

extern void USART2_Tx_Deal(void )
{
	
				if(usart2.tx.Write == usart2.tx.Read) {
            // êy?Yò??-·￠?ííê
			//		Led1_Set(ON_ST);
            usart2.tx.HaveData = 0;
        } else {
            HAL_UART_Transmit_IT(&huart2, (uint8_t *)&usart2_tx_buf[usart2.tx.Read], 1); //??D?′ò?a′??ú?D??
					
           usart2.tx.Read = RBUF_NEXT_PT(usart2.tx.Read, 1, sizeof(usart2_tx_buf));
			
						
        }
	
	
}

extern void USART2_Putchar(uint8_t ch)
{
    uint16_t head;
    head = RBUF_NEXT_PT(usart2.tx.Write, 1, sizeof(usart2_tx_buf));
    while(head == usart2.tx.Read) { //?ú
			printf("error");
    }
    usart2_tx_buf[usart2.tx.Write] = ch;
    usart2.tx.Write = head;
		
    if(usart2.tx.HaveData == 0) {
        // ·￠?í?1??óD???ˉ
        usart2.tx.HaveData = 1;
        if( HAL_UART_Transmit_IT(&huart2, (uint8_t *)&usart2_tx_buf[usart2.tx.Read], 1) != HAL_OK){
					printf("transmit error");
				}			
        usart2.tx.Read = RBUF_NEXT_PT(usart2.tx.Read, 1, sizeof(usart2_tx_buf));
    }

}

extern void USART2_Putbuf(uint8_t const *buf, uint16_t len)
{
    for(; len ; len--) {
        USART2_Putchar(*buf++);
    }
}

extern void USART2_RxReaddy(void)
{
    if(usart2.rx.busy == 0) {
        if(usart2.rx.rxcount) {
            usart2.rx.timeout++;

            if(usart2.rx.timeout > RXTIMEOUT) {
                usart2.rx.busy = 1;
            }
        }
    }
}


static uint16_t USART2_GetRxBufDat(uint8_t *buf)
{
    uint16_t len;
		uint8_t err;

    len = usart2.rx.rxcount;
		memcpy(buf, usart2_rx_buf, usart2.rx.rxcount);
    usart2.rx.rxcount = 0;
	
		huart2.RxState = HAL_UART_STATE_READY;    //Must be forced Preparation , Otherwise the next sentence is useless
	
		err = HAL_UART_Receive_IT(&huart2, (uint8_t *)&usart2_rx_buf[usart2.rx.rxcount], 1);
		if(err != HAL_OK){
			USART2_Putchar(err);
			printf("get err");
		}
		//printf("usart2.rx.rxcount %d \r\n ",usart2.rx.rxcount);
    usart2.rx.timeout = 0;
    usart2.rx.busy = 0;

    return len;
}


extern void  pollint_spi_transmit()
{
	uint16_t len;
	uint8_t i;
	static uint8_t retry_times=0;
//	USART2_RxReaddy();
//	if(usart2.rx.busy != 1) {
//   return;
//	}
	
	len = USART2_GetRxBufDat(&Write_Data[Spi_Trans_Count]);
	
	if(len){
/*	
		for(i=0;i<len;i++){
			printf(" buf[%d] %x ",i,Write_Data[i]);
		}
		printf("\r\nrxcount %d len %d Read %d Write %d\r\n ",usart2.rx.rxcount,len,usart2.tx.Read,usart2.tx.Write);
*/
		for( ; len ; len--){
			Spi_Trans_Count++;
			if(Spi_Trans_Count == 3){
				Spi_Trans_Count=0;
				Test_Send_To_Fpga();	         ///!!!!!!!!!!!!!!!!!!
				break;
			}
		}
		retry_times=0;
	}else if(Get_Retry_SPI4_Flag()){
		if(retry_times==0)
			Retry_SPI4();
		retry_times=1;
	}

}
	


#if 0
extern void  pollint_spi_transmit()
{
	
	uint8_t spi_trans_count = 0;
	
	if( usart2.rx.rxcount != 0){
		usart2.rx.Write = usart2.rx.rxcount - 1 ;
	}else{
		usart2.rx.Write = usart2_rx_buf_Number-1 ; 
	}

	if((usart2.rx.Write - usart2.rx.Read) < 2 && (usart2.rx.Write - usart2.rx.Read) >= 0 ){  //未发送的数据小于3  或者 数据已全部发送  就不发 
		return;
	}else{
		for( spi_trans_count = 0; spi_trans_count<3; spi_trans_count++){
			Fpga_Write_Data[spi_trans_count] = usart2_rx_buf[usart2.rx.Read];
			usart2.rx.Read = RBUF_NEXT_PT(usart2.rx.Read, 1, usart2_rx_buf_Number);
		}
		SPI1_Communicate();	
	}
	
}
#endif

