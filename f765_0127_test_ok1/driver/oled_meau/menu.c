#include "common.h"
//#include <avr/io.h>
#include <stdlib.h>
//#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

//#include "menu_portsio.h"
//#include "lcd.h"



#define OLED_LINES 8
#define TRUE 1
#define FALSE 0

#define MENU_PORTSIO_BTN_UP 0
#define MENU_PORTSIO_BTN_DOWN 1
#define MENU_PORTSIO_BTN_CONFIRM 2
#define MENU_PORTSIO_BTN_CANCEL 3

#define MENU_PORTSIO_PIN 1

MENU_BUTTON_ROLE button_num;
uint8_t display_flag=0;

/*
** Menu main components definition
*/

/*************************************************************************
 All possible menu states, depend how many nest actions we want
*************************************************************************/
static MENU_STATE menu_states[MENU_LEVEL_COUNT] = {
	{NULL, NULL, 0, 0, MENU_LEVEL_1},
	{NULL, NULL, 0, 0, MENU_LEVEL_2},
	{NULL, NULL, 0, 0, MENU_LEVEL_3},
	{NULL, NULL, 0, 0, MENU_LEVEL_4},
	{NULL, NULL, 0, 0, MENU_CALLBACK}
};


/*************************************************************************
 Assign menu state default value to first one (home menu)
*************************************************************************/
static MENU_STATE *current_state = &menu_states[0];

/*************************************************************************
 Map all menu action buttons to the real pins and assign functionality
*************************************************************************/
static MENU_BUTTON menu_buttons[MENU_BUTTON_ROLE_COUNT] = {
	{MENU_UP,      MENU_PORTSIO_BTN_UP,      0, FALSE},  //false},
	{MENU_DOWN,    MENU_PORTSIO_BTN_DOWN,    0, FALSE},  //false},
	{MENU_CONFIRM, MENU_PORTSIO_BTN_CONFIRM, 0, FALSE},  //false},
	{MENU_CANCEL,  MENU_PORTSIO_BTN_CANCEL,  0, FALSE},   //false}
	{MENU_NONE,  		0,  0, FALSE}   //false}
};


/*
** Render and control functions
*/
/*************************************************************************
 Helpers for setting the state properties
*************************************************************************/
static void setState(MENU_STATE *state, MENU_STATE *new_prev_state, MENU_ITEM *new_current_item, unsigned char new_current_item_idx, unsigned char new_cursor_line)
{
	state->prev_state = new_prev_state;
	state->current_item = new_current_item;
	state->current_item_idx = new_current_item_idx;
	state->cursor_line = new_cursor_line;
}

static void resetState(MENU_STATE *state){
	setState(state, NULL, NULL, 0, 0);
}

/*************************************************************************
 Basic render menu function, responsible for rendering the menu
*************************************************************************/
static void renderMenu(){


	MENU_ITEM *items = menuItemsGetHomeMenu();
	unsigned char max_options = menuItemsGetHomeMenuSize();
	unsigned char first_item = current_state->current_item_idx - current_state->cursor_line;
	MENU_ITEM *previous_item;
	int i,j;

	//	lcd_clrscr();
	//system("cls");
	OLED_Clear();
//	printf(" renderMenu \n");

	if(current_state->level != MENU_LEVEL_1){
		//MENU_ITEM *previous_item = menu_states[current_state->prev_state->level].current_item;
		previous_item = menu_states[current_state->prev_state->level].current_item;
		items = previous_item->submenu;//
		max_options = previous_item->submenu_length;
	//	printf(" != MENU_LEVEL_1 \n");
	}
//	printf(" max_options %d \n",max_options);
//	printf(" current_state->cursor_line %d \n",current_state->cursor_line);
//	unsigned char first_item = current_state->current_item_idx - current_state->cursor_line;

	for(i=0; i<max_options && i<OLED_LINES; i++){
		j = i - 1;
		if(i==current_state->cursor_line){
			OLED_ShowString_2(0,j, items[first_item+i].title,12);	  //反白  整行  即  当前光标位置
		}
		else {
			OLED_ShowString(0,j, items[first_item+i].title,12);	   //不 反白 整行
		}
		
		if(items[first_item+i].status == NONE){
			
		}else if(items[first_item+i].status == MENU_SET){
			OLED_ShowChar(112,j,'Y',12);
		}else if(items[first_item+i].status == MENU_RESET){
			OLED_ShowChar(112,j,'N',12);
		}else if(items[first_item+i].status == NON_OPERATE){
			OLED_ShowChar(112,j,'-',12);
		}

	}
}

