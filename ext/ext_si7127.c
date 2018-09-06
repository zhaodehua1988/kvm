#include"ext_si7127.h"
#include"his_iic.h"
#include"his_gpio.h"
 
typedef struct EXT_SI7127_DEV_E 
{
   HIS_IIC_HNDL      iicHndl;
   HIS_GPIO_DEV_E    reset;
   WV_U8             iicBus;
   WV_U8             mAddr;
   WV_U8             sAddr; 
   
}EXT_SI7127_DEV_E;

static  EXT_SI7127_DEV_E   gSii7127Dev;

/**********************************************************************************

WV_S32 EXT_7127_IicRead(EXT_SI7127_DEV_E * pDev ,WV_U8 sel,WV_U8 reg,WV_U8 *pData);

**********************************************************************************/
WV_S32 EXT_7127_IicRead(EXT_SI7127_DEV_E * pDev ,WV_U8 sel,WV_U8 reg,WV_U8 *pData)
{ 
   if(sel == 1){
	   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev-> mAddr, reg, pData, 1) );
	   }
	else{
	   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev->sAddr, reg, pData, 1) );
		}   
  return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_7127_Write(EXT_SI7127_DEV_E * pDev ,WV_U8 data);

**********************************************************************************/
WV_S32 EXT_7127_IicWrite(EXT_SI7127_DEV_E * pDev,WV_U8 sel ,WV_U8 reg,WV_U8 data)
{ 
   if(sel == 1){
        WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev-> mAddr, reg,&data, 1) );
        }
   else{
   	     WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev-> sAddr, reg,&data, 1) );	
   		}     
  return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_7127_SetPll(EXT_SI7127_DEV_E * pDev ,WV_U32 clkFreq);

**********************************************************************************/

WV_U8  EXT_7127_PLL_reg[]={
0x35,0x36,0x38,0x3A,0x27,           //  Color Depth | IDCK Range(MHz) | Pix/Clk //
//                                  //------------------------------------------//
0x06,0x30,0x32,0x00,0x0D,           //       8      |    25 - 65      |    1    //
0x03,0x31,0x11,0x00,0x0D,           //       8      |    65 - 112     |    1    //
0x03,0x33,0x00,0x10,0x0D,           //       8      |   112 -         |    1    //
0x03,0x30,0x10,0x10,0x2D,           //       8      |    25 - 65      |    2    //
0x03,0x31,0x00,0x10,0x2D,           //       8      |    65 -         |    2    //
//                                  //------------------------------------------//
0x03,0x40,0x30,0x10,0x0B,           //      10      |    25 - 53      |    1    //
0x03,0x41,0x10,0x10,0x0B,           //      10      |    40 - 104     |    1    //
0x03,0x43,0x00,0x10,0x0B,           //      10      |    86 - 203     |    1    //
0x03,0x40,0x10,0x10,0x2B,           //      10      |    25 - 53      |    2    //
0x03,0x41,0x00,0x10,0x2B,           //      10      |    40 - 94      |    2    //
//                                  //------------------------------------------//
0x03,0x50,0x30,0x10,0x0E,           //      12      |    25 - 44      |    1    //
0x03,0x51,0x10,0x10,0x0E,           //      12      |    36 - 68      |    1    //
0x03,0x53,0x00,0x10,0x0E,           //      12      |    71 - 168     |    1    //
0x03,0x50,0x10,0x10,0x2E,           //      12      |    25 - 44      |    2    //
0x03,0x51,0x00,0x10,0x2E,           //      12      |    36 - 78      |    2    //
//                                  //------------------------------------------//
0x03,0x30,0x10,0x10,0x0A,           //      16      |    25 - 64      |    1    //
0x03,0x31,0x00,0x10,0x0A,           //      16      |    53 - 114     |    1    //
0x03,0x30,0x00,0x10,0x2A            //      16      |    25 - 54      |    2    //
};


