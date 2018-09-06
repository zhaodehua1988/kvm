#include"ext_dvi.h"
#include"his_iic.h"
#include"his_gpio.h"

typedef struct EXT_DVI_DEV_E 
{
// 
  HIS_IIC_HNDL      iicHndl;
  HIS_GPIO_DEV_E    inSel[4];
  HIS_GPIO_DEV_E    edidSel[4];
  HIS_GPIO_DEV_E    outSel[4];  
  HIS_GPIO_DEV_E    drvSel;
  HIS_GPIO_DEV_E    swRset; 
  HIS_GPIO_DEV_E    islRset;
  // statute 
  WV_U8       		iicBus;
  WV_U8       		islAddr[8];   
  WV_U8       		swAddr; 
  //
  WV_U8             sw[5];  
  //
  WV_U8             edid[256];  
}EXT_DVI_DEV_E ;

static int gDviSwflag;//flag=1,sw is working,flag = 0 ,sw is free

WV_U8  EXT_DIV_ISL_CfgData[]={
  /*
  0x03,0x12, //default 0x12,recommended 0x63,detect mode
  0x04,0x00, //default 0x00,termination
  0x05,0x00, //default 0x00,output ctrl
  0x06,0x00, //default 0x00,transmit current and pre-emphasis
  0x07,0xcc, //default 0xcc,
  0x08,0xcc, //default 0xcc,equalization,value 0xa is from datasheet on page 9;
  0x09,0x00, //default 0x00,test pattern ctrl
  0x10,0x10	 //default 0x10,recommended 0x12
  */
/*
  0x03,0x63, //default 0x12,recommended 0x63,detect mode
  0x04,0x40, //default 0x00,termination  clk 100
  0x05,0x00, //default 0x00,output ctrl
  0x06,0xaa, //default 0x00,transmit current and pre-emphasis
  0x07,0xcc, //default 0xcc,
  0x08,0xc0, //default 0xcc,equalization,value 0xa is from datasheet on page 9;
  0x09,0x00, //default 0x00,test pattern ctrl
  0x10,0x12  //default 0x10,recommended 0x12
*/
/*************************************************/
/*
  0x03,0x63, //default 0x12,recommended 0x63,detect mode
  0x04,0x00, //default 0x00,termination  clk 100
  0x05,0x00, //default 0x00,output ctrl
  0x06,0x0f, //default 0x00,transmit current and pre-emphasis
  0x07,0xcc, //default 0xcc,
  0x08,0xcf, //default 0xcc,equalization,value 0xa is from datasheet on page 9;
  0x09,0x00, //default 0x00,test pattern ctrl
  0x10,0x10  //default 0x10,recommended 0x12
*/

  0x03,0x63, //default 0x12,recommended 0x63,detect mode
  0x04,0x40, //default 0x00,termination  clk 100
  0x05,0x00, //default 0x00,output ctrl
  0x06,0x0f, //default 0x00,transmit current and pre-emphasis
  0x07,0xcc, //default 0xcc,
  0x08,0xcc, //default 0xcc,equalization,value 0xa is from datasheet on page 9;
  0x09,0x00, //default 0x00,test pattern ctrl
  0x10,0x10  //default 0x10,recommended 0x12

};


WV_U8  EXT_DIV_SW_inMap[]={
 45,40,32,47,41,43,44,
 11, 9,18, 2, 8,10, 0,
 29,30,36,28,31,38,46,
 24,42,27,16,34,25,26,
 13,14,20, 4, 6,12,15,
 22,22,22,22,22,22,22
  
};

