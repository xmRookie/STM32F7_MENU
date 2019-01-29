#include "common.h"


#define SYSTEM_Y 2

//软件版本号
#define sw_version_yearhi    0x20
#define sw_version_yearlo    0x19
#define sw_version_month     0x01
#define sw_version_day       0x27
#define sw_version_dayfrag   0xc2

#define fw_version_len 10
#define sw_version_len 10

extern uint8_t En_Or_Dis[En_Or_Dis_NUM];


uint8_t sw[5] = {sw_version_yearhi,sw_version_yearlo,sw_version_month,sw_version_day,sw_version_dayfrag};   //sw版本号  十六进制  传给 qt


uint8_t version_char[sw_version_len];   //版本号  10个字符  缓存         sw和fw都是10个字符

//uint8_t sw_version[sw_version_len+1]={0};         //需要传给qt    char
//uint8_t fw_version[fw_version_len+1]={0};         //需要传给qt



/*************************************************
*函数介绍： Get_SW  获取软件版本号 
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern uint8_t* Get_SW(void)
{
	return sw;
}

/*************************************************
*函数介绍： 读取硬件版本号： HW :    返回hex数值  
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern uint8_t Get_HW(void)
{
	//uint8_t HW[8]={0};
	uint8_t hw=0;
	/*
	//高4位
	HW[0] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);   //H
	HW[1] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
	HW[2] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
	HW[3] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);   //L
	//低4位
	HW[4] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12);   //H
	HW[5] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13);
	HW[6] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14);
	HW[7] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15);   //L
	*/
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12); 
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
	
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12); 
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15);
	
	return hw;
}

//
/*************************************************
*函数介绍： hw  转成 char 型
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
uint8_t* hw_0x_to_char(uint8_t hw_version)
{
	version_char[0] = hw_version / 16 + 0x30;
	version_char[1] = hw_version % 16 + 0x30;
	
	//version_char[2] = 0;   //赋值0 ，防止oled显示出错
	
	return version_char;
}

//
/*************************************************
*函数介绍： fw sw  转成 char 型
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
uint8_t* _0x_to_char(uint8_t *pstr_fw)
{
	uint8_t i=0;
//	pstr_fw = Get_FW();    //获取Fw版本号   十六进制 如0x20 0x18 0x12 0x11 0xb3 --> "20181211B3"
	
	if(pstr_fw != NULL){   //转成char   存到version_char[]数组里
		version_char[0] = pstr_fw[0] / 16 + 0x30;
		version_char[1] = pstr_fw[0] % 16 + 0x30;
		
		version_char[2] = pstr_fw[1] / 16 + 0x30;
		version_char[3] = pstr_fw[1] % 16 + 0x30;
		
		version_char[4] = pstr_fw[2] / 16 + 0x30;
		version_char[5] = pstr_fw[2] % 16 + 0x30;
		
		version_char[6] = pstr_fw[3] / 16 + 0x30;
		version_char[7] = pstr_fw[3] % 16 + 0x30;
		
		version_char[8] = pstr_fw[4] / 16 + 0x30;
		version_char[9] = pstr_fw[4] % 16 + 0x30;
		
		for(i=0; i<fw_version_len; i++){
			if(version_char[i] > 0x39)
				version_char[i] += 7;         //如果是字母，转成字母的ascii
		}
		
		return version_char;
	}else{
		printf(" version get error !\r\n");
		return NULL;
	}

}
/*************************************************
*函数介绍： 显示  SW  HW  FW 版本号               HW:Vx.x
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern void Display_Version(void)
{
	char pstr_data[4][Display_Line_Width]={"SW:","HW:V","FW:","Exit"};
	uint8_t *version = NULL;
	uint8_t *fw_pstr = NULL;
	uint8_t hw_version;
	uint8_t dot = '.';
	
	version = _0x_to_char(sw);    			//获取sw版本号  char型
	if(version != NULL)
		strncat(pstr_data[0], version, sw_version_len);    //填写sw版本号
	
	hw_version = Get_HW();    //获取hw版本号  hex型
	version = hw_0x_to_char(hw_version);     //转成char型
	strncat(pstr_data[1], &version[0], 1); 
	strncat(pstr_data[1], &dot, 1); 
	strncat(pstr_data[1], &version[1], 1); 
	
	
	fw_pstr = Get_FW();//获取5个hex数值
	version = _0x_to_char(fw_pstr);    //获取fw版本号  char型
	if(version != NULL)
		strncat(pstr_data[2], version, fw_version_len);    //填写fw版本号             
	
	
	OLED_ShowString(0,1,pstr_data[0],12);
	OLED_ShowString(0,2,pstr_data[1],12);
	OLED_ShowString(0,3,pstr_data[2],12);
//	printf(" pstr_fw %s ",pstr_fw);      // 0x20 0x18 0x12 0x11 0xb3

}
/*************************************************
*函数介绍： 复位动作  
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern void Restart_Action(void)
{
	ReStart();       //iwdg.c
}
/*************************************************
*函数介绍： 恢复工厂设置 并复位 
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern void Restore_Action(void)
{
	Restore_Flash();
	ReStart();	
}
/*************************************************
*函数介绍： 更新动作   暂无
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern void Update_Action(void)
{
	char pstr_data[2][Display_Line_Width]={"FACTORY ONLY","Exit"};
	OLED_ShowString(0,SYSTEM_Y,pstr_data[0],12);
}





