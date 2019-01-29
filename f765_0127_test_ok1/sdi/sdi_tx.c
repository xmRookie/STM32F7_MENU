/****************************************************
*name:sdi_tx.c(gs6080)
*pin:	SDI_TX_EQ		--->	PH7		(OUT)
*			SDI_TX_SD		--->  PH8		(OUT)
*			SDI_TX_DSAB	--->	PH9		(OUT)
*			SDI_TX_SP		--->	PH10	(IN)
***************************************************/
#include "common.h"

extern void TX_SDI_Init(void)
{
		GPIO_InitTypeDef GPIO_SDITX_InitStruct;
		/* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 , GPIO_PIN_RESET);

    /*Configure GPIO pins : PH7 PH8 PH9 */
    GPIO_SDITX_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_SDITX_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_SDITX_InitStruct.Pull = GPIO_NOPULL;
    GPIO_SDITX_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_SDITX_InitStruct);
	
		__HAL_RCC_GPIOH_CLK_ENABLE();
	
		/*Configure GPIO pin : PH10 */
		GPIO_SDITX_InitStruct.Pin = GPIO_PIN_10;
    GPIO_SDITX_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_SDITX_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOH, &GPIO_SDITX_InitStruct);

}
/**********************************************/
extern void SDI_TX_EQ(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_RESET);
    }
}

extern void SDI_TX_SD(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_RESET);
    }
}

extern void SDI_TX_DSAB(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_9, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_9, GPIO_PIN_RESET);
    }
}

/*******************************************************/
extern void sdi_tx(void)
{
		if(SDI_TX_SP==0)
		{
				SDI_TX_SD(OFF_ST);
				SDI_TX_EQ(ON_ST);
				SDI_TX_DSAB(ON_ST);
		}
		else
				SDI_TX_DSAB(OFF_ST);
}