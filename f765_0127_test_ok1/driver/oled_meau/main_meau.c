#include "common.h"
#include "string.h"





static uint8_t blink_flag = 0;
static uint8_t enable_key_feature = 0;

uint16_t cnt_down=0;    //         100ms pollint
uint8_t Static_Display_Once=1;          //静态显示一次
uint8_t vid_link_flag=0;
uint8_t Display_Line_16[4][Display_Line_Width];    //oled  整行显示
uint8_t Display_Line_12[8][Display_Line_Width];    //oled  整行显示








/*************************************************
*函数介绍：获取是否需要闪烁标志
*输入参数：无
*输出参数：
*返回值  ：闪烁标志 1：显示  0：不显示
*************************************************/
uint8_t get_blink_flag(void)
{
	return blink_flag;
}



/*************************************************
*函数介绍：设置闪烁标志    1s调用一次
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
extern void set_blink_flag(void)
{
	static uint8_t n=1;
	if(n){
		n=0;
		blink_flag=1;
	}
	else{
		n=1;
		blink_flag=0;
	}
}





/*************************************************
*函数介绍：静态显示  "  +/-   . ms"     占用一栏
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
void Page0_piece4_Static_Display(void)    //显示  "  +/-   . ms"
{
	char line_2[]="  +/-";    //8*84=64  72  80    
	OLED_ShowString(0,0, line_2,12);
	OLED_ShowChar(64,0, '.',12);
	OLED_ShowChar(80,0, 'm',12);
	OLED_ShowChar(88,0, 's',12);
	
}
/*************************************************
*函数介绍：静态显示 “utc：时间 ”   及  “ +/-：”  的时间        占用一栏
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
void Page0_piece4_Dynamic_Display(void)   //
{
	char line_1_1[]="UTC: ";   //from  gps   or  RTC
	
	uint8_t line_1[1][20]={0};
	uint8_t line_y_1 = 5*8;      //strlen(line_1_1) * 8;  "UTC: "
	uint8_t line_y_2 = 5*8;     //"TOL +/- ";
	uint8_t line_y_3 = 9*8;
	static char j = '1';
	char line_2[3]="000";
	
#if 0
	memset(line_2,j,sizeof(line_2));
	j++;
	if(j == 0x3a){
		j = '0';
	}
#endif
	
	Get_UTCTime(&line_1[0],0);           //获取gps时间 
	
	if(Get_GPS_Display_Status() == 0x01){
		OLED_ShowString(line_y_1,-1, line_1[0],12); //显示GPS时间
		OLED_ShowString(0,-1, line_1_1,12);  //size = 12 时 ， y 可以从 -1
	}else if(Get_GPS_Display_Status() == 0x00){
		//memset(line_1[0],0,sizeof(line_1[0]));
		OLED_ShowString(line_y_1,-1,"          ",12);   //clear
		OLED_ShowString(0,-1, "     ",12);    //clear
	}

	OLED_ShowString(line_y_2,0, line_2,12); 
	OLED_ShowChar(line_y_3,0,j,12);
}

/*************************************************
*函数介绍：静态显示 " AVG:0s   . ms";   " JIT:0s   . ms";   占用二栏
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
extern void Page0_piece3_Static_Display(void)           //显示
{
	char line_3[]=" AVG: s";                              //" AVG: s   . ms";
	char line_4[]=" JIT: s";                              //" JIT: s   . ms";
	uint8_t mode;

	mode = Get_Config_Display_mode();
	
	if(mode == 3){                       //Detail mode diaplay   & default display video_mode
		OLED_ShowString(0,2, line_3,16);
		OLED_ShowChar(80,2, '.',16); 
		OLED_ShowChar(96,2, 'm',16); 
		OLED_ShowChar(104,2, 's',16); 
		
		OLED_ShowString(0,4, line_4,16); 
		OLED_ShowChar(80,4, '.',16); 
		OLED_ShowChar(96,4, 'm',16); 
		OLED_ShowChar(104,4, 's',16); 
	}else if(mode == 1 || mode == 2){   //video_mode || audio_mode
		OLED_ShowChar(84,0, 's',32);
		OLED_ShowChar(52,4, '.',32);
		OLED_ShowChar(84,4, 'm',32);
		OLED_ShowChar(100,4, 's',32);
	}
}

/*************************************************
*函数介绍：动态显示 AVG和JIT下的 延迟时间
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
void Page0_piece3_Dynamic_Display(void)   //
{
	static uint8_t judge_0='a';
	static uint8_t judge_1='a';
	static uint8_t judge_2='a';
	static uint8_t judge_3='a';
	static uint8_t judge_4='a';
	
	uint8_t line_y1 = 7*8;//strlen(line_1_1) * 8;
	uint8_t line_y2 = 5*8;
	uint8_t line_y3 = 11*8;
	uint8_t line_y4 = 4*16+4;         //
	 uint8_t i = '0';
	 uint8_t j = '0';
	uint8_t line_3[3]="999";
	uint8_t line_4[3]="999";
	uint8_t sec = '0';

	uint8_t reg_0 = 0;
	uint8_t reg_1 = 0;
	uint8_t reg_2 = 0;
	uint8_t reg_3 = 0;
	uint8_t reg_4 = 0;
	uint8_t reg_5 = 0;
	uint32_t gps_mode_jitter=0;
	uint32_t temp1;
	uint8_t temp;
	uint8_t display_mode;
	uint8_t clocktype_mode;
	uint8_t Delay_Data_Lock;
	
	clocktype_mode = Get_Config_SRC_clocktype();
	Delay_Data_Lock =	return_Reg_0x8d_num();

//	printf("clocktype_mode %d\r\n ",clocktype_mode);
	if(clocktype_mode == 0x01){                      //wire_mode
		Delay_Data_Lock &= 0x04;
		Delay_Data_Lock >>= 2;
		if(Delay_Data_Lock == 1){
		reg_0 = Get_Reg_0x8e();
		reg_1 = Get_Jit_Reg_1();   //16 进制
		reg_2 = Get_Jit_Reg_2();
		
		temp = reg_1 & 0xf0;
		temp = temp >> 4;
		line_4[0] = temp + 0x30;   //
		
		temp = reg_1 & 0x0f;
		line_4[1] = temp + 0x30;   //
		
		temp = reg_2 & 0xf0;
		temp = temp >> 4;
		line_4[2] = temp + 0x30;   //
		
		temp = reg_2 & 0x0f;   
		j = temp + 0x30;           // 
			
		sec = reg_0 + 0x30;	
		}else{
			line_4[0] = '9';
			line_4[1] = '9';
			line_4[2] = '9';
			j = '9';
			sec = '9';
		}
	}
#if 1	
	else if(clocktype_mode == 0x03  || clocktype_mode == 0x02){           //gps_mode  && osc_mode
		Delay_Data_Lock &= 0x08;
		Delay_Data_Lock >>= 3;
		if(Delay_Data_Lock == 1){
			reg_1 = Get_Reg_0x88();  //读取秒信息  通常为0
			if(reg_1 < 0x09){       //不能超过9s
				reg_5 = Get_Reg_0x8c();
				gps_mode_jitter += reg_5;
				
				reg_4 = Get_Reg_0x8b();   
				gps_mode_jitter += reg_4 * 0x100;
				
				reg_3 = Get_Reg_0x8a();  
				gps_mode_jitter += reg_3 * 0x10000;
				
				reg_2 = Get_Reg_0x89();
				gps_mode_jitter += reg_2 * 0x1000000;			 //按照林阳提供的算法
			
			//	printf("  gps_mode_jitter %x \r\n",gps_mode_jitter);
			
				sec = reg_1 + 0x30;
				
				gps_mode_jitter /= 0x9c4;
				
				j = gps_mode_jitter % 10 + 0x30;
				line_4[2] = (gps_mode_jitter % 100) / 10 + 0x30;
				line_4[1] = (gps_mode_jitter % 1000) / 100 + 0x30;
				line_4[0] = (gps_mode_jitter % 10000) / 1000 + 0x30;	
				
			//	printf(" %d %d %d %d\r\n",line_4[0],line_4[1],line_4[2],j);
			//	printf(" %x %x %x %x\r\n",reg_2,reg_3,reg_4,reg_5);
				
			}else{  //秒信息不为0   出错
				line_4[0] = '9';
				line_4[1] = '9';
				line_4[2] = '9';
				j = '9';
				sec = '9';
			}
		}else{  //秒信息不为0   出错
				line_4[0] = '9';
				line_4[1] = '9';
				line_4[2] = '9';
				j = '9';
				sec = '9';
		}
	}
#endif
	
	
	display_mode = Get_Config_Display_mode();

	if(display_mode == 3){                       //Detail mode diaplay   & default display video_mode		
		if(judge_1 != line_4[0] || Static_Display_Once == 1){                   //用静态显示 延迟时间  如果值同上次不同  或者  需要显示一次   
			OLED_ShowChar(line_y1,4, line_4[0],16);
			OLED_ShowChar(line_y1,2, line_4[0],16);   //AVG
			judge_1 = line_4[0];
		}
		if(judge_2 != line_4[1] || Static_Display_Once == 1){
			OLED_ShowChar(line_y1+8,4, line_4[1],16);
			OLED_ShowChar(line_y1+8,2, line_4[1],16);  //AVG
			judge_2 = line_4[1];
		}
		if(judge_3 != line_4[2] || Static_Display_Once == 1){
			OLED_ShowChar(line_y1+16,4, line_4[2],16);
			OLED_ShowChar(line_y1+16,2, line_4[2],16);  //AVG
			judge_3 = line_4[2];
		}	
		if(judge_4 != j || Static_Display_Once == 1){
			OLED_ShowChar(line_y3,4, j,16);
			OLED_ShowChar(line_y3,2, j,16);  //AVG
			judge_4 = j;
		}
		if(judge_0 != sec || Static_Display_Once == 1){
			OLED_ShowChar(40,4, sec,16); 
			OLED_ShowChar(40,2, sec,16);  //" AVG:"   5*8=40
			judge_4 = j;
		}
	}else if(display_mode == 1 || display_mode == 2){  //video_mode || audio_mode	
		if(judge_1 != line_4[0] || Static_Display_Once == 1){           //用静态显示    延迟时间
			OLED_ShowChar(4,4, line_4[0],32);
			judge_1 = line_4[0];
		}
		if(judge_2 != line_4[1] || Static_Display_Once == 1){ 
			OLED_ShowChar(20,4, line_4[1],32);
			judge_2 = line_4[1];
		}
		if(judge_3 != line_4[2] || Static_Display_Once == 1){
			OLED_ShowChar(36,4, line_4[2],32);
			judge_3 = line_4[2];
		}
		
		if(judge_4 != j || Static_Display_Once == 1){
			OLED_ShowChar(line_y4,4, j,32);
			judge_4 = j;
		}
		if(judge_0 != sec || Static_Display_Once == 1){
			OLED_ShowChar(line_y4,0, sec,32); 
			judge_4 = j;
		}
	}
}


/************************************************
*函数介绍：动态显示” status：状态“
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
void Page0_piece2_Dynamic_Display(uint8_t status)         //显示” status：状态“
{
	char line_5_1[]="  STATUS:";   // 0 GOOD   1 ERROR   2 INIT    9*8=72
	char line_5_2[4][Display_Line_Width]={"GOOD","ERROR","INIT","     "};
	
	uint8_t line_5_2_y = strlen(line_5_1) * 8;    //  	printf("%d",line_5_2_x);   
	
	OLED_ShowString(0,5, line_5_1,12);
	
	if(status<=2){
		if(status == 1){                 //error  需  闪烁
			if(get_blink_flag()){
				OLED_ShowString(line_5_2_y,5, line_5_2[status],12);
			}else{
				OLED_ShowString(line_5_2_y,5, line_5_2[3],12);
			}			
		}else if(status == 0){           //good
			OLED_ShowString(line_5_2_y,5, line_5_2[status],12);
		}else if(status == 2){   //init
			OLED_ShowString(line_5_2_y,5, line_5_2[status],12);
		}
	}
	
}

/*************************************************
*函数介绍：静态显示 GPS 、VID/AUD 、 MASTER/SLAVE 三种字体   oled主菜单下的最后一行
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
void Page0_piece1_Static_Display(uint8_t src_status)
{
	char line_6_1[]="GPS ";  //分开
	char line_6_2[]="VID ";
	char AUD[]="AUD ";	
	char line_6_3[2][Display_Line_Width]={"MASTER ","SLAVE "};	
	
	uint8_t line_6_2_y = strlen(line_6_1) * 8;
	uint8_t line_6_3_y = strlen(line_6_2) * 8 + line_6_2_y;
	uint8_t line_6_4_y = strlen(line_6_3[0]) * 8 + line_6_3_y;

	if(Get_Config_Display_mode() == 2){
		memcpy(line_6_2, AUD,sizeof(AUD));
	}
	
	if(src_status == 1){
		OLED_ShowString(line_6_3_y,6, line_6_3[0],12);
//		HDMI_OUT_LEDG(1);
	}else{
		OLED_ShowString(line_6_3_y,6, line_6_3[1],12);
//		HDMI_OUT_LEDG(0);
	}

}

/*************************************************
*函数介绍：动态显示 GPS 、VID/AUD 、 MASTER/SLAVE 三种字体 ，即需要闪烁时 ，oled主菜单下的最后一行
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
void Page0_piece1_Dynamic_Display(uint8_t gps_status,uint8_t vid_status,uint8_t src_status)      //oled主菜单下的最后一行
{
	char line_6_1[]="GPS ";  //分开
	char line_6_2[]="VID ";
	char AUD[]="AUD ";
	if(Get_Config_Display_mode() == 2){
			memcpy(line_6_2, AUD,sizeof(AUD));
	}
	char line_6_3[2][Display_Line_Width]={"MASTER","SLAVE "};	

	
	uint8_t line_6_2_y = strlen(line_6_1) * 8;
	uint8_t line_6_3_y = strlen(line_6_2) * 8 + line_6_2_y;

	if(Get_GPS_Display_Status() == 0x01){   //gps enable
		if(gps_status == 0){
			if(get_blink_flag())
				OLED_ShowString(0,6, line_6_1,12);
			else
				OLED_ShowString(0,6, "   ",12);	
		}else
			OLED_ShowString(0,6, line_6_1,12);
	}else if(Get_GPS_Display_Status() == 0x00){   //gps disable   clear diaplay 
			OLED_ShowString(0,6, "   ",12);
	}
	
	if(vid_status == 0){   //如果判断为0  即没有sdi没有视频   VID则闪烁
		if(get_blink_flag())
			OLED_ShowString(line_6_2_y,6, line_6_2,12);
		else
			OLED_ShowString(line_6_2_y,6, "   ",12);
	}else{                //否则，有视频，不闪烁
		OLED_ShowString(line_6_2_y,6, line_6_2,12);
	}
	if(src_status == 1)
		OLED_ShowString(line_6_3_y,6, line_6_3[0],12);    //master
	else if(src_status == 0)
		OLED_ShowString(line_6_3_y,6, line_6_3[1],12);   //slave

}



/*************************************************
*函数介绍；获取VID/AUD连接标志
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
extern uint8_t Get_vid_link_flag(void)
{
	return vid_link_flag;
}
/*************************************************
*函数介绍： 设置VID/AUD连接标志
*输入参数：
*输出参数：
*返回值  ：
*************************************************/
extern void Set_vid_link_flag(uint8_t flag)
{
	if(flag){
		vid_link_flag = 1;
	}else{
		vid_link_flag = 0;
	}
}

