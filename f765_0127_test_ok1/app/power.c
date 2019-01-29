#include "common.h"

static uint8_t workMode;

extern void SetWorkMode(uint8_t mode)
{
    workMode = mode;
}

extern uint8_t GetWorkMode(void)
{
    return workMode;
}

extern void ModeOn(STR_EVENT const *event)
{
    switch(event->evt) {
    case ev1ST:
        break;

    case evHST:
        break;

    case evURegSave:
        romflash_user_reg_save();
        break;

    case evGUserReg:
        romflash_user_reg_get();
        break;

    //case evFctyRegSave:
    //	break;

    case evGFctyReg:
        romflash_reg_refresh();
        break;


    case evRst:
        //RstMode();
        printf("RESET_KEY event\r\n");
        break;

    default:
        break;
    }
}



extern void powerReset(STR_EVENT const *event)
{
    switch(event->evt) {
    case ev1ST:
        break;

    case evHST:
        break;

    case evRst:
        //RstMode();
        printf("RESET_KEY event\r\n");
        break;

    default:
        break;
    }
}

extern void ModeKey(STR_EVENT const *event)
{
	if(Get_Key_Feature_Flag()){
		switch(event->evt) {
			case evKeyUP:
				menuPollButtons(evKeyUP);   //evKeyUP
					printf("evKeyUP event\r\n");
	        break;

	    case evKeyDown:
			menuPollButtons(evKeyDown);   //evKeyDown
					printf("evKeyDown event\r\n");
	        break;

	    case evKeyConfirm:
			menuPollButtons(evKeyConfirm);   //evKeyConfirm
	        printf("evKeyConfirm event\r\n");
	        break;
			
			case evKeyCancal:
			menuPollButtons(evKeyCancal);   //evKeyCancal
	        printf("evKeyCancal event\r\n");
	        break;
			
	    default:
	        break;
	  }
	}else{
		ClearAllEvent();
	}
}

