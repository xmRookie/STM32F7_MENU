#include "common.h"

#define NULL_KEY     0xffff
#define NOR_KEY      0x8000
#define RESET_KEY    0x4000

#define KEY_RESET_GPIO                     GPIOA

#define  CNT_DOWN 300//1200   // 1200 * 100 ms = 120 000ms


#define KeyFilterLen        3
static u8_t SCodeBuf[KeyFilterLen];
static u8_t PreKeyCode;            // KeyCode1存储上次扫描码,KeyCode2存储上次提交的按键值
static u8_t PressedKey;          // 存储上次提交的按键值
static u16_t KeyPressCount;

u8_t KeyScanCode;

extern int Current_y;
extern int Current_Page;

static void key_Par_Init(void)
{
    memset(SCodeBuf, 0xff, sizeof(SCodeBuf));
    PreKeyCode = 0xff;
    PressedKey = 0xff;
    KeyPressCount = 0;
    KeyScanCode = 0;
}


//按键初始化函数
extern void MX_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_KEY_InitStruct;

	 __HAL_RCC_GPIOA_CLK_ENABLE();
	
    /*Configure GPIO pin : PA10 PA11 PA12 */

		GPIO_KEY_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_KEY_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_KEY_InitStruct.Pull = GPIO_PULLUP;//GPIO_NOPULL;
		HAL_GPIO_Init(KEY_RESET_GPIO, &GPIO_KEY_InitStruct);
	
	
    key_Par_Init();
}



//函数  读取当前按键扫描值
//输入  无
//输出  无
//返回  按键扫描码,或者无按键(0xff)
static u8_t GetScanCode(void)
{

    u8_t kvalue;

    if(HAL_GPIO_ReadPin(KEY_RESET_GPIO, GPIO_PIN_10) == GPIO_PIN_RESET) {
        kvalue = 0x04;   //0x04
				return (kvalue);
    } else if(HAL_GPIO_ReadPin(KEY_RESET_GPIO, GPIO_PIN_11) == GPIO_PIN_RESET) {
        kvalue = 0x05;   
				return (kvalue);
    } else if(HAL_GPIO_ReadPin(KEY_RESET_GPIO, GPIO_PIN_12) == GPIO_PIN_RESET) {
        kvalue = 0x06;   
				return (kvalue);
    }else {
        kvalue = 0xff;
    }
		return (kvalue);
    
}


static u16_t GetKey(void)
{
    u8_t ch;

    for(ch = KeyFilterLen - 1 ; ch ; ch--) {
        SCodeBuf[ch] = SCodeBuf[ch - 1];
    }

    SCodeBuf[0] = GetScanCode();                         // 读取按键当前扫描码

    for(ch = 1 ; ch < KeyFilterLen ; ch++) {
        if(SCodeBuf[ch] != SCodeBuf[0]) {
            return NULL_KEY;
        }
    }

    ch = SCodeBuf[0];

//------------------ 上面的代码为防抖动滤波处理
    if(ch == 0xff) {
        // 当前没有按键按下
        if(PressedKey == 0xff) {
            // 弹起按键已经提交
            PreKeyCode = 0xff;
            KeyPressCount = 0;
            return(NULL_KEY);
        }

        if(PreKeyCode != 0xff) {
            // 弹起按键还没有提交,且第一次侦测到按键弹起
            PreKeyCode = 0xff;
            return(NULL_KEY);
        }

        ch = PressedKey;                             // 两次读到的扫描一样才返回
        KeyScanCode = 0xff;
        KeyPressCount = 0;
        PressedKey = 0xff;
        return(NOR_KEY + ch);
    }

    if(PressedKey == 0xff) {
        // 按键还没有提交
        if(ch == PreKeyCode) {                       // 两次读到的扫描一样才返回
            // 说明是按键按下
            PressedKey = ch;
            KeyPressCount = 1;
            KeyScanCode = ch;
            return(ch);
        } else {
            PreKeyCode = ch;
            return(NULL_KEY);
        }
    }

    if(ch == PreKeyCode) {                            // 两次读到的扫描一样才返回
        if(PressedKey == ch) {
            // 处理持续按键事件
            KeyPressCount++;

            if(KeyPressCount == 50) {              // 提交3秒事件     KeyPressCount == 300  
                return(RESET_KEY + ch);
            }

            return(NULL_KEY);
        } else {
            // 按下新的按键,但上一按键的弹起按键还没有提交,先提交上一按键的弹起按键
            ch = PressedKey;
            KeyScanCode = 0xff;
            KeyPressCount = 0;
            PreKeyCode = 0xff;
            PressedKey = 0xff;
            return(NOR_KEY + ch);
        }
    } else {
        PreKeyCode = ch;
        return (NULL_KEY);
    }
}

uint8_t key_3s=0;

/*
uint8_t led_1s = 0;
uint8_t led_100ms = 1;
uint8_t led_10ms = 0;
extern uint8_t Is_1s_led(void)
{
	return led_1s;
}
extern uint8_t Is_100ms_led(void)
{
	return led_100ms;
}
extern uint8_t Is_10ms_led(void)
{
	return led_10ms;
}
void Set_1s_led(void)
{
	led_1s=1;
	led_100ms=0;
	led_10ms=0;
}
void Set_100ms_led(void)
{
		led_1s=0;
	led_100ms=1;
	led_10ms=0;
}
extern void Set_10ms_led(void)
{
		led_1s=0;
	led_100ms=0;
	led_10ms=1;
}
*/

// 函数    按键扫描程序
extern void key_Scan(void)
{
    u16_t tmp;
	  //STR_EVENT dcode;
    tmp = GetKey();

    if(tmp == NULL_KEY) {
        //未接收到按键数据
        //printf("null key\n");
        return;
    }

    if(tmp & NOR_KEY) {
        // 弹起按键
	//		printf("%x",tmp);		
			if(!key_3s){
				if(tmp == 0x8004){    //5
			//		Set_10ms_led();
			Set_cnt_down(CNT_DOWN);
			SetWorkMode(MODE_KEY);
				PushEvt_EvtPar(evKeyUP,MODE_KEY);
		//			Set_button_num('w');
					
	
				}else if(tmp == 0x8005){   //按键2
			//		Set_100ms_led();
			Set_cnt_down(CNT_DOWN);
			SetWorkMode(MODE_KEY);
					PushEvt_EvtPar(evKeyDown,MODE_KEY);
			//		Set_button_num('s');
					
					

				}else if(tmp == 0x8006){  //按键3
				Set_cnt_down(CNT_DOWN);
					

		SetWorkMode(MODE_KEY);
					PushEvt_EvtPar(evKeyConfirm,MODE_KEY);
			//		Set_button_num('a');
					
					
		//			Set_1s_led();
				}else{}
			}else{
				key_3s=0;
			}
        //printf("NOR_KEY %x \n", tmp);
        return;
    } else if(tmp & RESET_KEY) {
        // 3秒按键
			key_3s=1;
			
			Set_button_num('d');
			Set_cnt_down(120);
	
			
		//	printf("%x",key_3s);
			  PushEvt_OnlyEvt(evRst);
        return;
    } else {
			//按下按键
				//	printf("KEY %d \n", tmp);
	
    }
}

