#include "common.h"


extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef  status;
unsigned char hisedid[128]={0x00};
	
const uint8_t io_map_0x98         =  0x98;//0x98>>1;    //DEFAULT IO MAP ADDRESS
const uint8_t cec_map_0x80        =  0x80;//0x80>>1;    //@IOMAP[0xF4]CEC Map Address set to 0x80
const uint8_t infoframe_map_0x7c  =  0x7c;//0x7c>>1;    //@IOMAP[0xF5]Infoframe Map Address set to 0x7C
const uint8_t dpll_map_0x4c       =  0x4c;//0x4c>>1;    //@IOMAP[0xF8]DPLL Map Address set to 0x4C
const uint8_t ksv_map_0x64        =  0x64;//0x64>>1;    //@IOMAP[0xF9]Repeater Map Address set to 0x64
const uint8_t edid_map_0x6c       =  0x6c;//0x6c>>1;    //@IOMAP[0xFA]EDID Map Address set to 0x6C
const uint8_t hdmi_map_0x68       =  0x68;//0x68>>1;    //@IOMAP[0xFB]HDMI Map Address set to 0x68
const uint8_t cp_map_0x44         =  0x44;//0x44>>1;    //@IOMAP[0xFD]CP Map Address set to 0x44

uint8_t testdata[5]={0xa5,0xa5,0xa5,0xa5,0xa5};

/******************************I2C****************************/

uint8_t slaveaddr;

void set_slave_addr(uint8_t addr)
{
   slaveaddr = addr;
}

void Adv_WriteReg(uint8_t slaveaddr,uint8_t regaddr, uint8_t *data,uint8_t n)
{
  HAL_I2C_Mem_Write(&hi2c1, slaveaddr, regaddr, 1, data, n, 50);
}

void Adv_ReadReg(uint8_t slaveaddr, uint8_t regaddr,uint8_t *data,uint8_t n)
{
   HAL_I2C_Mem_Read(&hi2c1, slaveaddr,regaddr , 1 , data, n, 50);
}


void adv_read_write(uint8_t reg, uint8_t data)
{
  uint8_t readdata;
	Adv_WriteReg(slaveaddr,reg,&data,1);
  Adv_ReadReg(slaveaddr,reg,&readdata,1);
}

/****************************EDID**************************/
void set_edid_data(unsigned char* edid, int edidlen)
{
		int i;
		
//////operation in HDMI map
	set_slave_addr(hdmi_map_0x68);   //set i2c acess address	
	adv_read_write(0x6C, 0xA3); // enable manual HPA

//////operation in io map
	set_slave_addr(io_map_0x98);   //set i2c acess address
	adv_read_write(0x20, 0x70); // HPD low

//////operation in KSV map
	set_slave_addr(ksv_map_0x64);   //set i2c acess address	
	adv_read_write(0x74, 0x00); // disable internal EDID
	

	set_slave_addr(edid_map_0x6c);   //set edid acess address

	for(i=0; i<edidlen; i++)
	{
		adv_read_write(i, edid[i]);
	}

   
//////operation in KSV map
	set_slave_addr(ksv_map_0x64);   //set i2c acess address	
	adv_read_write(0x74, 0x01); // enable internal EDID

//////operation in io map
	set_slave_addr(io_map_0x98);   //set i2c acess address
	adv_read_write(0x20, 0xF0); // HPD high

//////operation in HDMI map
	set_slave_addr(hdmi_map_0x68);   //set i2c acess address	
	adv_read_write(0x6C, 0xA2); // disable manual HPA	

}



/***********************************************************/
int check_adv()
{   
	  uint8_t readdata;
	
    Adv_ReadReg(slaveaddr,0xEB,&readdata,1);
	
	  if(readdata == 0xc1)//C1 adv7619
		 return 0;
		else return -1;
} 