WV_U8  EXT_DIV_SW_outMap[]={
29,30,31,13,14,15,28,
 0,25,16,11,10, 9,24,
12, 4, 6,22,36,38,20,
 2, 8,46,47,45,44,26,
42,32,41,40,18,27,43  
  
};
#if 0
WV_U8   Default_Dual_EDID[128] = 
{ 
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x52,0x49,0x00,0x79,0x63,0x1C,0x00,0x00,
    0x2A,0x10,0x01,0x03,0x80,0x34,0x21,0x78,0xEA,0x5A,0xD5,0xA7,0x56,0x4B,0x9B,0x24,
    0x13,0x50,0x54,0xBD,0xEF,0x80,0x71,0x4F,0x81,0xC0,0x81,0x80,0x8B,0xC0,0x90,0x40,
    0x95,0x00,0xA9,0x40,0x8C,0xC0,0x30,0x75,0x00,0xA0,0xF0,0x00,0x46,0x40,0x30,0x20,
    0x6C,0x00,0x07,0x44,0x21,0x00,0x00,0x1E,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,
    0x58,0x2C,0x45,0x00,0xDD,0x0C,0x11,0x00,0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x0A,
    0x4C,0x0A,0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
    0x00,0x54,0x52,0x49,0x20,0x32,0x34,0x30,0x57,0x0A,0x0A,0x0A,0x0A,0x0A,0x00,0x7D
  }; //extention and check_sum
#endif

#if 1
//2560
WV_U8   Default_Dual_EDID[128] = 
{ 
	0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0x00 ,0x10 ,0xAC ,0x35 ,0x40 ,0x4C ,0x37 ,0x38 ,0x30,
	0x0E ,0x14 ,0x01 ,0x03 ,0x80 ,0x41 ,0x29 ,0x78 ,0xEA ,0x8D ,0x85 ,0xAD ,0x4F ,0x35 ,0xB1 ,0x25,
	0x0E ,0x50 ,0x54 ,0xA5 ,0x4B ,0x00 ,0x81 ,0x80 ,0xA9 ,0x40 ,0xD1 ,0x00 ,0xD1 ,0x40 ,0x71 ,0x4F,
	0x81 ,0x00 ,0xB3 ,0x00 ,0x01 ,0x01 ,0xB0 ,0x68 ,0x00 ,0xA0 ,0xA0 ,0x40 ,0x2E ,0x60 ,0x30 ,0x20,
	0x36 ,0x00 ,0x81 ,0x90 ,0x21 ,0x00 ,0x00 ,0x1E ,0x00 ,0x00 ,0x00 ,0xFF ,0x00 ,0x47 ,0x35 ,0x30,
	0x31 ,0x48 ,0x30 ,0x34 ,0x41 ,0x30 ,0x38 ,0x37 ,0x4C ,0x0A ,0x28 ,0x3C ,0x80 ,0xA0 ,0x70 ,0xB0,
	0x23 ,0x40 ,0x30 ,0x20 ,0x36 ,0x00 ,0x81 ,0x91 ,0x21 ,0x00 ,0x00 ,0x1C ,0x00 ,0x00 ,0x00 ,0xFD,
	0x00 ,0x31 ,0x56 ,0x1D ,0x71 ,0x1C ,0x00 ,0x0A ,0x20 ,0x20 ,0x20 ,0x20 ,0x20 ,0x20 ,0x00 ,0x6D
}; 
#endif
/*
WV_U8   Default_Dual_EDID[128] = 
{ 
	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x05,0xE3,0x79,0x28,0xF8,0x03,0x00,0x00,
	0x20,0x1B,0x01,0x03,0x80,0x3E,0x22,0x78,0x2A,0x08,0xA5,0xA2,0x57,0x4F,0xA2,0x28,
	0x0F,0x50,0x54,0xBF,0xEF,0x00,0xD1,0xC0,0xB3,0x00,0x95,0x00,0x81,0x80,0x81,0x40,
	0x81,0xC0,0x01,0x01,0x01,0x01,0xA3,0x66,0x00,0xA0,0xF0,0x70,0x1F,0x80,0x30,0x20,
	0x35,0x00,0x6D,0x55,0x21,0x00,0x00,0x1A,0x56,0x5E,0x00,0xA0,0xA0,0xA0,0x29,0x50,
	0x30,0x20,0x35,0x00,0x6D,0x55,0x21,0x00,0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x17,
	0x50,0x1E,0x63,0x1E,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
	0x00,0x55,0x32,0x38,0x37,0x39,0x47,0x36,0x0A,0x20,0x20,0x20,0x20,0x20,0x01,0xE4
  }; //extention and check_sum
*/
#if 0
//3840*2160
WV_U8   Default_Dual_EDID[128] = 
{ 
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x24, 0x4D, 0x1F, 0x25, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x0C, 0x01, 0x03, 0x80, 0x30, 0x1E, 0x78, 0xE8, 0x64, 0x92, 0xA3, 0x57, 0x4A, 0x9C, 0x25,
	0x15, 0x50, 0x54, 0x2F, 0xCF, 0x00, 0x31, 0x59, 0x45, 0x59, 0x61, 0x59, 0x81, 0x80, 0xA9, 0x40,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x82, 0x6A, 0x00, 0x18, 0xF1, 0x70, 0x2E, 0x80, 0x58, 0x2C,
	0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0xDA, 0x2F, 0x80, 0xA0, 0x70, 0xB0, 0x1C, 0x40,
	0x30, 0x20, 0x26, 0x00, 0xE0, 0x2C, 0x11, 0x00, 0x00, 0x1A, 0x51, 0x31, 0x80, 0xA0, 0x70, 0x60,
	0x1C, 0x90, 0x30, 0x20, 0x2A, 0x00, 0xF0, 0x2C, 0x01, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x09, 0x5F, 0x16, 0x69, 0x11, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x94
 }; 
