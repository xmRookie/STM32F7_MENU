//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(STM32F4系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PG12（SCL）
//              D1   接PD5（SDA）
//              RES  接随开发板复位工作了
//              DC   接PD15
//              CS   接PD1               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//  生成日期   : 2018-0101
//All rights reserved
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------测试LED端口定义---------------- 


//-----------------OLED端口定义----------------  					   


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_SET)

//#define OLED_SDIN_READ() HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_1)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_12,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_12,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);

extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);
void OLED_ShowChar2(u8 x,u8 y,u8 chr,u8 size);
extern void OLED_ShowChar_2(u8 x,u8 y,u8 chr,u8 size);

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size);
void OLED_ShowString2(u8 x,u8 y,u8 *chr,u8 size);

void OLED_ShowString_2(u8 x,u8 y,u8 *chr,u8 size);  //反白

extern void OLED_ShowString_Size22(u8 x,u8 y,u8 *chr,u8 size);

void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



