
#ifndef MENU_H_
#define MENU_H_


struct MENU_ITEM;

/**
 * @name  Enum declaration for buttons functionality
 * Should not be changed as the big chunk of logic is based on it.
 */
typedef enum {
	MENU_UP,
	MENU_DOWN,
	MENU_CONFIRM,
	MENU_CANCEL,
	MENU_NONE
} MENU_BUTTON_ROLE;
#define MENU_BUTTON_ROLE_COUNT  5 // Length of MENU_BUTTON_ROLE items

/**
 * @name  Enum declaration for menu nest levels
 * Change when menu has more/less nest levels.
 */
typedef enum {
	MENU_LEVEL_1,
	MENU_LEVEL_2,
	MENU_LEVEL_3,
	MENU_LEVEL_4,
	MENU_CALLBACK,
}MENU_LEVEL;
#define MENU_LEVEL_COUNT        5 // Length of MENU_LEVEL items

/**
 * @name  Structure to represent menu button 
 * Represents each button in the menu, when state changed to pressed, an event will be dispatched.
 */
typedef struct {
	MENU_BUTTON_ROLE role;
	unsigned char button_PIN;
	unsigned int state;
	//bool pressed;
	unsigned char pressed;
} MENU_BUTTON;


/**
 * @name  Structure to represent overall menu state 
 * Works like a state machine and indicates/caches the actual and previous state of menu.
 */
typedef struct MENU_STATE {
	struct MENU_STATE *prev_state;
	struct MENU_ITEM *current_item;
	unsigned char current_item_idx;
	unsigned char cursor_line;
	MENU_LEVEL level;
}MENU_STATE;






/**
 @brief    Initialize menu initial state
 @param    void                                        
 @return   none
*/
extern void menuInit();


/**
 @brief    Reset menu to the initial state
 @param    void                                        
 @return   none
*/
extern void menuReset();

extern void menuReset_No_RenderMenu();

/**
 @brief    Check for menu button press and dispatch event if occurs
 @param    void                                        
 @return   none
*/
//extern void menuPollButtons();
extern void menuPollButtons(uint8_t key_mode);

extern  void scrollMenu(MENU_BUTTON *button);

extern unsigned char Set_button_num(unsigned char ch  );

extern unsigned char Get_button_num(void );

extern void Set_Display_Once_Flag(uint8_t flag);
extern uint8_t Get_Display_Once_Flag(void);
extern void Set_Exit(void);

#endif /* MENU_H_ */