WV_S32 EXT_7127_SetPll(EXT_SI7127_DEV_E * pDev ,WV_U8  sel,WV_U32 clkFreq)
{ 

  WV_U32   index; 
  WV_S32  i;
   if (clkFreq >=25 && clkFreq<=65)
      index = 1;
   else if (clkFreq>65&&clkFreq<=112)
      index = 2;
   else if (clkFreq>112)
      index = 3; 
   else
      index = 2;
         
   for (i=0;i<5;i++)
  {
    WV_CHECK_RET( EXT_7127_IicWrite(pDev,sel,EXT_7127_PLL_reg[i],EXT_7127_PLL_reg[index*5+i]) ); 
  }   
  return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_7127_SetReg(EXT_SI7127_DEV_E * pDev ,WV_U32 clkFreq);

**********************************************************************************/
WV_S32 EXT_7127_SetReg(EXT_SI7127_DEV_E * pDev ,WV_U32 disFreq)

{
      WV_U8  temp;
      WV_S32 i; 
      WV_U32 clkFreq ; 
       
      clkFreq  =  (disFreq  & 0Xffe00000) >>21;
      WV_printf(" Si72127 set disClk[%d]  Clk[%d]\r\n",disFreq,clkFreq); 
       HIS_GPIO_Set(pDev-> reset,1);
	   usleep(500000);
	   HIS_GPIO_Set(pDev-> reset,0); 
      
      EXT_7127_IicRead (pDev,0, 0x33, &temp);
      EXT_7127_IicWrite(pDev,0, 0x33, 0x00);//Initialize the two devices: 
      EXT_7127_IicRead (pDev,0, 0x33, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x33, &temp);
      EXT_7127_IicWrite(pDev,1, 0x33, 0x00);
      EXT_7127_IicRead (pDev,1, 0x33, &temp);
      
      usleep(500000);
      EXT_7127_IicRead (pDev,0, 0x0C, &temp);
      EXT_7127_IicWrite(pDev,0, 0x0C, 0x03);//Setup the PLL
      EXT_7127_IicRead (pDev,0, 0x0C, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x0C, &temp);
      EXT_7127_IicWrite(pDev,1, 0x0C, 0x03);
      EXT_7127_IicRead (pDev,1, 0x0C, &temp);
      usleep(500000);
     //pll_cfg_7172( dis_sel, SIL_7172_Sec, Color_Depth,Pclk_Freq,Pix_per_Clk);//Set color depth and pixel rate
      //pll_cfg_7172( dis_sel, SIL_7172_Pri, Color_Depth,Pclk_Freq,Pix_per_Clk);
      EXT_7127_SetPll(pDev,0,clkFreq);// clkFreq ? 
      EXT_7127_SetPll(pDev,1,clkFreq);//
      
     usleep(500000);
//      EXT_7127_IicRead (pDev,0, 0x31, &temp);
//      EXT_7127_IicWrite(pDev,0, 0x31, 0x04);//Setup the clock
//      EXT_7127_IicRead (pDev,0, 0x31, &temp);
//      EXT_7127_IicRead (pDev,1, 0x31, &temp);
//      EXT_7127_IicWrite(pDev,1, 0x31, 0x04);
//      EXT_7127_IicRead (pDev,1, 0x31, &temp);
      usleep(500000);
      EXT_7127_IicRead (pDev,0, 0x28, &temp);
      EXT_7127_IicWrite(pDev,0, 0x28, 0x04);//Reset the FIFO
      EXT_7127_IicRead (pDev,0, 0x28, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x28, &temp);
      EXT_7127_IicWrite(pDev,1, 0x28, 0x04);
      EXT_7127_IicRead (pDev,1, 0x28, &temp);
      usleep(500000);
      EXT_7127_IicRead (pDev,0, 0x39, &temp);
      EXT_7127_IicWrite(pDev,0, 0x39, 0x30);//Setup Dual-link mode:slave
      EXT_7127_IicRead (pDev,0, 0x39, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x39, &temp);
      EXT_7127_IicWrite(pDev,1, 0x39, 0x20);//Setup Dual-link mode:master
      EXT_7127_IicRead (pDev,1, 0x39, &temp);
      usleep(500000);
      EXT_7127_IicRead (pDev,0, 0x08, &temp);
      EXT_7127_IicWrite(pDev,0, 0x08, 0x33);//Set clock edge and normal operation mode:
      EXT_7127_IicRead (pDev,0, 0x08, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x08, &temp);
      EXT_7127_IicWrite(pDev,1, 0x08, 0x33);
      EXT_7127_IicRead (pDev,1, 0x08, &temp);
      usleep(500000);
      EXT_7127_IicRead (pDev,0, 0x28, &temp);
      EXT_7127_IicWrite(pDev,0, 0x28, 0x00);//Clear FIFO Reset:
      EXT_7127_IicRead (pDev,0, 0x28, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x28, &temp);
      EXT_7127_IicWrite(pDev,1, 0x28, 0x00);  
      EXT_7127_IicRead (pDev,1, 0x28, &temp);
      usleep(500000);
      EXT_7127_IicRead (pDev,0, 0x39, &temp);
      EXT_7127_IicWrite(pDev,0, 0x39, 0x38);//Sync Bit Set for Register
      EXT_7127_IicRead (pDev,0, 0x39, &temp);
      
      EXT_7127_IicRead (pDev,1, 0x39, &temp);
      EXT_7127_IicWrite(pDev,1, 0x39, 0x28);//Sync Bit Set for Register
      EXT_7127_IicRead (pDev,1, 0x39, &temp);
      
      usleep(500000);
      EXT_7127_IicRead (pDev,1, 0x09, &temp);//Clear the MDI Interrupt for Device 0x70
      EXT_7127_IicWrite(pDev,1, 0x09, temp|0x01);
      
      for(i=0; i<0; i++)
      {
//        sil7172_ProgR(dis_sel, SIL_7172_Pri, i, &temp);
//        temp = temp + 1;
//      Delay(500);
     EXT_7127_IicRead (pDev,0, 0x39, &temp);
     EXT_7127_IicWrite(pDev,0, 0x39, 0x38);
     EXT_7127_IicRead (pDev,0, 0x39, &temp);
      
     EXT_7127_IicRead (pDev,1, 0x39, &temp);
     EXT_7127_IicWrite(pDev,1, 0x39, 0x28);
     EXT_7127_IicRead (pDev,1, 0x39, &temp);
      
//      Delay(500);
      }
      
      for(i=0; i<0x40; i++)
      {
        EXT_7127_IicRead (pDev,0, i, &temp);
        temp = temp + 1;
      }
 

return WV_SOK;
}


/**********************************************************************************

WV_S32  EXT_SI7127_Open(EXT_SI7127_DEV_E  * pDev);

**********************************************************************************/
WV_S32  EXT_SI7127_Open(EXT_SI7127_DEV_E  * pDev)
{
    HIS_IIC_Open(&pDev-> iicHndl,pDev->iicBus); 
    HIS_GPIO_SetCfg(pDev-> reset,0); 
    HIS_GPIO_Set(pDev-> reset,1);
    usleep(20000); 
    HIS_GPIO_Set(pDev-> reset,0);
    
    EXT_7127_SetReg(pDev ,297000000); 
    return  WV_SOK;
}



/**********************************************************************************

WV_S32  EXT_SI7127_Close(EXT_SI7127_DEV_E  * pDev);

**********************************************************************************/
WV_S32  EXT_SI7127_Close(EXT_SI7127_DEV_E  * pDev)
{
    HIS_IIC_Close(&pDev-> iicHndl);  
    return  WV_SOK;
}

/**********************************************************************************

WV_S32  EXT_SI7127_Set(WV_U32 disFreq);

**********************************************************************************/

WV_S32  EXT_SI7127_Set(WV_U32 disFreq)
{
   EXT_7127_SetReg(&gSii7127Dev,disFreq);
   return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_SI7127_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_SI7127_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
	 WV_U32 clkFrea;    
	if(argc<2) 
		{
		prfBuff += sprintf(prfBuff,"SI7127 set <clk>\r\n");
		return WV_SOK;
		}
    if(strcmp(argv[0],"clk") ==0)
	{
	   prfBuff += sprintf(prfBuff,"SI7127  set  clk:\r\n");  
	   WV_STR_S2v(argv[1],&clkFrea); 
	   EXT_SI7127_Set(clkFrea);
	   return WV_SOK;
	}
	
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
  return WV_SOK;
}





/**********************************************************************************

WV_S32  EXT_SI7127_Init();

**********************************************************************************/
WV_S32  EXT_SI7127_Init()
{
    
   WV_CMD_Register("7127","set","set Sii7127 conf",EXT_SI7127_CMDSet);
 
    gSii7127Dev.iicBus = 0;
    gSii7127Dev.reset.bank = 8;
    gSii7127Dev.reset.num  = 2;
    gSii7127Dev.mAddr   =  0x70;
    gSii7127Dev.sAddr   =  0x72;
     EXT_SI7127_Open(&gSii7127Dev); 
     
     WV_printf(" EXT_SI7127_Init ok !\r\n");
              return  WV_SOK;
}


/**********************************************************************************

WV_S32  EXT_SI7127_DeInit();

**********************************************************************************/
WV_S32  EXT_SI7127_DeInit()
{
   
     EXT_SI7127_Close(&gSii7127Dev);
     WV_printf(" EXT_SI7127_DeInit ok  !\r\n"); 
    return  WV_SOK;
}







