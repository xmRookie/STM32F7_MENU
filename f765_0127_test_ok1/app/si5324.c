#include "common.h"

extern I2C_HandleTypeDef hi2c3;

const uint8_t SI5324_I2Caddr = 0x68;  //CMODE=0 The I2C address is 1101 [A2] [A1] [A0]

static void SI5324_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	
  /*Configure GPIO pins : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	
	/*Configure GPIO pins : PA11 PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

void Si5324_RST(Boolean status)
{
    if(status) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    }
}
//*****************************************************************************
uint8_t si5324addr;

void SI5324_slave_addr(uint8_t addr)
{
   si5324addr = addr;
}

void SI5324_WriteReg(uint8_t si5324addr,uint8_t regaddr, uint8_t *data,uint8_t n)
{
  HAL_I2C_Mem_Write(&hi2c3, si5324addr, regaddr, 1, data, n, 50);
}

void SI5324_ReadReg(uint8_t si5324addr, uint8_t regaddr,uint8_t *data,uint8_t n)
{
   HAL_I2C_Mem_Read(&hi2c3, si5324addr,regaddr , 1 , data, n, 50);
}

void SI5324_read_write(uint8_t reg, uint8_t data)
{
  uint8_t readdata;
	SI5324_WriteReg(si5324addr,reg,&data,1);
  //SI5324_ReadReg(si5324addr,reg,&readdata,1);
}

//**********************************mode****************************************
/******************************************************************************
# Part number: Si5324
#END_HEADER
#PROFILE
# Name: Si5324
#INPUT
# Name: CKIN
# Channel: 1
# Frequency (MHz): 19.440000
# N3: 875
# Maximum (MHz): 22.148437
# Minimum (MHz): 18.945312
#END_INPUT
#INPUT
# Name: CKIN
# Channel: 2
# Frequency (MHz): 114.284248
# N3: 5144
# CKIN2 to CKIN1 Ratio: 14285531 / 2430000
# Maximum (MHz): 130.207500
# Minimum (MHz): 111.376785
#END_INPUT
#PLL
# Name: PLL
# Frequency (MHz): 4976.640000
# XA-XB Frequency (MHz): 114.285000
# f3 (MHz): 0.022217
# N1_HS: 10
# N2_HS: 10
# N2_LS: 22400
# Phase Offset Resolution (ns): 2.00939
# BWSEL_REG Option: Frequency (Hz)
#  4:    6
#  3:   13
#  2:   27
#  1:   56
#END_PLL
#OUTPUT
# Name: CKOUT
# Channel: 1
# Frequency (MHz): 248.832000
# NC1_LS: 2
# CKOUT1 to CKIN1 Ratio: 64 / 5
# Maximum (MHz): 283.499993
# Minimum (MHz): 242.499993
#END_OUTPUT
#CONTROL_FIELD
**************************************************************************/
static void SI5324_4k(void)    //3840x2160x30Hz  248.832MHz
{
	SI5324_slave_addr(SI5324_I2Caddr);
	SI5324_read_write(0x00 ,0x54 );		//0
	SI5324_read_write(0x01 ,0xE4 );		//1
	SI5324_read_write(0x02 ,0x42 );		//2
	SI5324_read_write(0x03 ,0x15 );		//3
	SI5324_read_write(0x04 ,0x92 );		//4	
	SI5324_read_write(0x05 ,0xED );		//5
	SI5324_read_write(0x06 ,0x2D );		//6
	SI5324_read_write(0x07 ,0x2A );		//7
	SI5324_read_write(0x08 ,0x00 );		//8
	SI5324_read_write(0x09 ,0xC0 );		//9
	SI5324_read_write(0x0A ,0x08 );		//10
	SI5324_read_write(0x0B ,0x40 );		//11
	SI5324_read_write(0x13 ,0x29 );		//19
	SI5324_read_write(0x14 ,0x3E );		//20
	SI5324_read_write(0x15 ,0xFF );		//21
	SI5324_read_write(0x16 ,0xDF );		//22
	SI5324_read_write(0x17 ,0x1F );		//23
	SI5324_read_write(0x18 ,0x3F );		//24
	SI5324_read_write(0x19 ,0xC0 );		//25
	SI5324_read_write(0x1F ,0x00 );		//31
	SI5324_read_write(0x20 ,0x00 );		//32
	SI5324_read_write(0x21 ,0x01 );		//33
	SI5324_read_write(0x22 ,0x00 );		//34
	SI5324_read_write(0x23 ,0x00 );		//35
	SI5324_read_write(0x24 ,0x01 );		//36
	SI5324_read_write(0x28 ,0xC0 );		//40
	SI5324_read_write(0x29 ,0x57 );		//41
	SI5324_read_write(0x2A ,0x7F );		//42
	SI5324_read_write(0x2B ,0x00 );		//43
	SI5324_read_write(0x2C ,0x03 );		//44
	SI5324_read_write(0x2D ,0x6A );		//45
	SI5324_read_write(0x2E ,0x00 );		//46
	SI5324_read_write(0x2F ,0x14 );		//47
	SI5324_read_write(0x30 ,0x17 );		//48
	SI5324_read_write(0x37 ,0x00 );		//55
	SI5324_read_write(0x83 ,0x1F );		//131
	SI5324_read_write(0x84 ,0x02 );		//132
	SI5324_read_write(0x89 ,0x01 );		//137
	SI5324_read_write(0x8A ,0x0F );		//138
	SI5324_read_write(0x8B ,0xFF );		//139
	SI5324_read_write(0x8E ,0x00 );		//142
	SI5324_read_write(0x8F ,0x00 );		//143
	SI5324_read_write(0x88 ,0x40 );		//136
	
}

//******************************************************************************
extern void Si5324_Test(void)  //si5324 main
{
	const char buf_4k[]={"si5324 set 248.832MHz"};
	SI5324_Init();
	SI5324_4k();	
	USART3_Putbuf(buf_4k,strlen(buf_4k));
}