/***************************************************************************************
 Function to handle basic btn clicks (Executed when menu is not in CALLBACK state)
***************************************************************************************/
extern  void scrollMenu(MENU_BUTTON *button)
{
	unsigned char max_options;
	MENU_ITEM *previous_item ;
	MENU_ITEM *items;
	MENU_ITEM *item;
	MENU_STATE *prev_state;

	switch(button->role){
		case MENU_UP:
			max_options = menuItemsGetHomeMenuSize();
			if(current_state->level != MENU_LEVEL_1){
				previous_item = menu_states[current_state->prev_state->level].current_item;
				max_options = previous_item->submenu_length;
			}
			if(current_state->current_item_idx != 0){
				if(current_state->cursor_line == 0){
					// Move screen
					current_state->current_item_idx -= 1;
				}else{
					// Move cursor
					current_state->cursor_line -= 1;
					current_state->current_item_idx -= 1;
				}
			}else{
					current_state->current_item_idx = max_options - 1;
					current_state->cursor_line = OLED_LINES - 1;
					if(current_state->cursor_line > current_state->current_item_idx)
						current_state->cursor_line = max_options - 1;
			}
			break;
		case MENU_DOWN: {
			max_options = menuItemsGetHomeMenuSize();
			if(current_state->level != MENU_LEVEL_1){
				previous_item = menu_states[current_state->prev_state->level].current_item;
				max_options = previous_item->submenu_length;
			}

			if(current_state->current_item_idx < max_options-1){
				if(current_state->cursor_line == OLED_LINES - 1){
					// Move screen
					current_state->current_item_idx += 1;
				}else{
					// Move cursor
					current_state->cursor_line += 1;
					current_state->current_item_idx += 1;
				}
			}else{
				current_state->cursor_line = 0;
				current_state->current_item_idx=0;
			}
			break;
		}
		case MENU_CONFIRM:
		//	printf(" \n MENU_CONFIRM \n");
			items = menuItemsGetHomeMenu();
			if(current_state->level != MENU_LEVEL_1){
				previous_item = menu_states[current_state->prev_state->level].current_item;
				items = previous_item->submenu;
			}
				
			item = &items[current_state->current_item_idx];
			prev_state = current_state;
			if(item->render_callback){
				prev_state->current_item = item;
				current_state = &menu_states[MENU_CALLBACK];
				setState(current_state, prev_state, prev_state->current_item, prev_state->current_item_idx, 0);
			}else if(item->submenu){
				prev_state->current_item = item;
				current_state = &menu_states[current_state->level + 1];
				setState(current_state, prev_state, NULL, 0, 0);
			}
			break;
		case MENU_CANCEL:
			if(current_state->level != MENU_LEVEL_1){
				current_state = &menu_states[current_state->prev_state->level];
			}
			break;
	}
	if(current_state->level != MENU_CALLBACK){
		//printf(" scrollMenu != MENU_CALLBACK \n");
		renderMenu();
	}else{
		current_state->prev_state->current_item->render_callback(current_state->current_item_idx);
	}
}



extern void menuReset()
{
	current_state = &menu_states[MENU_LEVEL_1];
	resetState(current_state);
	renderMenu();
}

extern void menuReset_No_RenderMenu()
{
	current_state = &menu_states[MENU_LEVEL_1];
	resetState(current_state);
//	renderMenu();
}


extern void menuInit()
{
//	menuPortsioInit();
//	menuReset();
	menuReset_No_RenderMenu();

	Arr_Init();
}





extern void Set_Display_Once_Flag(uint8_t flag)
{
	display_flag = flag;
}
extern uint8_t Get_Display_Once_Flag(void)
{
	return display_flag;
}
extern unsigned char Set_button_num(unsigned char ch )
{
	if(ch == 'w')
		button_num = MENU_UP;
	else if(ch == 's')
		button_num = MENU_DOWN;
	else if(ch == 'a')
		button_num = MENU_CONFIRM;
	else if(ch == 'd')
		button_num = MENU_CANCEL;
	else
		button_num = MENU_NONE;
	Set_Display_Once_Flag(1);
}

extern unsigned char Get_button_num(void )
{
	return button_num;
}
extern void menuPollButtons(uint8_t key_mode)
{
	unsigned char i,exited;
	MENU_BUTTON *b;

	i=0;
	//printf(" key_mode %d ",key_mode);
	
//	for( i=0; i<1; i++){    //MENU_BUTTON_ROLE_COUNT
		b = &menu_buttons[i];
	
	switch (key_mode){
	case evKeyDown:
		b->role = MENU_DOWN;
		b->state = 0xF000 ;//test
		break;
	case evKeyUP:
		b->role = MENU_UP;
		b->state = 0xF000 ;//test
		break;
	case evKeyConfirm:
		b->role = MENU_CONFIRM;
		b->state = 0xF000 ;//test
		break;
	case evKeyCancal:
		b->role = MENU_CANCEL;
		b->state = 0xF000 ;//test
		break;
	default:
		b->role = MENU_NONE;
		b->state = 0x0000 ;//test
		break;
	}
	//printf(" menuPollButtons");
		// Eliminate button debouncing
		// http://www.ganssle.com/debouncing-pt2.htm
		// b->state = (b->state << 1) | !((MENU_PORTSIO_PIN) & (1<<(b->button_PIN))) | 0XE000;
	
		//b->role = Get_button_num();
		
			

		if(b->state == 0xF000){   //判断有按键按下  
			b->pressed = TRUE;
			if(current_state->level != MENU_CALLBACK){
				scrollMenu(b);                        //判断是那个按键
			}else{
				 exited = current_state->prev_state->current_item->btn_handle_callback(b, current_state->current_item_idx);
				if(exited){
					current_state = &menu_states[current_state->prev_state->level];
					renderMenu();
				}
			}
			//break;    //666
		}else{
			b->pressed = FALSE;
		}
//	}
	
}

extern void Set_Exit(void)
{
	current_state->level = 4;
}