/*************************************************
*函数介绍： 主界面
*输入参数： 用于静态显示一次
*输出参数： 
*返回值  ： 1
*************************************************/
extern uint8_t Dsp_interface(uint8_t num)       //主界面
{
	char operation[]="operation";
	char test[]="999.9ms";        //for test
	uint8_t i;
	static uint8_t test_num=1;
	static uint8_t pre_mode=0;
	uint8_t mode;
	uint8_t Status=0;
	
	mode = Get_Config_Display_mode();
	Status = Get_Status();
	
	if(mode != pre_mode){
		pre_mode = mode;
		Static_Display_Once=1;
		num = 1;
		OLED_Clear();
		
	}
	
//	OLED_Clear(); 
	
	for(i=0; i<4; i++){
		memset(Display_Line_16[i],0,sizeof(Display_Line_16[i]));
	}
	
	
	
	if(mode == 3){    													//Detail mode diaplay   & default display video_mode
		if(num){   //if(1){
			Page0_piece4_Static_Display();
			Page0_piece3_Static_Display();		
			//Page0_piece1_Static_Display(Get_SRC_MODE());
		}
		Page0_piece4_Dynamic_Display();
		
		if(Status == 0)        //如果状态正常，才读reg
			Page0_piece3_Dynamic_Display();
		
		Page0_piece2_Dynamic_Display(Status);                 //status_flag  stm32自动识别 
		Page0_piece1_Dynamic_Display(Get_gps_value(),Get_vid_link_flag(),Get_SRC_MODE());
	}else if(mode == 1){		   										////Audio mode display
		Page0_piece3_Static_Display();	
		//Page0_piece1_Static_Display(Get_SRC_MODE());
		if(Status == 0 )	  //如果状态正常，才读reg
			Page0_piece3_Dynamic_Display();	
		//Page0_piece1_Dynamic_Display(Get_gps_value(),Get_vid_link_flag(),0);  //size = 32时  一行最多8个字符             128/16=8个字符
		
	}else if(mode == 2){      											//Video mode display
		Page0_piece3_Static_Display();	
		//Page0_piece1_Static_Display(Get_SRC_MODE());		
		if(Status == 0)     //如果状态正常，才读reg
			Page0_piece3_Dynamic_Display();	
		//Page0_piece1_Dynamic_Display(Get_gps_value(),Get_vid_link_flag(),0);       //size = 32时  一行最多8个字符             128/16=8个字符
	}

	return 1;
}





