#include "tlv320aic3268.h"
#include "his_iic.h"
#include "sys.h"
#include "his_gpio.h"

typedef struct EXT_3268_DEV_E 
{
// 
  HIS_IIC_HNDL      iicHndl;
  HIS_GPIO_DEV_E    sel;
  // statute
  WV_U8       		iicBus;
  WV_U8       		iicAdr;
  //
  WV_U8             liCtl[5];

  WV_U8             hpCtl;
  WV_U8             loCtl;
  
}EXT_3268_DEV_E ;

typedef struct EXT_MIC_DEV_E 
{
// 

  HIS_GPIO_DEV_E    sel[4];
  // statute
   WV_U8             micCtl[4];
   
}EXT_MIC_DEV_E ;

static   EXT_3268_DEV_E   gCodecDev[3];
static   EXT_MIC_DEV_E    gMicDev;
static   HIS_GPIO_DEV_E   gAudoRest;

static   unsigned char ext_mux_ena[4];
/**********************************************************************************

WV_S32 EXT_3268_DevInit(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_DevInit(EXT_3268_DEV_E * pDev )
{ 
    WV_CHECK_RET(HIS_IIC_Open  (&pDev->iicHndl, pDev->iicBus));
    HIS_GPIO_SetCfg(pDev-> sel,0);
    HIS_GPIO_Set(pDev-> sel,0);

   
 return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_3268_DevDeInit(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_DevDeInit(EXT_3268_DEV_E * pDev )
{ 
     WV_CHECK_RET( HIS_IIC_Close ( &pDev->iicHndl) ); 
 return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_3268_Read(EXT_3268_DEV_E * pDev ,WV_U8 *pData);

**********************************************************************************/
WV_S32 EXT_3268_Read(EXT_3268_DEV_E * pDev ,WV_U8 reg,WV_U8 *pData)
{ 
 
   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev->iicAdr, reg, pData, 1) );
 return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_3268_Write(EXT_3268_DEV_E * pDev ,WV_U8 data);

**********************************************************************************/
WV_S32 EXT_3268_Write(EXT_3268_DEV_E * pDev ,WV_U8 reg,WV_U8 data)
{ 
   WV_printf("3268_Write  fd[%d]  dev[%3x]  reg[%#x]= %d [%#x]\r\n",pDev->iicHndl.fd,pDev->iicAdr,reg,data,data);
   WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev->iicAdr, reg,&data, 1) );
 return WV_SOK;	
}