#endif
static  EXT_DVI_DEV_E  gDviDev;



/*****************************************************

WV_S32  EXT_DVI_IicInSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicInSel(WV_S32  chn,EXT_DVI_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> inSel[chn],1)  );
	usleep(100000);
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_DVI_IicInDeSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicInDeSel(WV_S32  chn,EXT_DVI_DEV_E *pDev)
{
	usleep(100000);
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> inSel[chn],0)  );
	usleep(100000);
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_DVI_IicOutSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicOutSel(WV_S32  chn,EXT_DVI_DEV_E *pDev)
{  
    ;
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> outSel[chn],1)  );
	usleep(100000);
	return WV_SOK;
}


/*****************************************************

WV_S32  EXT_DVI_IicOutDeSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicOutDeSel(WV_S32  chn,EXT_DVI_DEV_E *pDev)
{
	usleep(100000);
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> outSel[chn],0)  );
	usleep(100000);
	return WV_SOK;
}


/*****************************************************

WV_S32  EXT_DVI_IicEdidSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicEdidSel(WV_S32  chn,EXT_DVI_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> edidSel[chn],1)  );
	usleep(100000);
	return WV_SOK;
}


/*****************************************************

WV_S32  EXT_DVI_IicEdidDeSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicEdidDeSel(WV_S32  chn,EXT_DVI_DEV_E *pDev)
{
	usleep(100000);
	WV_CHECK_RET( HIS_GPIO_Set(pDev->edidSel[chn],0)  );
	usleep(100000);
	return WV_SOK;
}


/*****************************************************

WV_S32  EXT_DVI_IicDrvSel(WV_S32  chn,EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicDrvSel(EXT_DVI_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> drvSel,1)  );
	usleep(100000);
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_DVI_IicDrvDeSel(EXT_DVI_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_DVI_IicDrvDeSel(EXT_DVI_DEV_E *pDev)
{
	usleep(100000);
	WV_CHECK_RET( HIS_GPIO_Set(pDev->drvSel,0)  );
	usleep(100000);
	return WV_SOK;
}

/**********************************************************************************

WV_S32 EXT_DVI_Read(EXT_VGA_DEV_E * pDev ,WV_U8 *pData);

**********************************************************************************/
WV_S32 EXT_DVI_IicReadIsl(EXT_DVI_DEV_E * pDev ,WV_U8 chn,WV_U8 reg,WV_U8 *pData)
{ 
 

   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev-> islAddr[chn], reg, pData, 1) );
  // WV_printf("isl reda dev[%#x] reg[%#x] = %#x[%d]\r\n",pDev-> islAddr[chn],reg,*pData,*pData); 
  return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_DVI_Write(EXT_VGA_DEV_E * pDev ,WV_U8 data);

