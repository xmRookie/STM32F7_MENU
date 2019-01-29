#ifndef MENU_CALLBACK_H_
#define MENU_CALLBACK_H_

/**
 * @name  Structure to represent menu item 
 * Represents each menu item. Single MENU_ITEM can contains `render_callback` && `btn_handle_callback` or
 * `submenu_length` && `submenu`.
 * If item has first set, it will be used as final, custom action for menu.
 * If item has second set, it will be used as another step from which the menu nest will proceed.
 */


# define MENU_HOME_ITEMS 4

# define SYSTEM_MENU 6
# define SYSTEM_FEATURE_MENU 6

# define CONFIG_MENU 8
# define CONFIG_FORMAT_MENU 18
# define CONFIG_VIDEO_MENU 3
# define CONFIG_VIDEO_INPUT_MENU 4
# define CONFIG_VIDEO_OUTPUT_MENU 3
# define CONFIG_AUDIO_MENU 3
# define CONFIG_NETWORK_MENU 3
# define CONFIG_NETWORK_IP_MENU 5
# define CONFIG_NETWORK_MAC_MENU 7
# define CONFIG_GPS_MENU 2
# define CONFIG_SRC_MENU 5
# define CONFIG_SRC_MODE_MENU 3
# define CONFIG_SRC_SYNC_TYPE_MENU 13
# define CONFIG_SRC_CLOCK_TYPE_MENU 4
# define CONFIG_SRC_SYNC_FRAME_MENU 7
# define CONFIG_DISPLAY_MENU 3
# define CONFIG_DISPLAY_MODE_MENU 4
# define CONFIG_DISPLAY_BRIGHTNESS_MENU 3

# define STATUS_MENU 6 
# define STATUS_SYNCH_MENU 3

typedef enum {
	MENU_SET,
	MENU_RESET,
	NONE,
	NON_OPERATE,
}MENU_STATUS;
#define MENU_STATUS_COUNT        4 // Length of MENU_LEVEL items

typedef struct MENU_ITEM {
	//char *title;
	char title[20];
	void (* render_callback)(unsigned char which);
	//bool (* btn_handle_callback)(MENU_BUTTON *button, unsigned char which);
	unsigned char (* btn_handle_callback)(MENU_BUTTON *button, unsigned char which);
	unsigned char submenu_length;
	struct MENU_ITEM *submenu;
	MENU_STATUS status;
} MENU_ITEM;

MENU_ITEM *menuItemsGetHomeMenu();
unsigned char menuItemsGetHomeMenuSize();

void System_UniversalCallbackRender(unsigned char which);
unsigned char System_UniversalCallback(MENU_BUTTON *button, unsigned char column);

void Config_Display_Mode_ActionCallbackRender(unsigned char which);
unsigned char Config_Display_Mode_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_Display_Brightness_ActionCallbackRender(unsigned char which);
unsigned char Config_Display_Brightness_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Status_Synch_PrintCallbackRender(unsigned char which);
unsigned char Status_Synch_PrintCallback(MENU_BUTTON *button, unsigned char column);

void Status_PrintCallbackRender(unsigned char which);
unsigned char Status_PrintCallback(MENU_BUTTON *button, unsigned char column);

void Config_Format_ActionCallbackRender(unsigned char which);
unsigned char Config_Format_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_SRC_Sync_Frame_ActionCallbackRender(unsigned char which);
unsigned char Config_SRC_Sync_Frame_ActionCallback(MENU_BUTTON *button, unsigned char column);


void Config_SRC_Clock_Type_ActionCallbackRender(unsigned char which);
unsigned char Config_SRC_Clock_Type_ActionCallback(MENU_BUTTON *button, unsigned char column);


void Config_SRC_Sync_Type_ActionCallbackRender(unsigned char which);
unsigned char Config_SRC_Sync_Type_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_SRC_Mode_ActionCallbackRender(unsigned char which);
unsigned char Config_SRC_Mode_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_GPS_ActionCallbackRender(unsigned char which);
unsigned char Config_GPS_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_Network_IP_ActionCallbackRender(unsigned char which);
unsigned char Config_Network_IP_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_Network_MAC_ActionCallbackRender(unsigned char which);
unsigned char Config_Network_MAC_ActionCallback(MENU_BUTTON *button, unsigned char column);


void Config_Video_Input_ActionCallbackRender(unsigned char which);
unsigned char Config_Video_Input_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Config_Video_Output_ActionCallbackRender(unsigned char which);
unsigned char Config_Video_Output_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Exit_ActionCallbackRender(unsigned char which);
unsigned char Exit_ActionCallback(MENU_BUTTON *button, unsigned char column);

void Exit2MainMenu_ActionCallbackRender(unsigned char which);
unsigned char Exit2MainMenu_ActionCallback(MENU_BUTTON *button, unsigned char column);


extern void Arr_Init(void);
extern void Arr_Update(void);
void ip_mac_char(void);
char * ip_int_to_char(unsigned char ip_num) ; //??intD¨ª¡Áa3¨¦char
char* my_mac_int_to_char(unsigned char mac_num) ; //??intD¨ª¡Áa3¨¦char

#endif /* MENU_CALLBACK_H_ */