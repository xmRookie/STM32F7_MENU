#include "common.h"

static u8_t LedCount;


extern void led_Par_Init(void)
{
    LedCount = 0;
}
/*
PG0->UserLEDB
PG1->UserLEDR
PG2->HDMI_IN_LEDG
PG3->HDMI_OUT_LEDG
PG4->HDSDI_IN_LEDG
PG5->HDSDI_OUT_LEDG
*/
extern void MX_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
//    __HAL_RCC_GPIOG_CLK_ENABLE();
	
    /*Configure GPIO pin : PB7 */
    /*Configure GPIO pins : PG0 PG1 PG2 PG3 PG4 PG5 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 , GPIO_PIN_SET);
	
	
		GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_14 ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    led_Par_Init();
}

extern void Led1_Set(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    }
}


extern void Led2_Set(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
    }
}


extern void Led_Test1(void)
{
    LedCount++;

    if(LedCount == 1) {
        Led1_Set(OFF_ST);
        Led2_Set(ON_ST);
    } else if(LedCount == 2) {
        Led1_Set(ON_ST);
        Led2_Set(OFF_ST);
        LedCount = 0;
    }
}

extern void UserLEDB(uint8_t status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, GPIO_PIN_RESET);
    }
}

extern void UserLEDR(uint8_t status)                            //test fpga_bad
{
    if(status) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1, GPIO_PIN_RESET);
    }
}

extern void HDMI_IN_LEDG(uint8_t status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}

extern void HDMI_OUT_LEDG(uint8_t status)                        //test     master(video fomat) / slave(sync slave)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);
    }
}

extern void HDSDI_IN_LEDG(uint8_t status)                          //test    cpu_up
{
    if(status) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);
    }
}

extern void HDSDI_OUT_LEDG(uint8_t status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

extern void Led_Test(void)
{
    LedCount++;

    if(LedCount == 1) {
        UserLEDB(OFF_ST);
     //   UserLEDR(ON_ST);
    } else if(LedCount == 2) {
        UserLEDB(ON_ST);
     //   UserLEDR(OFF_ST);
        LedCount = 0;
    }
}


