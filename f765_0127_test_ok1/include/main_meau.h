



#define Display_Line_Width 20





#define En_Or_Dis_NUM 5
#define CONFIG_FORMAT_NUM 17
#define InputNum 3
#define OutputNum 2
#define IP_NUM 4
#define MAC_ARR_NUM 6
#define UTCOFFSET_NUM 1
#define SRC_MODE_NUM 2
#define SRC_SYNV_TYPE_NUM 12
#define SRC_CLOCK_TYPE_NUM 3
#define SRC_SYNC_FRAME_NUM 6
#define DISPLAY_MODE_NUM 3
#define DISPLAY_BRIGHTNESS_NUM 2

#define RESTORE_NUM 1

//#define MAC_ARR1_NUM 3
//#define MAC_ARR2_NUM 3   



#define InputNum_ADDR                    (ADDR_FLASH_SECTOR_11 + 8)    //有些 扇区写不进去？
#define OutputNum_ADDR                   (InputNum_ADDR + InputNum+8) /* Start @ of user Flash area */
#define SRC_MODE_NUM_ADDR                (OutputNum + OutputNum_ADDR+8)
#define SRC_SYNV_TYPE_NUM_ADDR           (SRC_MODE_NUM + SRC_MODE_NUM_ADDR+8)  
#define DISPLAY_MODE_NUM_ADDR            (SRC_SYNV_TYPE_NUM + SRC_SYNV_TYPE_NUM_ADDR+8)  /* Start @ of user Flash area */
#define DISPLAY_BRIGHTNESS_NUM_ADDR      (DISPLAY_MODE_NUM + DISPLAY_MODE_NUM_ADDR+8)
#define En_Or_Dis_NUM_ADDR               (DISPLAY_BRIGHTNESS_NUM+DISPLAY_BRIGHTNESS_NUM_ADDR+8)
#define CONFIG_FORMAT_NUM_ADDR           (En_Or_Dis_NUM+En_Or_Dis_NUM_ADDR+8)
#define RESTORE_NUM_ADDR                 (CONFIG_FORMAT_NUM+CONFIG_FORMAT_NUM_ADDR+8)
#define UTCOFFSET_NUM_ADDR               (RESTORE_NUM + RESTORE_NUM_ADDR + 8)
#define IP_NUM_ADDR              				 (UTCOFFSET_NUM + UTCOFFSET_NUM_ADDR + 8)
//#define MAC_ARR1_NUM_ADDR              	 (IP_NUM + IP_NUM_ADDR + 8)
//#define MAC_ARR2_NUM_ADDR              	 (MAC_ARR1_NUM + MAC_ARR1_NUM_ADDR + 8)
#define SRC_CLOCK_TYPE_NUM_ADDR          (IP_NUM + IP_NUM_ADDR + 8)
#define SRC_SYNC_FRAME_NUM_ADDR          (SRC_CLOCK_TYPE_NUM + SRC_CLOCK_TYPE_NUM_ADDR + 8)
#define MAC_ARR_NUM_ADDR              	 (SRC_SYNC_FRAME_NUM + SRC_SYNC_FRAME_NUM_ADDR + 8)


//清空数组
extern void Clear_Arr(void);  

extern uint8_t Get_vid_link_flag(void);
extern void Set_vid_link_flag(uint8_t flag);


//主界面
extern uint8_t Dsp_interface(uint8_t num);

extern void Pollint_Display_Main_menu(uint8_t key_mode);

extern void Set_cnt_down(uint16_t cnt);

extern void set_blink_flag(void);

extern void Set_Key_Feature(uint8_t key_feature);
extern uint8_t Get_Key_Feature_Flag(void);


