#include "common.h"
//#include "time.h"
//#include "stdio.h"
//#include "stdlib.h"



#define Tol_Arr_Numb 128
#define FiFo_Active_cnt_MAX 128


#define config_rw9_addr    0x09
#define config_rwa_addr    0x0a
#define config_rwb_addr    0x0b  
#define config_rwc_addr    0x0c 

/*
typedef struct 
{
	uint32_t cpuclk_last;
	uint32_t cpuclk_delay;
	uint32_t cpuclk_last_average;
	uint32_t cpuclk_cur_average;
	uint8_t gps_value;
	uint8_t gps_sec;
	uint8_t valid_cnt;
}TOL;
TOL Tol_Arr[Tol_Arr_Numb];
*/
uint32_t temp;
uint8_t x=0;

static uint8_t Send_Sec_Plus_Flag=0;
static uint8_t GPS_Sec=0;
static uint8_t Rewrite_GPS_Sec = 0;
static uint8_t Reset_Pulse_Flag=0;
static uint8_t Send_Dirft_Flag=0;

uint32_t Delay_Arr[Tol_Arr_Numb];
uint32_t Average_Arr[Tol_Arr_Numb];

uint8_t FiFo_Active = 0; 
uint8_t FiFo_Active_cnt=0;

uint32_t clk_drift_calc=0;
uint32_t clk_dirft_current=0;

//飘值的hex
uint8_t config_rw9_num=0;
uint8_t config_rwa_num=0;
uint8_t config_rwb_num=0;
uint8_t config_rwc_num=0;



/* --------------------------Set  Send_sec_plus  to   fpga --------------------------------*/
extern void Set_Send_Sec_Plus(uint8_t flag,uint8_t Sec)
{
	if(flag) {
		Send_Sec_Plus_Flag = 1;
		GPS_Sec = Sec;
	}else{
		Send_Sec_Plus_Flag = 0;
	}
}
/* --------------------------Get  Send_sec_plus  to   fpga --------------------------------*/
extern uint8_t Get_Send_Sec_Plus(void)
{
	return Send_Sec_Plus_Flag;
}
/* --------------------------Get_GPS_Sec --------------------------------*/
extern uint8_t Get_GPS_Sec(void)
{
	Rewrite_GPS_Sec = GPS_Sec;
	Rewrite_GPS_Sec <<= 1;
	return Rewrite_GPS_Sec;                 //返回当前的 秒值  信息
}
/* --------------------------Send_GPS_info --------------------------------*/
extern void Send_GPS_info(void)
{
	if(Get_Send_Sec_Plus()){
		//		Send_GPS_Sec_Num(Get_GPS_Sec());
				Set_Send_Sec_Plus(0,0);
				Send_GPS_Sec_Pulse(0x80);             //上升沿
				Reset_Pulse_Flag = 1;
	}else{
				if(Reset_Pulse_Flag){
					Reset_Pulse_Flag = 0;
					Send_GPS_Sec_Pulse(0x00);           // 变为低电平    用于下次发送上升沿
				}
	}
}

/* --------------------------Set_Send_Dirft_Flag --------------------------------*/
extern void Set_Send_Dirft_Flag(uint8_t flag)
{
	if(flag)
		Send_Dirft_Flag = 1;
	else
		Send_Dirft_Flag = 0;
}
/* --------------------------Get_Send_Dirft_Flag --------------------------------*/
extern uint8_t Get_Send_Dirft_Flag(void)
{
	return Send_Dirft_Flag;
}

extern void Send_Dirft(void)
{
	static uint8_t reg=1;
	switch(reg){
		case 1: 
						Send_GPS_Dirft(config_rw9_addr,config_rw9_num);
						reg++;
			break;
		case 2: 
						Send_GPS_Dirft(config_rwa_addr,config_rwa_num);
						reg++;
			break;
		case 3: 
						Send_GPS_Dirft(config_rwb_addr,config_rwb_num);
						reg++;
			break;
		case 4:
						Send_GPS_Dirft(config_rwc_addr,config_rwc_num);
						Set_Send_Dirft_Flag(0);
						reg=1;
			break;
		default:
						reg=1;
			break;
	}
}