/**********************************************************************************

WV_S32 EXT_3268_Sel(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_Sel(EXT_3268_DEV_E * pDev )
{ 
 
   WV_CHECK_RET( HIS_GPIO_Set(pDev-> sel,1)  );
   usleep(100000);
 return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_3268_DeSel(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_DeSel(EXT_3268_DEV_E * pDev )
{ 
    usleep(100000);
    WV_CHECK_RET( HIS_GPIO_Set(pDev->sel,0)  );
    usleep(100000);
 return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_3268_DevStart(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_DevStart(EXT_3268_DEV_E * pDev)
{  
   EXT_3268_Sel(pDev );  //sel IIC bus
   
     
      WV_U8  temp;
  	 EXT_3268_Read(pDev,0x00,&temp);
   // Software Reset
   EXT_3268_Write(pDev,0,0);
   EXT_3268_Write(pDev,0x7f,0);
   EXT_3268_Write(pDev,0x01,0x01);
   usleep(100000);
   
   //FIFO Configuration
    EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
    EXT_3268_Write(pDev,0x7f ,0x78);//Select Book 120
    EXT_3268_Write(pDev,0x32 ,0x80);//Enable DAC FIFO
    EXT_3268_Write(pDev,0x7f ,0x64);//Select Book 100
    EXT_3268_Write(pDev,0x32 ,0x80);//Enable ADC FIFO

    EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
    EXT_3268_Write(pDev,0x7f ,0x00);//Select Book 0
    
   // Power and Analog Configuration 
   EXT_3268_Write(pDev,0x00,0x04);//Select Page 4
   EXT_3268_Write(pDev,0x77,0xc0);//Disable miniDSP power-up sync with ASI
    EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
   EXT_3268_Write(pDev,0x0d,0x00);//# Program DOSR = 128
   EXT_3268_Write(pDev,0x0e,0x80);//# Program DOSR = 128
   EXT_3268_Write(pDev,0x14,0x80);//Program AOSR = 128
   EXT_3268_Write(pDev,0x00,0x01);//Select Page 1
   EXT_3268_Write(pDev,0x01,0x00);//Disable weak AVDD to DVDD connection, make analog supplies available
  EXT_3268_Write(pDev,0x23,0x30);//
   EXT_3268_Write(pDev,0x4d,0x00);// HP OCP Configuration
   EXT_3268_Write(pDev,0x08,0x00);//Full chip CM = 0.9V
   EXT_3268_Write(pDev,0x03,0x00);//PTM_P3/P4
   EXT_3268_Write(pDev,0x04,0x00);//PTM_P3/P4 
   // Clock configuration
   // MCLK = 12.288 MHz, BCLK = 3.072 MHz, WCLK = 48 kHz  
	EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
	EXT_3268_Write(pDev,0x04,0x00); //Set DAC_CLKIN as MCLK -- default not mandatory to program
	EXT_3268_Write(pDev,0x0b,0x81); //NDAC = 1
	EXT_3268_Write(pDev,0x0c,0x82); //MDAC = 2
	EXT_3268_Write(pDev,0x0d,0x00); //Program the OSR of DAC to 128 to get
	EXT_3268_Write(pDev,0x0e,0x80); //DAC_FS = DAC_MOD_CLK / DOSR = 6.144MHz / 128 = 48kHz 

  
  //Audio Serial Interface Routing Configuration - Audio Serial Interface #1
  //ASI #1 playback  mast? 
	EXT_3268_Write(pDev,0x00,0x04);//Select Page 4
	EXT_3268_Write(pDev,0x01,0x00);//I2S mode, 16-bit
	EXT_3268_Write(pDev,0x0a,0x24);//Route ASI#1 WCLK and BCLK to WCLK1 pin and BCLK1 pin
	                               //D7-D5 001: WCLK1 pin is Word Clock output from ASI1
	                               //D4-D2  001: BCLK1 pin is Bit Clock output from ASI1     
	EXT_3268_Write(pDev,0x08,0x50);//Left Channel DAC and Primary ASI's Right channel data to Right Channel DAC 

  //  Signal Processing Settings 
  EXT_3268_Write(pDev,0x00,0x00);//Select Page 0 
  EXT_3268_Write(pDev,0x3c,0x01); //Set the DAC Mode to PRB_P1
  // Output Channel Configuration

  


  EXT_3268_Write(pDev,0x00,0x00);//Select Page 0 
  EXT_3268_Write(pDev,0x3f,0xc0);//Power up the Left and Right DAC Channels with route the Primary
                                 //ASI's left channel data to Left DAC and right channel to Right DAC
  EXT_3268_Write(pDev,0x40,0x00);  // # Unmute the DAC digital volume control


  EXT_3268_Write(pDev,0x00,0x01);  // Select Page 1  
  EXT_3268_Write(pDev,0x08,0x00);  // 

  EXT_3268_Write(pDev,0x09,0x00);  // HP Sizing = 100%
  EXT_3268_Write(pDev,0x1f,0x00);  // Headphone in Ground-centered Mode, HPL Gain=0dB  ?????????????
  EXT_3268_Write(pDev,0x20,0x80);  //HPR To have same gain as HPL, set to 0dB

  EXT_3268_Write(pDev,0x11,0x0c); // Power-on left and right analog mixers (MAL, MAR) 
  if(pDev-> hpCtl == 1) {
  	 EXT_3268_Write(pDev,0x1b,0xf3);  //Enable DAC to HPL/R and power-up HPL/R  MAL/R //  ?
  	                     // Wait for reference to power up
  	}
 
  EXT_3268_Write(pDev,0x2d,0x02); // SPK driver powered
  EXT_3268_Write(pDev,0x28,0xf9); //RECP powered
  EXT_3268_Write(pDev,0x16,0x03); //LOL and LOR powered
   usleep(40000);

  //Audio Bypass Routing Configuration
  //EXT_3268_Write(pDev,0x00,0x01);//Select Page 1 
  
  EXT_3268_Write(pDev,0x34,0x20);//Connect IN2L(Mic_In) to PGA with 20k input //?
  EXT_3268_Write(pDev,0x37,0x20);//Connect IN2R(Mic_In) to PGA with 20k input //?
  EXT_3268_Write(pDev,0x36,0x80);//Set Left common mode input resistor to 20k
  EXT_3268_Write(pDev,0x39,0x80);//Set Right common mode input resistor to 20k
  EXT_3268_Write(pDev,0x12,0x00);//Connect left PGA to left analog mixer
  EXT_3268_Write(pDev,0x13,0x00); //Connect right PGA to right analog mixer
  EXT_3268_Write(pDev,0x3b,0x00); //  Unmute left PGA
  EXT_3268_Write(pDev,0x3c,0x00); // Unmute right PGA

  //usleep(4000000); 

  
  if(pDev-> loCtl == 1){

        EXT_3268_Write(pDev,0x17,0xc0); //Enable MAL/R to LOL/R     //  ?
	    EXT_3268_Write(pDev,0x16,0xc3); //Enable DAC to LOL/R and power-up LOL/R    //  ?
	    usleep(40000);                   // Wait for reference to power up
	 } 

//usleep(4000000); 
 
 //WV_U8  temp;
  	// EXT_3268_Read(pDev,0x00,&temp); 
//set input	 
  WV_U8  regP1R52;
  WV_U8  regP1R53;
  WV_U8  regP1R55;
  WV_U8  regP1R56; 
 
  regP1R52   =  (pDev-> liCtl[0] & 0X03) << 6;
  regP1R52  |=  (pDev-> liCtl[1] & 0X03) << 4;
  regP1R52  |=  (pDev-> liCtl[2] & 0X03) << 2;
  regP1R53  =  (pDev-> liCtl[3] & 0X01)  << 5;
  
  regP1R55   =  (pDev-> liCtl[0] & 0X03) << 6;
  regP1R55  |=  (pDev-> liCtl[1] & 0X03) << 4;
  regP1R55  |=  (pDev-> liCtl[2] & 0X03) << 2;
  regP1R56  =  (pDev-> liCtl[3] & 0X01) << 5;
  
  EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
  EXT_3268_Write(pDev,0x7f ,0x00);//Select Book 0
  EXT_3268_Write(pDev,0x00,0x02);//Select Page  
  EXT_3268_Write(pDev,0x00,0x01);//Select Page  1
  
  EXT_3268_Write(pDev,0x34,regP1R52);//Set Left common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x35,regP1R53);//Set Left common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x37,regP1R55);//Set right common mode input resistor to 20k
  EXT_3268_Write(pDev,0x38,regP1R56);//Set right common mode input resistor to 20k 	 
  

 
   EXT_3268_Write(pDev,0x00,0x00);//Select Page  0	 
 
  EXT_3268_DeSel(pDev ); //desel IICbus
   
 return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_3268_DevStart(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_DevStart_line(EXT_3268_DEV_E * pDev)
{  
   EXT_3268_Sel(pDev );  //sel IIC bus
   
     
      WV_U8  temp;
  	 EXT_3268_Read(pDev,0x00,&temp);
   // Software Reset
   EXT_3268_Write(pDev,0,0);
   EXT_3268_Write(pDev,0x7f,0);
   EXT_3268_Write(pDev,0x01,0x01);
   usleep(100000);
   

    
   EXT_3268_Write(pDev,0x00,0x01);//Select Page 1
   EXT_3268_Write(pDev,0x01,0x00);//Disable weak AVDD to DVDD connection, make analog supplies available
   EXT_3268_Write(pDev,0x23,0x30);//
   EXT_3268_Write(pDev,0x4d,0x00);// HP OCP Configuration
   EXT_3268_Write(pDev,0x08,0x00);//Full chip CM = 0.9V
   //EXT_3268_Write(pDev,0x03,0x00);//PTM_P3/P4
  // EXT_3268_Write(pDev,0x04,0x00);//PTM_P3/P4 
   


   EXT_3268_Write(pDev,0x09,0x00);  // HP Sizing = 100%
  EXT_3268_Write(pDev,0x1f,0x00);  // Headphone in Ground-centered Mode, HPL Gain=0dB  ?????????????
  EXT_3268_Write(pDev,0x20,0x00);  //HPR To have same gain as HPL, set to 0dB

  EXT_3268_Write(pDev,0x11,0x0c); // Power-on left and right analog mixers (MAL, MAR) 
//  if(pDev-> hpCtl == 1) {
  	 EXT_3268_Write(pDev,0x1b,0xf3);  //Enable DAC to HPL/R and power-up HPL/R  MAL/R //  ?
  	                     // Wait for reference to power up
//  	}
 
  //EXT_3268_Write(pDev,0x2d,0x02); // SPK driver powered
 // EXT_3268_Write(pDev,0x28,0xf9); //RECP powered
 // EXT_3268_Write(pDev,0x16,0x03); //LOL and LOR powered
   usleep(40000);

  //Audio Bypass Routing Configuration
  //EXT_3268_Write(pDev,0x00,0x01);//Select Page 1 
  
  //EXT_3268_Write(pDev,0x34,0x20);//Connect IN2L(Mic_In) to PGA with 20k input //?
  //EXT_3268_Write(pDev,0x37,0x20);//Connect IN2R(Mic_In) to PGA with 20k input //?
  EXT_3268_Write(pDev,0x36,0x80);//Set Left common mode input resistor to 20k
  EXT_3268_Write(pDev,0x39,0x02);//Set Right common mode input resistor to 20k
  EXT_3268_Write(pDev,0x12,0x00);//Connect left PGA to left analog mixer
  EXT_3268_Write(pDev,0x13,0x00); //Connect right PGA to right analog mixer
  EXT_3268_Write(pDev,0x3b,0x00); //  Unmute left PGA
  EXT_3268_Write(pDev,0x3c,0x00); // Unmute right PGA

  //usleep(4000000); 

  
  if(pDev-> loCtl == 1){

        EXT_3268_Write(pDev,0x17,0xc0); //Enable MAL/R to LOL/R     //  ?
	    EXT_3268_Write(pDev,0x16,0xc3); //Enable DAC to LOL/R and power-up LOL/R    //  ?
	    usleep(40000);                   // Wait for reference to power up
	 } 

//usleep(4000000); 
 
 //WV_U8  temp;
  	// EXT_3268_Read(pDev,0x00,&temp); 
//set input	 
  WV_U8  regP1R52;
  WV_U8  regP1R53;
  WV_U8  regP1R55;
  WV_U8  regP1R56; 
 
  regP1R52   =  (pDev-> liCtl[0] & 0X03) << 6;
  regP1R52  |=  (pDev-> liCtl[1] & 0X03) << 4;
  regP1R52  |=  (pDev-> liCtl[2] & 0X03) << 2;
  regP1R53  =  (pDev-> liCtl[3] & 0X01)  << 5;
  
  regP1R55   =  (pDev-> liCtl[0] & 0X03) << 6;
  regP1R55  |=  (pDev-> liCtl[1] & 0X03) << 4;
  regP1R55  |=  (pDev-> liCtl[2] & 0X03) << 2;
  regP1R56  =  (pDev-> liCtl[3] & 0X01) << 5;
  
  EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
  EXT_3268_Write(pDev,0x7f ,0x00);//Select Book 0
  EXT_3268_Write(pDev,0x00,0x02);//Select Page  
  EXT_3268_Write(pDev,0x00,0x01);//Select Page  1
  
  EXT_3268_Write(pDev,0x34,regP1R52);//Set Left common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x35,regP1R53);//Set Left common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x37,regP1R55);//Set right common mode input resistor to 20k
  EXT_3268_Write(pDev,0x38,regP1R56);//Set right common mode input resistor to 20k 	 
  

 
   EXT_3268_Write(pDev,0x00,0x00);//Select Page  0	 
 
  EXT_3268_DeSel(pDev ); //desel IICbus
   
 return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_3268_SetIn(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_SetIn(EXT_3268_DEV_E * pDev)
{
  WV_U8  regP1R52;
  WV_U8  regP1R53;
  WV_U8  regP1R55;
  WV_U8  regP1R56; 
 
  regP1R52   =  (pDev-> liCtl[0] & 0X03) << 6;
  regP1R52  |=  (pDev-> liCtl[1] & 0X03) << 4;
  regP1R52  |=  (pDev-> liCtl[2] & 0X03) << 2;
  regP1R53  =  (pDev-> liCtl[3] & 0X01) << 5;
  
  regP1R55   =  (pDev-> liCtl[0] & 0X03) << 6;
  regP1R55  |=  (pDev-> liCtl[1] & 0X03) << 4;
  regP1R55  |=  (pDev-> liCtl[2] & 0X03) << 2;
  regP1R56  =  (pDev-> liCtl[3] & 0X01) << 5;
  
  EXT_3268_Sel(pDev );  //sel IIC bus
  EXT_3268_Write(pDev,0x00,0x00);//Select Page  1
  EXT_3268_Write(pDev,0x7f ,0x00);//Select Book 0
 // EXT_3268_Write(pDev,0x00,0x02);//Select Page  
  EXT_3268_Write(pDev,0x00,0x01);//Select Page  1
  
  EXT_3268_Write(pDev,0x34,regP1R52);//Set Left common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x35,regP1R53);//Set Left common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x37,regP1R55);//Set right common mode input resistor to 20k
  EXT_3268_Write(pDev,0x38,regP1R56);//Set right common mode input resistor to 20k 
  EXT_3268_Write(pDev,0x00,0x00);//Select Page  1 
  EXT_3268_DeSel(pDev );  //desel IIC bus
  return  WV_SOK;
}




/**********************************************************************************

WV_S32 EXT_3268_SetOut(EXT_3268_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_3268_SetOut(EXT_3268_DEV_E * pDev)
{

   EXT_3268_Sel(pDev );  //sel IIC bus
     EXT_3268_Write(pDev,0x00,0x00);//Select Page 0
    EXT_3268_Write(pDev,0x7f ,0x00);//Select Book 0
   EXT_3268_Write(pDev,0x00,0x01);//Select Page  1
   if(pDev-> hpCtl == 1){
     EXT_3268_Write(pDev,0x09,0x00);  // HP Sizing = 100%
//     EXT_3268_Write(pDev,0x1f,0x00);  // Headphone in Ground-centered Mode, HPL Gain=0dB
     EXT_3268_Write(pDev,0x20,0x00);  //HPR To have same gain as HPL, set to 0dB
     EXT_3268_Write(pDev,0x1b,0xf3);  //Enable DAC to HPL/R and power-up HPL/R  MAL/R //  ?
   }  
   else{
    //EXT_3268_Write(pDev,0x1b,0x0);  //power-down HPL/R  MAL/R //    
    }  
   
  if(pDev-> loCtl == 1){                 
     EXT_3268_Write(pDev,0x17,0xc0); //Enable MAL/R to LOL/R     //  ? //Enable MAL/R to LOL/R     //  ?
     EXT_3268_Write(pDev,0x16,0xc3); //Enable DAC to LOL/R and power-down LOL/R    //  ?
    }
  else{
     EXT_3268_Write(pDev,0x16,0xc0); //power-dowwn LOL/R    //  ? 
    }   
 
  EXT_3268_DeSel(pDev );  //desel IIC bus
  return  WV_SOK;
}

/*************************************************** 

WV_S32 EXT_3268_GetMux(WV_U8 *pMuxEna);

***************************************************/
WV_S32 EXT_3268_GetMux(WV_U8 *pMuxEna)
{
	memcpy(pMuxEna,ext_mux_ena,4);
	return WV_SOK;
	
}
/*************************************************** 

WV_S32 EXT_3268_SetMux(WV_U32 chn ,WV_U32 ena);

***************************************************/
WV_S32 EXT_3268_SetMux(WV_U32 chn ,WV_U32 ena)
{

	unsigned char data;
	switch(chn)
	{
		case 1:
			data = 0;
			break;
		case 2:
			data = 1;
			break;
		case 0:
			data = 2;
			break;
		case 3:
			data = 3;
			break;
		default:
			return 0;
	}
	ext_mux_ena[data] = ena;

 if(chn == 3) {
 	gCodecDev[0].liCtl[chn]  = ena & 0x01;
 	}
 else{
    gCodecDev[0].liCtl[chn]  = (ena & 0x01) <<1;  
    } 
 
  EXT_3268_SetIn(& gCodecDev[0] );
  
 return WV_SOK;
}


