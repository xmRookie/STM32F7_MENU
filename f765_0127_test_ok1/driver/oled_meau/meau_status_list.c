#include "common.h"

//extern uint8_t Display_Line[][Display_Line_Width];  
//extern CURRENT_DISPLAY Cur_Dis;

#define STATUS_Y_CURSOR 2
#define STATUS_SIZE 16

static uint8_t Reg_0x8d_num;
static uint8_t Reg_0x84_num;
static uint8_t Reg_0x83_num;

/*************************************************
*函数介绍： 获取fpga Reg_0x83值
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t return_Reg_0x83_num(void)
{
	return Reg_0x83_num;
}
/*
*函数介绍： 获取fpga Reg_0x84值
*输入参数： 
*输出参数：
*返回值  ：
*/
extern uint8_t return_Reg_0x84_num(void)
{
	return Reg_0x84_num;
}
/*************************************************
*函数介绍： 读取fpga Reg_0x83值
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t pollint_Reg_0x83(void)   
{
	uint8_t Status , Num = 0;
	
	Reg_0x83_num = 	Get_Reg_0x83();

	return Reg_0x83_num;
}
/* ************************************************
*函数介绍： 读取fpga Reg_0x84值   判断 SDI是否有视频流？
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t pollint_Reg_0x84(void)    //判断 SDI是否有视频流？
{
	uint8_t Status , Num = 0;
	
	Reg_0x84_num = 	Get_Reg_0x84();

	Num = (Reg_0x84_num & 0x10)>> 4;            
	
	//printf(" 0x%x \r\n",Num);
	
	if(Num == 1){
		Status = 1;        //vid link
	}else{ 
		Status = 0;        //vid no link
	}
	
	Set_vid_link_flag(Status);
	
	return Status;
}

/*************************************************
*函数介绍： 读取fpga Reg_0x82值   判断 status状态位，控制error灯亮灭
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t pollint_Reg_0x82(void)     //判断 status状态位，控制error灯亮灭
{
	uint8_t Status , Num = 0;
	
	Num = Get_Reg_0x82();
	Num <<= 3;             //左移3位 
	
//	printf(" 0x%x \r\n",Num);
	
	if(Num == 0xf8){
		Status = 0;        //good
	}else{ 
		Status = 1;        //error
	}
	
	Set_Status(Status);
	
	return Status;
}
/*************************************************
*函数介绍： 读取fpga Reg_0x87值   判断 type-c接口  是否有线连接     有线 status亮灯   没有线  status灭灯
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t pollint_Reg_0x87(void)      //判断 type-c接口  是否有线连接     有线 status亮灯   没有线  status灭灯
{
	uint8_t Num = 0;
	
	Num = Get_Reg_0x87();
	Num <<= 7;            
	
	//printf(" 0x%x \r\n",Num);
	
	if(Num == 0x80){       //status状态灯
		HDMI_OUT_LEDG(1);
	}else{
		HDMI_OUT_LEDG(0);
	}
	
	
	return Num;
}
/*************************************************
*函数介绍： 读取fpga Reg_0x8d值 
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t pollint_Reg_0x8d(void)
{
	Reg_0x8d_num = Get_Reg_0x8d();
}
/*************************************************
*函数介绍： 获取fpga Reg_0x8d值 
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t return_Reg_0x8d_num(void)
{
	return Reg_0x8d_num;
}
/*************************************************
*函数介绍： 显示Video_Format状态
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern void Display_Video_Format(void)
{
	char pstr_data[2][Display_Line_Width]={"","<Exit"};   //No_Detect
	char _Auto[]="720p60Hz";
	char _720p50Hz[]="720p50Hz";
	char _720p59Hz[]="720p59Hz";
	char _720p60Hz[]="720p60Hz";
	char _1080i50Hz[]="1080i50Hz";
	char _1080i59Hz[]="1080i59Hz";
	char _1080i60Hz[]="1080i60Hz";
	char _1080p23Hz[]="1080p23Hz";
	char _1080p24Hz[]="1080p24Hz";
	char _1080p25Hz[]="1080p25Hz";
	char _1080p29Hz[]="1080p29Hz";
	char _1080p30Hz[]="1080p30Hz";
	char _1080p50Hz[]="1080p50Hz";
	char _1080p59Hz[]="1080p59Hz";
	char _1080p60Hz[]="1080p60Hz";
	char _1080pSF23Hz[]="1080pSF23Hz";
	char _1080pSF24Hz[]="1080pSF24Hz";
	uint8_t Bottom_Line = 2 - 1;
	uint8_t status_flag = 0;
	uint8_t test_pat=0;
	uint8_t _0x83;
	uint8_t _0x84;
	uint8_t temp;
	
	status_flag = Get_Config_Format();    //判断显示哪种状态
	test_pat = Get_Config_Video_Input();  //如果当前是test_pat状态下
	printf(" test_pat %d \r\n",test_pat );
	if(test_pat == 3){
		if(status_flag == 1)
			memcpy(pstr_data[0],_Auto,sizeof(_Auto));
		else if(status_flag == 2)
			memcpy(pstr_data[0],_720p50Hz,sizeof(_720p50Hz));
		else if(status_flag == 3)
			memcpy(pstr_data[0],_720p59Hz,sizeof(_720p59Hz));
		else if(status_flag == 4)
			memcpy(pstr_data[0],_720p60Hz,sizeof(_720p60Hz));
		else if(status_flag == 5)
			memcpy(pstr_data[0],_1080i50Hz,sizeof(_1080i50Hz));
		else if(status_flag == 6)
			memcpy(pstr_data[0],_1080i59Hz,sizeof(_1080i59Hz));
		else if(status_flag == 7)
			memcpy(pstr_data[0],_1080i60Hz,sizeof(_1080i60Hz));
		else if(status_flag == 8)
			memcpy(pstr_data[0],_1080p23Hz,sizeof(_1080p23Hz));
		else if(status_flag == 9)
			memcpy(pstr_data[0],_1080p24Hz,sizeof(_1080p24Hz));
		else if(status_flag == 10)
			memcpy(pstr_data[0],_1080p25Hz,sizeof(_1080p25Hz));
		else if(status_flag == 11)
			memcpy(pstr_data[0],_1080p29Hz,sizeof(_1080p29Hz));
		else if(status_flag == 12)
			memcpy(pstr_data[0],_1080p30Hz,sizeof(_1080p30Hz));
		else if(status_flag == 13)
			memcpy(pstr_data[0],_1080p50Hz,sizeof(_1080p50Hz));
		else if(status_flag == 14)
			memcpy(pstr_data[0],_1080p59Hz,sizeof(_1080p59Hz));
		else if(status_flag == 15)
			memcpy(pstr_data[0],_1080p60Hz,sizeof(_1080p60Hz));
		else if(status_flag == 16)
			memcpy(pstr_data[0],_1080pSF23Hz,sizeof(_1080pSF23Hz));
		else if(status_flag == 17)
			memcpy(pstr_data[0],_1080pSF24Hz,sizeof(_1080pSF24Hz));
	}else if(test_pat == 2 ){        //需要从寄存器0x83和0x84读值  以判断当前视频格式
			_0x83 = return_Reg_0x83_num();
			_0x84 = return_Reg_0x84_num();
		  temp = _0x83 & 0x0f;
			switch(temp){
				case 0x00:   memcpy(pstr_data[0],"1920x1080",9); //1920x1080
					break;
				case 0x01:   memcpy(pstr_data[0],"1280x720",8); //1280x720
					break;
				case 0x02:   memcpy(pstr_data[0],"2048x1080",9); //2048x1080	
					break;
				case 0x03:   memcpy(pstr_data[0],"1920x1080",9); //1920x1080
					break;
				case 0x08:   memcpy(pstr_data[0],"720x486",7); //720x486
					break;
				case 0x09:   memcpy(pstr_data[0],"720x576",7); //720x576
					break;
				case 0x0f:   memcpy(pstr_data[0],"No_Detect",9); 
					break;
				default:     //memcpy(pstr_data[0],"No_Detect",9); 
					break;
			}
			if(temp != 0x0f){
				temp = _0x83 & 0x10;
				if(temp){   //progressive
					strcat(pstr_data[0],"p");
				}else{      //interlaced
					strcat(pstr_data[0],"i");
				}
				temp = _0x84 & 0x0f;
				switch(temp){
					case 0x00:   //strcat(pstr_data[0],"p");//None
						break;
					case 0x02:   strcat(pstr_data[0],"23Hz");//23.98Hz
						break;
					case 0x03:   strcat(pstr_data[0],"24Hz");//24Hz
						break;
					case 0x04:   strcat(pstr_data[0],"47Hz");//47.95 Hz
						break;
					case 0x05:   strcat(pstr_data[0],"25Hz");//25 Hz
						break;
					case 0x06:   strcat(pstr_data[0],"29Hz");//29.97 Hz
						break;
					case 0x07:   strcat(pstr_data[0],"30Hz");//30 Hz
						break;
					case 0x08:   strcat(pstr_data[0],"48Hz");//48 Hz
						break;
					case 0x09:   strcat(pstr_data[0],"50Hz");//50 Hz
						break;
					case 0x0a:   strcat(pstr_data[0],"59Hz");//59.94 Hz
						break;
					case 0x0b:   strcat(pstr_data[0],"60Hz");//60 Hz
						break;
					default:
						break;
				}
			}
				
	}

	OLED_ShowString(0,STATUS_Y_CURSOR,pstr_data[0],STATUS_SIZE);
}
/*************************************************
*函数介绍： 显示Audio_Format状态
*输入参数： 
*输出参数：
*返回值  ：
*************************************************/
extern void Display_Audio_Format(void)
{
	char pstr_data[2][Display_Line_Width]={"No_Support","<Exit"};
	OLED_ShowString(0,STATUS_Y_CURSOR,pstr_data[0],12);
}
/*************************************************
*函数介绍： 显示网络状态
*输入参数： 
*输出参数： 
*返回值  ：
*************************************************/
extern void Display_Ethernet_Status(void)
{
	char pstr_data[2][Display_Line_Width]={"","<Exit"};
	char UNLink[]="No_Link";
	char Link[]="Link";
	char error[]="ETH_ERROR";
	uint8_t eth_status=0;
	
	eth_status = Read_Eth_Link_Status();

	if(eth_status == 1){                 									//网络联通
		memcpy(pstr_data[0],Link,sizeof(Link));
	//	printf("eth_status %x link up\r\n",eth_status);
	}else if(eth_status == 0x00){                        //网络断开
		memcpy(pstr_data[0],UNLink,sizeof(UNLink));
	//	printf("eth_status %x link down\r\n",eth_status);
	}else if(eth_status == 0x02){                                                 //网络出差错
		Set_Status(1);     //eth_error !  phy值不正常  可能是MDC  MDIO 有问题
		memcpy(pstr_data[0],error,sizeof(error));
	}
	
	OLED_ShowString(0,STATUS_Y_CURSOR,pstr_data[0],STATUS_SIZE);
}
/*************************************************
*函数介绍： 显示错误状态
*输入参数： 
*输出参数： status = 0 GOOD状态,显示“0.0.0”       status == 1    显示“0.0.1”  
*返回值  ：
*************************************************/
extern void Display_Error_Status(void)
{
	char pstr_data[2][Display_Line_Width]={"","<Exit"};    //"None"
	char Good_Status[]="0.0.0";
	char FPGA_No_Link_Status[]="0.0.1";
	uint8_t status;
	
	status = Get_Status();   //Get_Error_Status();
	
	if(status == 0)
		memcpy(pstr_data[0],Good_Status,sizeof(Good_Status));
	else if(status == 1)
		memcpy(pstr_data[0],FPGA_No_Link_Status,sizeof(FPGA_No_Link_Status));
	
	OLED_ShowString(0,STATUS_Y_CURSOR,pstr_data[0],STATUS_SIZE);
}