**********************************************************************************/
WV_S32 EXT_DVI_IicWriteIsl(EXT_DVI_DEV_E * pDev,WV_U8 chn ,WV_U8 reg,WV_U8 data)
{ 
 
   WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev-> islAddr[chn], reg,&data, 1) );
  //   WV_printf("isl write dev[%#x] reg[%#x] = %#x[%d]\r\n",pDev-> islAddr[chn],reg,data,data); 
  return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_DVI_EdidWrite(EXT_VGA_DEV_E * pDev ,WV_U8 chn);

**********************************************************************************/
WV_S32  EXT_DVI_EdidWrite(EXT_DVI_DEV_E * pDev ,WV_U8 chn)
{ 
  WV_S32  i;
  EXT_DVI_IicInDeSel( chn,pDev); //
  EXT_DVI_IicEdidSel(chn,pDev);
  usleep(100000); 
  for(i=0;i<128;i++){
   		//WV_CHECK_RET(  HIS_IIC_Write8 (pDev-> iicHndl,0xa0, i,&Default_Dual_EDID[i], 1) );
        //HIS_IIC_Write8 (pDev-> iicHndl,0xa0, i,&Default_Dual_EDID[i], 1);
   } 
   EXT_DVI_IicEdidDeSel(chn,pDev);
   EXT_DVI_IicInSel( chn,pDev);  
  return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_DVI_Read(EXT_VGA_DEV_E * pDev ,WV_U8 *pData);

**********************************************************************************/
WV_S32 EXT_DVI_IicReadSw(EXT_DVI_DEV_E * pDev ,WV_U8 reg,WV_U8 *pData)
{ 
 
   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev-> swAddr, reg, pData, 1) );
  //  WV_printf("dvisw read  %d = %#x[%d]\r\n",reg,*pData,*pData); 
  return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_DVI_Write(EXT_VGA_DEV_E * pDev ,WV_U8 data);

**********************************************************************************/
WV_S32 EXT_DVI_IicWriteSw(EXT_DVI_DEV_E * pDev,WV_U8 reg,WV_U8 data)
{ 
 
   WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev-> swAddr, reg,&data, 1) );
  // WV_printf("dvisw write  %d = %#x[%d]\r\n",reg,data,data);   
 return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_DVI_DrvInit(EXT_DVI_DEV_E * pDev );