/*************************************************** 

WV_S32 EXT_3268_SetLinEnc(WV_U32 chn ,WV_U32 ena);

***************************************************/
WV_S32 EXT_3268_SetLinEnc(WV_U32 chn ,WV_U32 ena)
{
	WV_S32 i;
	//clr input set
	for(i=0;i<4;i++)
		{
		   gCodecDev[1].liCtl[i]  =  0;  
		}
     gCodecDev[1].liCtl[chn]  = (ena & 0x01)<<1;  
	  if(chn == 3)
	  {
		gCodecDev[1].liCtl[i]  =  ena & 0x01;  
	  }
  
  EXT_3268_SetIn(& gCodecDev[1] );
  return WV_SOK;
}


/*************************************************** 

WV_S32  EXT_3268_SetMic(WV_U32 chn ,WV_U32 ena);

***************************************************/

WV_S32  EXT_3268_SetMic(WV_U32 chn ,WV_U32 ena)
{
 		 
 		 
	    gMicDev.micCtl[chn] = ena & 0x01;
	    HIS_GPIO_Set(gMicDev.sel[chn],gMicDev.micCtl[chn]);	
 
 	return WV_SOK;
}



/****************************************************************************

WV_S32 HIS_3268_CmdGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 HIS_3268_CmdGet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
 
   WV_U32 chn;
   WV_S32  i;
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"audio get <dev>  <mic> <iic> \r\n");
		return WV_SOK;
		}
		
	 if(strcmp(argv[0],"dev") ==0)	
	 {
	    if(argc <2)
	    {
	      prfBuff += sprintf(prfBuff,"audio get <dev>  <num> \r\n");
		return WV_SOK;   
	    }
      WV_STR_S2v(argv[1],&chn); 
      prfBuff += sprintf(prfBuff,"audio chn :%d\r\n",chn);
      prfBuff += sprintf(prfBuff,"iicHndl :%d \r\n",gCodecDev[chn].iicHndl.fd);
      prfBuff += sprintf(prfBuff,"sel IO : %d .%d \r\n",gCodecDev[chn].sel.bank,gCodecDev[chn].sel.num);
	  prfBuff += sprintf(prfBuff,"iicAdr : %d\r\n",gCodecDev[chn].iicAdr); 
	  prfBuff += sprintf(prfBuff,"input  : %d %d %d %d\r\n",gCodecDev[chn].liCtl[0],gCodecDev[chn].liCtl[1],gCodecDev[chn].liCtl[2],gCodecDev[chn].liCtl[3]); 
	  prfBuff += sprintf(prfBuff,"hpCtl : %d\r\n",gCodecDev[chn].hpCtl); 
	  prfBuff += sprintf(prfBuff,"loCtl : %d\r\n",gCodecDev[chn].loCtl); 
	    return  WV_SOK;
	  }
	  
    if(strcmp(argv[0],"mic") ==0)	
	 {      
        for(i=0;i<4;i++)
        {
        	prfBuff += sprintf(prfBuff,"mic IO %d : %d .%d \r\n",i,gMicDev.sel[i].bank,gMicDev.sel[i].num);
        	prfBuff += sprintf(prfBuff,"micCtl %d : %d \r\n",i,gMicDev.micCtl[i] ); 
	    } 
	    return  WV_SOK;
	  }	  
	  
	  
	  if(strcmp(argv[0],"iic") ==0)	
	 {  
	   if(argc <5)
	    {
	      prfBuff += sprintf(prfBuff,"audio get iic <dev> <book> <page> <reg> \r\n");
		return WV_SOK;   
	    }
	    
	     WV_U32 dev,book,page,reg;
	     WV_U8   iicData;
	     WV_STR_S2v(argv[1],&dev);
	     WV_STR_S2v(argv[2],&book);     
	     WV_STR_S2v(argv[3],&page);     
	     WV_STR_S2v(argv[4],&reg); 
	     
	     EXT_3268_Sel(&gCodecDev[dev] );
             EXT_3268_Write(&gCodecDev[dev],0x00,0x00);//Select Page 0 
	     EXT_3268_Write(&gCodecDev[dev],0x7f ,book);//Select Book 0
         EXT_3268_Write(&gCodecDev[dev],0x00,page);//Select Page  1   
	      EXT_3268_Read(&gCodecDev[dev],reg,&iicData);
	     EXT_3268_DeSel(&gCodecDev[dev] ); 
	     prfBuff += sprintf(prfBuff,"DEV[%d] BOOK[%d] PAGE[%d] %#x = %#x[%d]\r\n",dev,book,page,reg,iicData,iicData);  
	 
	         
        
	    return  WV_SOK;
	  }	  
	  
	  
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n");  
	 
	return WV_SOK;
 	
} 


/****************************************************************************

WV_S32 HIS_3268_CmdSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 HIS_3268_CmdSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
 
   
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"audio set  <mux> /<mic> /line  \r\n");
		return WV_SOK;
		}
   if(strcmp(argv[0],"mux") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"audio set  mux  <chn> <ena>\r\n");  
			return WV_SOK; 
		}
		WV_U32   chn,ena;
		WV_STR_S2v(argv[1],&chn);
	    WV_STR_S2v(argv[2],&ena); 
		EXT_3268_SetMux(chn,ena ); 
		return WV_SOK;  
	} 

     if(strcmp(argv[0],"mic") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"audio set  mic  <chn> <ena>\r\n");  
			return WV_SOK; 
		}
		WV_U32   chn,ena;
		WV_STR_S2v(argv[1],&chn);
	    WV_STR_S2v(argv[2],&ena); 
		EXT_3268_SetMic(chn,ena );  
		return WV_SOK;  
	} 
    if(strcmp(argv[0],"line") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"audio set  line  <chn> <ena>\r\n");  
			return WV_SOK; 
		}
		WV_U32   chn,ena;
		WV_STR_S2v(argv[1],&chn);
	    WV_STR_S2v(argv[2],&ena); 
		EXT_3268_SetLinEnc(chn,ena );  
		return WV_SOK;  
	}  
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
	        
	return WV_SOK;
 	
} 




/***************************************************

WV_S32  EXT_3268_Init();

***************************************************/
WV_S32  EXT_3268_Init()
{
	EXT_3268_DEV_E * pDev;

   WV_CMD_Register("audio","get","get audio info",HIS_3268_CmdGet);
   WV_CMD_Register("audio","set","set audio ",HIS_3268_CmdSet); 

    //reset
     gAudoRest.bank = 7;
     gAudoRest.num =  3;
    HIS_GPIO_SetCfg(gAudoRest,0);
    HIS_GPIO_Set(gAudoRest,1);
     usleep(40000);
    HIS_GPIO_Set(gAudoRest,0);
    usleep(40000);
    HIS_GPIO_Set(gAudoRest,1);
     usleep(40000);
    HIS_GPIO_Set(gAudoRest,0);
         usleep(40000);
    HIS_GPIO_Set(gAudoRest,1);
     usleep(40000);
    //mix IIS3 TX
	gCodecDev[0].iicBus = 0;
	gCodecDev[0].iicAdr = 0x30; 
	gCodecDev[0].sel.bank  = 7;
	gCodecDev[0].sel.num   = 7; 
	gCodecDev[0].liCtl[0]  = 2;
	gCodecDev[0].liCtl[1]  = 2; 
	gCodecDev[0].liCtl[2]  = 2; 
	gCodecDev[0].liCtl[3]  = 1; 
	gCodecDev[0].hpCtl  = 1;
	gCodecDev[0].loCtl = 0;
	pDev  = & gCodecDev[0]; 
    EXT_3268_DevInit(pDev);
    EXT_3268_DevStart_line(pDev);
    //EXT_3268_SetIn(pDev);
    //EXT_3268_SetOut(pDev);
    
     usleep(40000);
    
  /*  
    // linin enc  IIS1 RX
	gCodecDev[1].iicBus = 0;
	gCodecDev[1].iicAdr = 0x32; 
	gCodecDev[1].sel.bank  = 7;
	gCodecDev[1].sel.num  = 7; 
	gCodecDev[1].liCtl[0] = 2;
	gCodecDev[1].liCtl[1] = 0; 
	gCodecDev[1].liCtl[2] = 0; 
	gCodecDev[1].liCtl[3] = 0; 
	gCodecDev[1].hpCtl  = 0;
	gCodecDev[1].loCtl = 0;
	pDev  = & gCodecDev[1]; 
    EXT_3268_DevInit(pDev);
    EXT_3268_DevStart(pDev);
    EXT_3268_SetIn(pDev);
    EXT_3268_SetOut(pDev);
     usleep(40000);
 
 // MIC enc dec   IIS2 RX
	gCodecDev[2].iicBus = 0;
	gCodecDev[2].iicAdr = 0x30; 
	gCodecDev[2].sel.bank  = 7;
	gCodecDev[2].sel.num  = 6; 
	gCodecDev[2].liCtl[0] = 2;
	gCodecDev[2].liCtl[1] = 2; 
	gCodecDev[2].liCtl[2] = 2; 
	gCodecDev[2].liCtl[3] = 1; 
	gCodecDev[2].hpCtl  = 0;
	gCodecDev[2].loCtl = 1;
	pDev  = & gCodecDev[2]; 
     EXT_3268_DevInit(pDev);
     EXT_3268_DevStart(pDev);
     EXT_3268_SetIn(pDev);
     EXT_3268_SetOut(pDev);
    
    // mic contrl
     
	gMicDev.sel[0].bank = 9;
	gMicDev.sel[0].num  = 2; 
	gMicDev.sel[1].bank = 9;
	gMicDev.sel[1].num  = 3;
	gMicDev.sel[2].bank = 9;
	gMicDev.sel[2].num  = 4; 
	gMicDev.sel[3].bank = 9;
	gMicDev.sel[3].num  = 5; 
	WV_S32 i;
	for(i = 0; i< 4;i++)
	{
		HIS_GPIO_SetCfg(gMicDev.sel[i],0); 
		gMicDev.micCtl[i] = 1; 
		HIS_GPIO_Set(gMicDev.sel[i],gMicDev.micCtl[i]);	
		
	}
    
  */ 
	int i;
	for(i=0;i<4;i++)
	{
		ext_mux_ena[i] = 1;
	}
  
 return WV_SOK;
}
 
 
/***************************************************

WV_S32  EXT_3268_DeInit();

***************************************************/
WV_S32  EXT_3268_DeInit()
{
/*
	WV_S32 i;

	for (i = 0;i<3;i++)
	{
	  EXT_3268_DevDeInit(&gCodecDev[i] );
	}
*/
EXT_3268_DevDeInit(&gCodecDev[0] );
return WV_SOK;  
}



