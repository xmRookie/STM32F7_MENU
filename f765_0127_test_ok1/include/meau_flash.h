

//get
extern uint8_t Get_Update_Flash_Flag(void);
//set

extern void Set_Update_Flash_Flag(uint8_t flag);



extern uint8_t  Get_Status(void);
//extern uint8_t Get_Error_Status(void);
//set
///extern void Set_Error_Staus(uint8_t status);
extern void Set_Status(uint8_t status)  ;
//extern void Set_FPGA_Reg_config(uint32_t sector_addr,uint8_t reg_addr);




//flash
extern void Read_Flash_IP_MAC(void);
//extern uint8_t Read_Flash(void);
extern void Restore_Flash(void);
//extern void Write_Flash(uint32_t sector_addr,uint8_t arr[],uint8_t total);
//extern void Test_Write_Flash_1(void);
//extern void Test_Write_Flash_2(void);

//extern void SwitchOver_Func_1(uint8_t arr[],uint8_t total,uint8_t num,uint32_t sector_addr,uint8_t display_flag);
//extern void SwitchOver_Func(uint8_t arr[],uint8_t total,uint8_t num,uint32_t sector_addr,uint8_t display_flag);

//extern void Dsp_Config_Video_Input_Dis(uint8_t arr[],uint8_t total) ;

extern void En_Or_Dis_UpData(void);


//eth
extern void Send_User_Data(uint8_t *Command_Arr,uint16_t Len);
extern void Update_User_Arr(uint8_t start_pos,uint8_t len,uint8_t complete_update_flag);
extern void Update_Flash_Arr(uint8_t *user_arr,uint16_t pos);















extern void Write2Flash(void);
extern uint8_t ReadFlash2Arr(void);
extern void Write2Flash_Send2FPGA(uint32_t sector_addr);
extern void Set_FPGA_Reg_Config(uint32_t sector_addr,uint8_t reg_addr);

//get
extern uint8_t Get_Config_Format(void);
extern uint8_t Get_Config_Video_Input(void);
extern uint8_t Get_Config_Video_Output(void);
extern int8_t Get_UTCoffset_Num(void);
extern uint8_t Get_IP_Fragment(uint8_t fragment);
extern uint8_t Get_MAC_Fragment(uint8_t fragment);
extern uint8_t Get_SRC_MODE(void);
extern uint8_t Get_Config_SRC_synctype(void);
extern uint8_t Get_Config_SRC_clocktype(void);
extern uint8_t Get_Config_SRC_syncframe(void);
extern uint8_t Get_Config_Display_mode(void);
extern uint8_t Get_Config_Display_Brightness(void);

extern uint8_t Get_GPS_Display_Status(void);
//set
extern void Set_Config_Format(uint8_t pos);
extern void Set_Config_Video_Input(uint8_t pos);
extern void Set_Config_Video_Output(uint8_t pos);
extern void Set_UTCoffset_Num(int8_t num);
extern void Set_SRC_MODE(uint8_t mode);
extern void Set_Config_SRC_synctype(uint8_t pos);
extern void Set_Config_SRC_clocktype(uint8_t pos);
extern void Set_Config_SRC_syncframe(uint8_t pos);
extern void Set_Config_Display_mode(uint8_t pos);