**********************************************************************************/
/*
WV_S32 EXT_DVI_DrvInit(EXT_DVI_DEV_E * pDev ) 
{ 
       WV_S32  i,j; 
       
      EXT_DVI_IicDrvSel(pDev);
      usleep(10000); 
       for(i=0;i<4;i++)
       {
           
          for(j=0;j<sizeof(EXT_DIV_ISL_CfgData)/2;j++)
          {
           
			EXT_DVI_IicWriteIsl(pDev,i*2,EXT_DIV_ISL_CfgData[j*2],EXT_DIV_ISL_CfgData[j*2+1]);  
          } 
          
         for(j=0;j<sizeof(EXT_DIV_ISL_CfgData)/2;j++)
          {
           	EXT_DVI_IicWriteIsl(pDev,i*2+1,EXT_DIV_ISL_CfgData[j*2],EXT_DIV_ISL_CfgData[j*2+1]);
          } 
       	 	//EXT_DVI_IicWriteIsl(pDev,i*2+1,0x03,0x67); // clock in tri-stated
        	//EXT_DVI_IicWriteIsl(pDev,i*2+1,0x05,0x01); // clock out tri-stated  
          usleep(10000);   
       }
     // usleep(10000); 
      EXT_DVI_IicDrvDeSel(pDev);
      usleep(10000); 
  return WV_SOK;	
}
*/
/**********************************************************************************

WV_S32 EXT_DVI_DrvInit(EXT_DVI_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_DVI_DrvInit(EXT_DVI_DEV_E * pDev ) 
{ 
       WV_S32  i,j; 
       printf("********************* dvi drv init *******************\n");
      EXT_DVI_IicDrvSel(pDev);
      usleep(10000); 
       for(i=0;i<4;i++)
       {
           
          for(j=0;j<sizeof(EXT_DIV_ISL_CfgData)/2;j++)
          {
           
			EXT_DVI_IicWriteIsl(pDev,i*4,EXT_DIV_ISL_CfgData[j*2],EXT_DIV_ISL_CfgData[j*2+1]);  
          } 
          
         for(j=0;j<sizeof(EXT_DIV_ISL_CfgData)/2;j++)
          {
           	EXT_DVI_IicWriteIsl(pDev,i*4+2,EXT_DIV_ISL_CfgData[j*2],EXT_DIV_ISL_CfgData[j*2+1]);
          } 
        	//EXT_DVI_IicWriteIsl(pDev,i*2+1,0x05,0x01); // clock out tri-stated  
			//EXT_DVI_IicWriteIsl(pDev,i*2+1,0x03,0x67); // clock in tri-stated
			//EXT_DVI_IicWriteIsl(pDev,i*2+1,0x04,0x44); // clock in tri-stated


          usleep(10000);   
       }
     // usleep(10000); 
      EXT_DVI_IicDrvDeSel(pDev);
      usleep(10000); 
  return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_DVI_SwInit(EXT_DVI_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_DVI_SwInit(EXT_DVI_DEV_E * pDev ) 
{ 
  	
  	EXT_DVI_IicWriteSw(pDev,0xff, 0x00);    //page00
    EXT_DVI_IicWriteSw(pDev,0xff, 0x00);    //page00
     
  	EXT_DVI_IicWriteSw(pDev,0x00, 0xaa);
  	usleep(1000000);
  	EXT_DVI_IicWriteSw(pDev,0x00, 0x00);
  	EXT_DVI_IicWriteSw(pDev,0x03, 0xa8);    //lane mode  ;   strobe ASC mode  a8  28
  	EXT_DVI_IicWriteSw(pDev,0x05, 0x86);    //c4   c2   82  84
  	EXT_DVI_IicWriteSw(pDev,0x06, 0x80);  
  	EXT_DVI_IicWriteSw(pDev,0x08, 0xc0);    //50 60 vpp
  	EXT_DVI_IicWriteSw(pDev,0x0a, 0x33);    //input eq=8
  	//EXT_DVI_IicWriteSw(pDev,0x0b, 0xf7);   //DE  12db              //84   85  f4  f5   
  	
  return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_DVI_SwInit(EXT_DVI_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_DVI_SwDeInit(EXT_DVI_DEV_E * pDev ) 
{ 
  	
  	 
  return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_DVI_SwSetChn(EXT_VGA_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_DVI_SwSetChn(EXT_DVI_DEV_E * pDev ,WV_U8  in  ,WV_U8 out ) 
{ 
    WV_S32  i; 
    
    pDev -> sw[out] = in;
  	EXT_DVI_IicWriteSw(pDev,0xff, 0x03);    //page03
  	for(i=0;i<7;i++)
  	{
  		EXT_DVI_IicWriteSw(pDev,EXT_DIV_SW_outMap[out*7+i],EXT_DIV_SW_inMap[in*7+i]);    // 
  	}
	EXT_DVI_IicWriteSw(pDev, 0xff, 0x00);    //page00
	EXT_DVI_IicWriteSw(pDev, 0x04, 0x55);     //switch active 
    usleep(1000000);
	EXT_DVI_IicWriteSw(pDev, 0x04, 0);     //switch end
   return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_DVI_DevOpen(EXT_DVI_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_DVI_DevOpen(EXT_DVI_DEV_E * pDev ) 
{ 
   WV_S32 i;
   
      for(i=0;i<4;i++){
        HIS_GPIO_SetCfg(pDev-> inSel[i],0);
		HIS_GPIO_Set(pDev-> inSel[i],1);


	    HIS_GPIO_SetCfg(pDev-> outSel[i],0);
		HIS_GPIO_Set(pDev-> outSel[i],0);

		HIS_GPIO_SetCfg(pDev-> edidSel[i],0);
		HIS_GPIO_Set(pDev-> edidSel[i],0); 
      }
 
      HIS_GPIO_SetCfg(pDev-> drvSel,0);
	  HIS_GPIO_Set(pDev-> drvSel,0);
	  
      HIS_GPIO_SetCfg(pDev->swRset,0);
	  HIS_GPIO_Set(pDev->swRset,0);  // rest swIC  23636
	   usleep(100000);
	  HIS_GPIO_Set(pDev-> swRset,1);  
	  
	  HIS_GPIO_SetCfg(pDev-> islRset,0);
	  HIS_GPIO_Set(pDev-> islRset,0); //reset  isl  
	  usleep(100000);
	  HIS_GPIO_Set(pDev-> islRset,1);  
	   usleep(100000); 
	  HIS_GPIO_Set(pDev-> islRset,0);
	 
	  //open iic dev
	   HIS_IIC_Open(&pDev-> iicHndl,pDev->iicBus); 
	     WV_printf("DVI open \r\n");
      for(i=0;i<4;i++){
          EXT_DVI_EdidWrite(pDev ,i); 
      }


       
      EXT_DVI_DrvInit(pDev); 
	   WV_printf("DVI isl init ok\r\n");
       usleep(10000);
       EXT_DVI_SwInit(pDev);
      WV_printf("DVI sw init ok \r\n");
      
	gDviSwflag = 0; 
   return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_DVI_DevClose(EXT_DVI_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_DVI_DevClose(EXT_DVI_DEV_E * pDev  ) 
{  
   WV_CHECK_RET( HIS_IIC_Close  ( &pDev-> iicHndl) ); 
   return WV_SOK;	
}

/****************************************************************************

WV_S32 EXT_DVI_GetSw(WV_U8 *pSw)

****************************************************************************/
WV_S32 EXT_DVI_GetSw(WV_U8 *pSw)
{
	WV_S32 i;
	for(i=0;i<5;i++)
	{
		pSw[i] = gDviDev.sw[i];
	}
	return 0;
}
/*********************************************************************************

WV_S32  EXT_DVI_SetSw(WV_U8  outChn, WV_U8  inChn, WV_U8 ena )

*********************************************************************************/
WV_S32  EXT_DVI_SetSw(WV_U8  outChn, WV_U8  inChn, WV_U8 ena )
{
	
	//printf("dvi set sw out[%d] in[%d] ena[%d]\n",outChn,inChn,ena);
	if(gDviSwflag == 0)
	{
		//printf("111 dvi set sw out[%d] in[%d] ena[%d]\n",outChn,inChn,ena);
		gDviSwflag = 1;
		WV_S8 sw[5];
		WV_S8 key=0;
		
		EXT_DVI_GetSw(sw);
		EXT_FPGA_SendKB(sw[outChn],&key,1);//pop all key

		if(ena == 1)
		{
			EXT_DVI_SwSetChn(&gDviDev ,inChn  ,outChn);
		}
		else
		{
			EXT_DVI_SwSetChn(&gDviDev ,5  ,outChn); 
		}
		
		gDviSwflag = 0;
		
	}else{
		WV_printf(" dvi set sw is working,please Wait a few seconds...\n ");
		return WV_EFAIL;
	}
    return WV_SOK; 
}

