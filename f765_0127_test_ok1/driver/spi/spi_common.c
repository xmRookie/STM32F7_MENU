#include "common.h"


extern void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI1) {
		SPI1_RxCpltCallback();
	}
	
	if(hspi->Instance == SPI5) {
		SPI5_RxCpltCallback();
	}

    //接收到数据

}

extern void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
		
		if(hspi->Instance == SPI1)
		{
			SPI1_TxCpltCallback();
		}
		
		if(hspi->Instance == SPI5)
		{
			SPI5_TxCpltCallback();
		}
}


extern  void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
		if(hspi->Instance == SPI5)
		{
			SPI5_TxRxCpltCallback();
		}

		
		if(hspi->Instance == SPI1)
		{
			SPI1_TxRxCpltCallback();
		}
		
		if(hspi->Instance == SPI4)
		{
			SPI4_TxRxCpltCallback();
		}
		
}