int Set_Adv(void)
{
  
	uint8_t readdata = 0xff;
	
  set_slave_addr(io_map_0x98);   //set i2c acess address
	Adv_ReadReg(slaveaddr,0xff,&readdata,1); //Reset Controls
	
	adv_read_write(0xff, 0x80);       //i2c reset
 	HAL_Delay(1000);
 
	if(check_adv() < 0)
	return -1;
	#if 0
	adv_read_write(0xF4, 0x80);	       	//cec map i2c address
	adv_read_write(0xF5, 0x7C);	       	//infofrmae map i2c address
	adv_read_write(0xF8, 0x4c);	       	//dpll map i2c address
	adv_read_write(0xF9, 0x64);	       	//ksv map i2c address
	adv_read_write(0xFA, 0x6c);	       	//edid map i2c address
	adv_read_write(0xFB, 0x68);	       	//hdmi map i2c address
	adv_read_write(0xFD, 0x44);	      	//cp map i2c address	
	
	adv_read_write(0x00, 0x19);         //Set VID_STD for 2x1 mode
	adv_read_write(0x01, 0x05);         //Prim_Mode =101b HDMI-Comp   
	adv_read_write(0x03, 0x95);        //20-bit SDR 4:2:2 interleaved Mode 0
	
	/*
     	                      寄存器0x03
	0x00 		8-bit SDR ITU-656 mode
	0x01		10-bit SDR ITU-656 mode
	0x02		12-bit SDR ITU-656 Mode 0
	0x06		12-bit SDR ITU-656 Mode 1
	0x0A		12-bit SDR ITU Mode 2
	0x20		8-bit 4:2:2 DDR Mode
	0x21		10-bit 4:2:2 DDR Mode
	0x22		12-bit 4:2:2 DDR Mode 0
	0x23		12-bit 4:2:2 DDR Mode 1
	0x24		12-bit 4:2:2 DDR Mode 2
	0x40		24-bit 4:4:4 SDR Mode
	0x41		30-bit 4:4:4 SDR Mode
	0x42		36-bit 4:4:4 SDR Mode 0
	0x46		36-bit SDR 4:4:4 Mode 1
	0x4C		24-bit SDR 4:4:4 Mode 3
	0x50		24-bit SDR 4:4:4 Mode 4
	0x51		30-bit SDR 4:4:4 Mode 4
	0x52		36-bit SDR 4:4:4 Mode 4
	0x54		24-bit SDR 4:4:4 interleaved Mode 0
	0x60		24-bit 4:4:4 DDR mode
	0x61		30-bit 4:4:4 DDR mode
	0x62		36-bit 4:4:4 DDR mode
	0x80		16-bit ITU-656 SDR mode
	0x81		20-bit ITU-656 SDR mode
	0x82		24-bit ITU-656 SDR Mode 0
	0x86		24-bit ITU-656 SDR Mode 1
	0x8A		24-bit ITU-656 SDR Mode 2
	0x90		16-bit SDR 4:2:2 Mode 4
	0x94		16-bit SDR 4:2:2 interleaved Mode 0
	0x95		20-bit SDR 4:2:2 interleaved Mode 0
	0x96		24-bit SDR 4:2:2 interleaved Mode 0
	
	*/
	adv_read_write(0x05, 0x28); 				//AV Codes Off
	adv_read_write(0x06, 0xa0); 				//No inversion on VS,HS pins *
	adv_read_write(0x0c, 0x42);					//Power up part
	adv_read_write(0x15, 0x80);					//Disable Tristate of Pins
	adv_read_write(0x19, 0x83);					//LLC DLL phase *
	adv_read_write(0x33, 0x40);					//LLC DLL MUX enable
	adv_read_write(0xDD, 0x00); 				//ADI Required Write
	adv_read_write(0xE7, 0x04); 				//ADI Required Write
	

	
//////operation in CP map
	set_slave_addr(cp_map_0x44);   	//set i2c acess address	
	
	adv_read_write(0xba, 0x01);    //Enable the free run feature in HDMI mode
// CHANGE FOR LCD
 // adv_read_write(0x7b, 0xc5);
	adv_read_write(0x7c, 0x01); 		  ///HS VS invert 
  adv_read_write(0x7e, 0xa6);  		  //HS START toward ->166
	adv_read_write(0x7d, 0x12);  	  	//HS END toward00

	//adv_read_write(0x7f, 0x77);  		//VS  START(H4bit)   END(L4bit)  0-7 towards   8-f away from 

///CHANGE FOR LCD
  ///adv_read_write(0xbf, 0x13);  //force cp free run
//	adv_read_write(0xff, 0x10);  //cp free run
	
//////operation in KSV map
	set_slave_addr(ksv_map_0x64);   //set i2c acess address	
	adv_read_write(0x40, 0x81);     //01000001

//////operation in DPPL map
	set_slave_addr(dpll_map_0x4c);
	//TMDS >170MHz
	adv_read_write(0xB5, 0x01);     //Setting MCLK to 256Fs
	adv_read_write(0xC3, 0x80);     //ADI Required Write
	adv_read_write(0xCF, 0x03);     //ADI Required Write
	adv_read_write(0xDB, 0x80);     //ADI Required Write
	
//////operation in HDMI map
	set_slave_addr(hdmi_map_0x68);   //set i2c acess address	
	//TMDS >170MHz
	adv_read_write(0xC0, 0x03);			//ADI Required Write
	adv_read_write(0x00, 0x08);			//Set HDMI Input Port A (BG_MEAS_PORT_SEL = 001b)
	adv_read_write(0x02, 0x03);			//ALL BG Ports enabled
	adv_read_write(0x03, 0x98);			//ADI Required Write
	adv_read_write(0x10, 0xA5);			//ADI Required Write
	adv_read_write(0x1B, 0x00);			//ADI Required Write
	adv_read_write(0x45, 0x04);			//ADI Required Write
	adv_read_write(0x97, 0xC0);			//ADI Required Write
	adv_read_write(0x3E, 0x69);			//ADI Required Write
	adv_read_write(0x3F, 0x46);			//ADI Required Write
	adv_read_write(0x4E, 0xFE);			//ADI Required Write
	adv_read_write(0x4F, 0x08);			//ADI Required Write
	adv_read_write(0x50, 0x00);			//ADI Required Write
	adv_read_write(0x57, 0xA3);			//ADI Required Write
	adv_read_write(0x58, 0x07);			//ADI Required Write
	adv_read_write(0x6F, 0x08); 		//ADI Required Write
	adv_read_write(0x83, 0xFC);			//Enable clock terminators for port A & B
	adv_read_write(0x84, 0x03);			//FP MODE
	adv_read_write(0x85, 0x10);			//ADI Required Write
	adv_read_write(0x86, 0x9B);			//ADI Required Write
	adv_read_write(0x89, 0x03);			//HF Gain
	adv_read_write(0x9B, 0x03);			//ADI Required Write
	adv_read_write(0x93, 0x03);			//ADI Required Write
	adv_read_write(0x5A, 0x80);			//ADI Required Write
	adv_read_write(0x9C, 0x80);			//ADI Required Write
	adv_read_write(0x9C, 0xC0);			//ADI Required Write
	adv_read_write(0x9C, 0x00);			//ADI Required Write
#else
	adv_read_write(0xF4, 0x80);	       	//cec map i2c address
	adv_read_write(0xF5, 0x7C);	       	//infofrmae map i2c address
	adv_read_write(0xF8, 0x4c);	       	//dpll map i2c address
	adv_read_write(0xF9, 0x64);	       	//ksv map i2c address
	adv_read_write(0xFA, 0x6c);	       	//edid map i2c address
	adv_read_write(0xFB, 0x68);	       	//hdmi map i2c address
	adv_read_write(0xFD, 0x44);	      	//cp map i2c address	
	
	//adv_read_write(0x00, 0x19);         //Set VID_STD for 2x1 mode
	adv_read_write(0x01, 0x06);         //Prim_Mode =110b HDMI-GR
	adv_read_write(0x02, 0xF5);         //Auto CSC, YCrCb out, Set op_656 bit	
	adv_read_write(0x03, 0x80);        //16-bit ITU-656 SDR mode
	
	/*
     	                      寄存器0x03
	0x00 		8-bit SDR ITU-656 mode
	0x01		10-bit SDR ITU-656 mode
	0x02		12-bit SDR ITU-656 Mode 0
	0x06		12-bit SDR ITU-656 Mode 1
	0x0A		12-bit SDR ITU Mode 2
	0x20		8-bit 4:2:2 DDR Mode
	0x21		10-bit 4:2:2 DDR Mode
	0x22		12-bit 4:2:2 DDR Mode 0
	0x23		12-bit 4:2:2 DDR Mode 1
	0x24		12-bit 4:2:2 DDR Mode 2
	0x40		24-bit 4:4:4 SDR Mode
	0x41		30-bit 4:4:4 SDR Mode
	0x42		36-bit 4:4:4 SDR Mode 0
	0x46		36-bit SDR 4:4:4 Mode 1
	0x4C		24-bit SDR 4:4:4 Mode 3
	0x50		24-bit SDR 4:4:4 Mode 4
	0x51		30-bit SDR 4:4:4 Mode 4
	0x52		36-bit SDR 4:4:4 Mode 4
	0x54		24-bit SDR 4:4:4 interleaved Mode 0
	0x60		24-bit 4:4:4 DDR mode
	0x61		30-bit 4:4:4 DDR mode
	0x62		36-bit 4:4:4 DDR mode
	0x80		16-bit ITU-656 SDR mode
	0x81		20-bit ITU-656 SDR mode
	0x82		24-bit ITU-656 SDR Mode 0
	0x86		24-bit ITU-656 SDR Mode 1
	0x8A		24-bit ITU-656 SDR Mode 2
	0x90		16-bit SDR 4:2:2 Mode 4
	0x94		16-bit SDR 4:2:2 interleaved Mode 0
	0x95		20-bit SDR 4:2:2 interleaved Mode 0
	0x96		24-bit SDR 4:2:2 interleaved Mode 0
	
	*/
	adv_read_write(0x05, 0x28); 				//AV Codes Off
	adv_read_write(0x06, 0xA6); 				//Invert VS,HS pins *
	adv_read_write(0x0c, 0x42);					//Power up part
	adv_read_write(0x15, 0x80);					//Disable Tristate of Pins
	adv_read_write(0x19, 0x83);					//LLC DLL phase *
	adv_read_write(0x33, 0x40);					//LLC DLL MUX enable
	
//////operation in CP map
	set_slave_addr(cp_map_0x44);   	//set i2c acess address	
	
	adv_read_write(0xba, 0x01);    //Set HDMI FreeRun
// CHANGE FOR LCD
 // adv_read_write(0x7b, 0xc5);
	adv_read_write(0x6c, 0x00); 		  //Required ADI write

	//adv_read_write(0x7f, 0x77);  		//VS  START(H4bit)   END(L4bit)  0-7 towards   8-f away from 

///CHANGE FOR LCD
  ///adv_read_write(0xbf, 0x13);  //force cp free run
//	adv_read_write(0xff, 0x10);  //cp free run
	
//////operation in KSV map
	set_slave_addr(ksv_map_0x64);   //set i2c acess address	
	adv_read_write(0x40, 0x81);     //Disable HDCP 1.1 features

//////operation in DPPL map
	set_slave_addr(dpll_map_0x4c);
	//TMDS <170MHz
	adv_read_write(0xB5, 0x01);     //Setting MCLK to 256Fs

//////operation in HDMI map
	set_slave_addr(hdmi_map_0x68);   //set i2c acess address	
	//TMDS <170MHz
	adv_read_write(0xC0, 0x03);			//ADI Required Write
	adv_read_write(0x00, 0x08);			//Set HDMI Input Port A (BG_MEAS_PORT_SEL = 001b)
	adv_read_write(0x02, 0x03);			//ALL BG Ports enabled
	adv_read_write(0x03, 0x98);			//ADI Required Write
	adv_read_write(0x10, 0xA5);			//ADI Required Write
	adv_read_write(0x1B, 0x08);			//ADI Required Write
	adv_read_write(0x45, 0x04);			//ADI Required Write
	adv_read_write(0x97, 0xC0);			//ADI Required Write
	adv_read_write(0x3E, 0x69);			//ADI Required Write
	adv_read_write(0x3F, 0x46);			//ADI Required Write
	adv_read_write(0x4E, 0xFE);			//ADI Required Write
	adv_read_write(0x4F, 0x08);			//ADI Required Write
	adv_read_write(0x50, 0x00);			//ADI Required Write
	adv_read_write(0x57, 0xA3);			//ADI Required Write
	adv_read_write(0x58, 0x07);			//ADI Required Write
	adv_read_write(0x6F, 0x08); 		//ADI Required Write
	adv_read_write(0x83, 0xFC);			//Enable clock terminators for port A & B
	adv_read_write(0x84, 0x03);			//FP MODE
	adv_read_write(0x85, 0x10);			//ADI Required Write
	adv_read_write(0x86, 0x9B);			//ADI Required Write
	adv_read_write(0x89, 0x03);			//HF Gain
	adv_read_write(0x9B, 0x03);			//ADI Required Write
	adv_read_write(0x93, 0x03);			//ADI Required Write
	adv_read_write(0x5A, 0x80);			//ADI Required Write
	adv_read_write(0x9C, 0x80);			//ADI Required Write
	adv_read_write(0x9C, 0xC0);			//ADI Required Write
	adv_read_write(0x9C, 0x00);			//ADI Required Write
//////以下addr 72在寄存器手册中没有,但在脚本中出现，实验时没用到addr 72但是可用
	/*set_slave_addr(0x72);
	adv_read_write(0x01, 0x00);			//Set N Value(6144)
	adv_read_write(0x02, 0x18);			//Set N Value(6144)
	adv_read_write(0x03, 0x00);			//Set N Value(6144)
	adv_read_write(0x15, 0x01);			//Input 422 (YCrCb) with Separate Syncs, 44.1kHz fs
	adv_read_write(0x16, 0xED);			//YCrCb 422 Style 3
	adv_read_write(0x18, 0x46);			//CSC disabled
	adv_read_write(0x3B, 0x80);			//PR Auto Mode
	adv_read_write(0x40, 0x80);			//General Control Packet Enable
	adv_read_write(0x41, 0x10);			//Power Down control
	adv_read_write(0x48, 0x08);			//Data right justified
	adv_read_write(0x49, 0xA8);			//Set Dither_mode - 12-to-10 bit
	adv_read_write(0x4C, 0x04);			//8 bit Output
	adv_read_write(0x55, 0x20);			//Set YCrCb 422 in AVinfo Frame
	adv_read_write(0x56, 0x08);			//Set active format Aspect
	adv_read_write(0x96, 0x20);			//HPD Interrupt clear
	adv_read_write(0x98, 0x03);			//ADI Required Write
	adv_read_write(0x99, 0x02);			//ADI Required Write
	adv_read_write(0x9C, 0x30);			//PLL Filter R1 Value
	adv_read_write(0x9D, 0x61);			//Set clock divide
	adv_read_write(0xA2, 0xA4);			//ADI Required Write
	adv_read_write(0xA3, 0xA4);			//ADI Required Write
	adv_read_write(0xAF, 0x16);			//Set HDMI Mode
	adv_read_write(0xBA, 0x60);			//No clock delay
	adv_read_write(0xDE, 0x9C);			//ADI Required Write
	adv_read_write(0xE4, 0x60);			//VCO Swing Reference Voltage
	adv_read_write(0xFA, 0x7D);			//Nbr of times to search for good phase
	*/
#endif

//////operation in edid map
	set_edid_data(hisedid, 128);
	

	return 0;   
}




void advTask(void)
{  
	set_slave_addr(io_map_0x98);  
  while(check_adv() != 0)
	{
		
	}
	Set_Adv();
	 
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	 
	while(1)
	{
		Adv_ReadReg(0x68,0x07,testdata,1);
		Adv_ReadReg(0x68,0x08,testdata+1,1);
		Adv_ReadReg(0x68,0x2e,testdata+2,1);
		Adv_ReadReg(0x68,0x2f,testdata+3,1);
		Adv_ReadReg(0x98,0x6a,testdata+4,1);
		
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_3);
		HAL_Delay(500);
	}
	
	
}