/****************************************************************************

WV_S32 EXT_DVI_CMDGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_DVI_CMDGet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
	   WV_S32 i;  
	if(argc<1) 
	{
		prfBuff += sprintf(prfBuff,"dvi get <sw>\r\n");
		return WV_SOK;
	}
    if(strcmp(argv[0],"sw") ==0)
	{
	   prfBuff += sprintf(prfBuff,"dvi get sw :\r\n");
	    for(i=0;i<5;i++)
	    { 
	       prfBuff += sprintf(prfBuff,"%d  : %d\r\n",i,gDviDev.sw[i]);  
	    }
	   
	   return WV_SOK;
	}
  return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_DVI_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_DVI_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
	   
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"dvi set <sw>/<isl>\r\n");
		return WV_SOK;
		}
    if(strcmp(argv[0],"sw") ==0)
	{
	   if(argc < 4)
	   {
	     prfBuff += sprintf(prfBuff,"dvi set sw  <outchn>  <inchn>  <ena>:\r\n");
	     return WV_SOK;
	    } 
	    WV_U32   inChn,outChn,ena;  
	    WV_STR_S2v(argv[1],&outChn);
	    WV_STR_S2v(argv[2],&inChn);	 
		WV_STR_S2v(argv[3],&ena); 
	    EXT_DVI_SetSw(outChn,inChn, ena );
	   return WV_SOK;
	}

    if(strcmp(argv[0],"isl") ==0)
	{
	   if(argc < 4)
	   {
	     prfBuff += sprintf(prfBuff,"dvi set isl  <chn>  <reg>  <data>:\r\n");
	     return WV_SOK;
	    } 
	    WV_U32   reg,data,chn;  
	    WV_STR_S2v(argv[1],&chn);
	    WV_STR_S2v(argv[2],&reg);	 
		WV_STR_S2v(argv[3],&data);
		printf("chn[%d],reg[%d],data[%d]\n",chn,reg,data); 
		EXT_DVI_IicWriteIsl(&gDviDev,chn,reg,data);
	    //EXT_DVI_SetSw(outChn,inChn, ena );
	   return WV_SOK;
	}

  return WV_SOK;
}


/**********************************************************************************

WV_S32 EXT_DVI_Init();

**********************************************************************************/
WV_S32 EXT_DVI_Init() 
{  
     WV_S32 i;
     WV_CMD_Register("dvi","get","get dvi info",EXT_DVI_CMDGet);
    WV_CMD_Register("dvi","set","get dvi conf",EXT_DVI_CMDSet);
     // IIC config
    gDviDev.iicBus  = 0;
    gDviDev.swAddr  = 0x40;
    for(i=0;i<8;i++){
	    gDviDev.islAddr[i] = i*2;
	    }
	//gpio config

    for(i=0;i<4;i++){
	  //   gDviDev.inSel[i].bank = 6;
      //   gDviDev.inSel[i].num  = i*2;  
      //   gDviDev.edidSel[i].bank = 6;
      //   gDviDev.edidSel[i].num  = i*2+1;  

    	 gDviDev.outSel[i].bank = 8;
    	 gDviDev.outSel[i].num  = i+4;   
        }

	     gDviDev.inSel[0].bank = 6;
    	 gDviDev.inSel[0].num  = 0;  
	     gDviDev.inSel[1].bank = 6;
    	 gDviDev.inSel[1].num  = 3;  
	     gDviDev.inSel[2].bank = 6;
    	 gDviDev.inSel[2].num  = 4;  
	     gDviDev.inSel[3].bank = 6;
    	 gDviDev.inSel[3].num  = 7;  

    	 gDviDev.edidSel[0].bank = 6;
    	 gDviDev.edidSel[0].num  = 1; 
    	 gDviDev.edidSel[1].bank = 6;
    	 gDviDev.edidSel[1].num  = 2; 
    	 gDviDev.edidSel[2].bank = 6;
    	 gDviDev.edidSel[2].num  = 5; 
    	 gDviDev.edidSel[3].bank = 6;
    	 gDviDev.edidSel[3].num  = 6; 
  
         
	 	 gDviDev.drvSel.bank = 7;
		 gDviDev.drvSel.num= 4;    
		 
		 gDviDev.islRset.bank = 7;
		 gDviDev.islRset.num = 5;

		 gDviDev.swRset.bank = 7;
		 gDviDev.swRset.num = 1; 
     // init sw
	for(i=0;i<5;i++){
	     gDviDev.sw[i] = i; 
         } 
            
     EXT_DVI_DevOpen(&gDviDev);
	for(i=0;i<4;i++)
	{
    	EXT_DVI_SetSw(i, i, 1);
     }  
    EXT_DVI_SetSw(4, 0, 1);  
    //EXT_DVI_SetSw(3, 4, 1);  	    
   return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_DVI_DeInit();

**********************************************************************************/
WV_S32 EXT_DVI_DeInit() 
{
  EXT_DVI_DevClose(&gDviDev);
  
  return WV_SOK;
}




