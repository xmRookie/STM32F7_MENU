/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "lwip.h"
#include "common.h"


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);



static void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}


void HardWare_Init(void)
{
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
	
    MX_DMA_Init();

	
	
		MX_USART2_UART_Init();   ////MX_USART2_UART_Init
    MX_USART3_UART_Init();

   romflash_dual_mode();

		Read_Flash_IP_MAC();
    config_env_init();
		MX_LWIP_Init();          //1
	
//	MX_ethernetif_init();   //2  重复了
#ifdef EN_TIM3
    MX_TIM3_Init();
#endif
		
    //MX_I2C1_Init();
    //MX_I2C3_Init();
    //MX_I2C2_Init();
    //MX_I2C4_Init();
	
  //  MX_SPI1_Init();
    MX_SPI4_Init();
  //  MX_SPI5_Init();

  //  MX_I2S2_Init();
		
		GPS_PPS_Init();                 //gps pps中断和oled显示可能会冲突吧    须检测
		
		
		  
    //MX_I2S3_Init();

		

		MX_UART4_Init();

    MX_LED_Init();

    MX_KEY_Init();
		
	//Read_Flash();            //！！！！！！！！！
			ReadFlash2Arr();
			
	MX_IWDG_Init();
	
	MX_RTC_Init();         //rtc
	//
	//Display_Init();
	
	
	
	OLED_Init();
	OLED_Clear();
	
	menuInit();
	
}


int main(void)
{
	
    STR_EVENT event;
	  /* Configure the MPU attributes as Device memory for ETH DMA descriptors */
	
    CPU_CACHE_Enable();
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    HardWare_Init();
    ClearAllEvent();
    SetWorkMode(MODE_ON);
    //tcp_echoserver_init();
    udp_echoclient_connect();

    tsMux_init();
#ifdef EXT_OSC_HSE
	printf("EXT_OSC_HSE\r\n");
#endif	
	
    printf("version is %s\r\n", VERSION);

		HDSDI_IN_LEDG(1);     //cpu_up
#ifdef FPGA_LINK    //先和fpga通信，判断fpga是否run
		Set_Status(0);    //good
#else
		Set_Status(1);    //no fpga link
#endif



#if 0
	Send_FPGA_Restart(0x10);     //restart 0x10
	delay_ms(1000);
	//Send_FPGA_Restart();            //run 0x00;
	/*
	while(Get_Retry_SPI4_Flag()){
		Reset_FPGA();
	}
	*/
#endif 
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1) {
        pollint();
		pollintTim3();
        ReadEvent(&event);

        if(event.evt != evNON) {
            switch(GetWorkMode()) {
            case MODE_ON:
                break;

            case MODE_REG:
                break;

            case MODE_ERR:
                break;

            case MODE_OFF:
                break;

            case MODE_RESET:
                powerReset(&event);
                break;
			case MODE_KEY:
				ModeKey(&event);
				break;
            default:
                break;
            }
        }
    }
}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
#ifdef EXT_OSC_HSE
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState =    RCC_HSE_ON;   //   RCC_HSE_BYPASS;// 
		//RCC_OscInitStruct.LSEState = RCC_LSE_BYPASS;  //RCC_LSE_ON;  // RCC_LSE_BYPASS
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 432;
#else
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.LSIState = RCC_LSI_ON;  //RTC_LSI
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 216;
#endif
    RCC_OscInitStruct.PLL.PLLQ = 9;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;

    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Activate the Over-Drive mode
    */
    if(HAL_PWREx_EnableOverDrive() != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3
            | RCC_PERIPHCLK_USART6 | RCC_PERIPHCLK_UART4
            | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2
            | RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_I2C4
						| RCC_PERIPHCLK_RTC      //RTC  
#ifdef EXT_OSC_HSE
            | RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
#else
            | RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 96;
#endif
    PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
    PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
    PeriphClkInitStruct.PLLI2SDivQ = 1;
		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV2;    //RCC_RTCCLKSOURCE_HSE_DIV8;   //RTC  HSE DIV8   3.125MHZ  for 1s
	//PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI; //RTC LSI 32K
  //PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; //RTC LSE 32.768K
    PeriphClkInitStruct.I2sClockSelection = RCC_I2SCLKSOURCE_PLLI2S;
    PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
    PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
    PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
#ifdef EXT_OSC_HSE
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
#endif

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
	
	   GPIO_InitTypeDef GPIO_InitStruct;
   
	/*Configure GPIO pin : PC6--S0     PC7--S1     PC8--S2      PC9--S3 */    //fpga 时钟芯片
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);   //0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);      //1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);     //1
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);  //0
	
	
	
	
	//硬件版本号HW：高4位 PIN -->    H3:PB12    H2:PB13   H1:PB14    H0:PB15
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//硬件版本号 HW：PIN -->  低4位    L3:PD12    L2:PD13   L1:PD14    L0:PD15
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
	 /*Configure GPIO pin : PG5 */
#if 1 
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;  //GPIO_MODE_IT_FALLING;//
  GPIO_InitStruct.Pull = GPIO_PULLUP;  //GPIO_PULLDOWN;//
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
		HAL_NVIC_SetPriority(EXTI9_5_IRQn, 3, 3);
     HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
#endif
	
/*Configure GPIO pin : PH6--SDI_RC_CD??PH7--SDI_TX_EQ??PH8--SDI_TX_SD??PH9--SDI_TX_DSAB??PH10--SDI_TX_SP??PH11--SDI_TX_OP_CTL??PH12--SDI_RX_SO_ADJ??PH13--SDI_RX_BYPS??PH14--SDI_RX_GS??PG15--SDI_RX_SLP */
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_RESET);    //SDI_TX_EQ = 0
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_8, GPIO_PIN_RESET);    //SDI_TX_SD = 0
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_9, GPIO_PIN_SET);      //SDI_TX_DSAB = 1
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);     //SDI_TX_OP_CTL = 0
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_13, GPIO_PIN_RESET);    //SDI_RX_BYPS = 0
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_14, GPIO_PIN_RESET);    //SDI_RX_GS = 0 
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15, GPIO_PIN_RESET);    //SDI_RX_SLP = 0

}

#ifdef EXT_OSC_HSE
/* USB_OTG_FS init function */
static void MX_USB_OTG_FS_USB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin : PA9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PA10 PA11 PA12 */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
    printf("error %s  ;line %d\n", file, line);

    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1) {
    }

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
