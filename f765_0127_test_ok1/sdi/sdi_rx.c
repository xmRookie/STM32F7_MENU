/****************************************************
*name:sdi_rx.c(gs6042)
*pin:	SDI_RX_CD			--->	PH6		(IN)
*			SDI_RX_OP_CTL	--->  PH11	(OUT)
*			SDI_RX_SO_ADJ	--->	PH12	(OUT)
*			SDI_RX_BYPS		--->	PH13	(OUT)
*			SDI_RX_GS			--->	PH14	(OUT)
*			SDI_RX_SLP		--->	PH15	(OUT)
***************************************************/
#include "common.h"

extern void RX_SDI_Init(void)
{
		GPIO_InitTypeDef GPIO_SDIRX_InitStruct;
		/* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 , GPIO_PIN_RESET);

    /*Configure GPIO pins : PH11 PH12 PH13 PH14 PH15 */
    GPIO_SDIRX_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 ;
    GPIO_SDIRX_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_SDIRX_InitStruct.Pull = GPIO_NOPULL;
    GPIO_SDIRX_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_SDIRX_InitStruct);
	
		__HAL_RCC_GPIOH_CLK_ENABLE();
	
		/*Configure GPIO pin : PH6 */
		GPIO_SDIRX_InitStruct.Pin = GPIO_PIN_6;
    GPIO_SDIRX_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_SDIRX_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOH, &GPIO_SDIRX_InitStruct);

}

/**********************************************/
extern void SDI_RX_OP_CTL(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);
    }
}

extern void SDI_RX_SO_ADJ(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_RESET);
    }
}

extern void SDI_RX_BYPS(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}

extern void SDI_RX_GS(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_14, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_14, GPIO_PIN_RESET);
    }
}

extern void SDI_RX_SLP(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15, GPIO_PIN_RESET);
    }
}

/*******************************************************/
extern void sdi_rx(void)
{
		if(SDI_RX_CD==0)
				SDI_RX_SLP(ON_ST);
		else
				SDI_RX_SLP(OFF_ST);
		
		SDI_RX_GS(OFF_ST);
		SDI_RX_BYPS(OFF_ST);
		SDI_RX_SO_ADJ(OFF_ST);
		SDI_RX_OP_CTL(OFF_ST);
}