/*************************************************
*函数介绍： 清空数组
*输入参数： 
*输出参数： 
*返回值  ： 
*************************************************/
extern void Clear_Arr(void)
{
	uint8_t i=0;
	for(i=0 ; i<8; i++){
		memset(Display_Line_12[i],0,sizeof(Display_Line_12[i]));
		if(i<4){
			memset(Display_Line_16[i],0,sizeof(Display_Line_16[i]));
		}
	}
}



/*************************************************
*函数介绍： 设置倒计时时间
*输入参数： 
*输出参数： 
*返回值  ： 
*************************************************/
extern void Set_cnt_down(uint16_t cnt)
{
//CNT_DOWN
	cnt_down = cnt;  //100ms * 1200 = 120s  大概2分钟内，按键无操作，自动跳回主显示菜单  
}
/*************************************************
*函数介绍： 显示当前菜单
*输入参数： 
*输出参数： 
*返回值  ： 
*************************************************/
extern void Pollint_Display_Main_menu(uint8_t key_mode)
{
	static uint8_t clear_Oled_Flag=1;   //判断是否需要清屏幕
	if(cnt_down != 0 ){
		cnt_down--; 	
		if(clear_Oled_Flag == 0){
			clear_Oled_Flag=1;
			Set_Key_Feature(0); 
			ClearAllEvent();
			menuReset();
		}else{
			Set_Key_Feature(1); 
		}	
	}else{
		Set_Key_Feature(0);    //disable key feature
		if(clear_Oled_Flag){
			menuReset_No_RenderMenu();
			OLED_Clear(); 
		}
		Dsp_interface(clear_Oled_Flag) ;
		clear_Oled_Flag=0;
	}
}


extern void Set_Key_Feature(uint8_t key_feature)
{
	if(key_feature)
		enable_key_feature = 1;
	else
		enable_key_feature = 0;
}
extern uint8_t Get_Key_Feature_Flag(void)
{
	return enable_key_feature ;
}



////////////////////////////////////////////////////////////////////////////////