/* --------------------------GPS_PPS_Init --------------------------------*/
extern void GPS_PPS_Init(void)
{
	//memset(Tol_Arr,0,sizeof(Tol_Arr));
	GPIO_InitTypeDef GPIO_InitStruct;
		/*Configure GPIO pin : PD2 */               //for GPS
  GPIO_InitStruct.Pin = GPIO_PIN_2;   
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

static uint32_t cnt=0;
extern void cpuclk_cnt_add(void)
{
	cnt++;
}

/*
extern uint8_t Get_GPS_sec(void)   //获取当前gps 秒 信息 
{
	return 1;
}
*/
extern uint32_t Get_cpuclk(void)   //获取 当前 cpu 时钟
{
	return cnt;
}



extern void GPS_PPS_Interrupt(void)
{
	static uint32_t clk_dirft_current=12500000;//50000;   //默认值   test  12.5MHZ 12500000
	static uint32_t cnt=0;
	static uint32_t previous_cpuclk=0;
	uint32_t current_cpuclk;
	uint32_t cpu_delay;
	
	//current_cpuclk = Get_cpuclk();
	
	//	current_cpuclk = Get_Cnt();
	current_cpuclk = Get_RTC_clk();
		cpu_delay = current_cpuclk - previous_cpuclk;	
	
	if(cpu_delay > clk_dirft_current*1.2){
		previous_cpuclk = current_cpuclk;	
		FiFo_Active_cnt=0;
		FiFo_Active=0;
	}else if(cpu_delay < clk_dirft_current*0.8){
		previous_cpuclk = current_cpuclk;	
		FiFo_Active_cnt=0;
		FiFo_Active=0;
	}else{
		clk_dirft_current = (clk_dirft_current * cnt + cpu_delay) / (cnt+1);
		Average_Arr[x] = clk_dirft_current;
		Delay_Arr[x] = cpu_delay;
		x = RBUF_NEXT_PT(x, 1, Tol_Arr_Numb);
		cnt++;
		FiFo_Active_cnt++;
		if(FiFo_Active_cnt > FiFo_Active_cnt_MAX){               //clk_drift_calc必须连续128个有效，才发送给fpga，否则不发送
			FiFo_Active_cnt = FiFo_Active_cnt_MAX;
			FiFo_Active = 1;
		}else{
			FiFo_Active = 0;
		}
		
		if(FiFo_Active){
			clk_drift_calc = clk_dirft_current;    //clk_drift_calc发给fpga		
			config_rwc_num =  clk_drift_calc % 0x100;
			config_rwb_num = (clk_drift_calc % 0x10000) / 0x100;
			config_rwa_num = (clk_drift_calc % 0x1000000) / 0x10000;
			config_rw9_num =  clk_drift_calc / 0x1000000;	
			Set_Send_Dirft_Flag(1);
		}else{
			clk_drift_calc = clk_drift_calc;       //clk_drift_calc不发送给fpga
		}
		
		if(cnt > Tol_Arr_Numb){
			cnt = Tol_Arr_Numb;
		}
			
	}
	
	previous_cpuclk = current_cpuclk;

	
	Set_Send_Sec_Plus(1,0);                   //使能送gps秒脉冲
	
#if 1	
	//test
			config_rwc_num =  clk_dirft_current % 0x100;
			config_rwb_num = (clk_dirft_current % 0x10000) / 0x100;
			config_rwa_num = (clk_dirft_current % 0x1000000) / 0x10000;
			config_rw9_num =  clk_dirft_current / 0x1000000;
			Set_Send_Dirft_Flag(1);
	//		printf(" %x %x %x %x \r\n",config_rwc_num,config_rwb_num,config_rwa_num,config_rw9_num);
	//printf("%d %d %d %d\r\n",current_cpuclk,cpu_delay,cnt,clk_dirft_current);     //log info
	
//	printf("Act%d Act_cnt%d \r\n",FiFo_Active,FiFo_Active_cnt);                 //log info
//	Get_RTC_Time();                                                             //log info
#endif
	
//	Get_RTC_clk();
	
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_2){
		//printf("GPIO_PIN_2\r\n");
			GPS_PPS_Interrupt();
	}
	
	if(GPIO_Pin  == GPIO_PIN_5){    //目前  PG5  ->  pHY ETH_INTER

		Link_Callback();          //eth
	}
}















