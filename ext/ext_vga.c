#include"ext_vga.h"
#include"his_iic.h"
#include"his_gpio.h"
#include"his_ssp.h"
#include"ext_fpga.h"

typedef struct EXT_VGA_CHN_E 
{
  
  WV_U8  r;
  WV_U8  g;
  WV_U8  b;
}EXT_VGA_CHN_E ;
 
static  EXT_VGA_CHN_E   vgaInMap[] =
{
   {0,1,2},
   {3,4,5},
   {6,7,8},
   {9,10,11},
   {12,13,14}
}; 
/*
static  EXT_VGA_CHN_E   vgaInMap[] =
{
   {0,0,0},
   {0,0,0},
   {0,0,0},
   {0,0,0},
   {0,0,0}
};
*/
static  EXT_VGA_CHN_E   vgaOutMap[] =
{
   {0,1,2},
   {3,4,5},
   {6,7,8},
   {9,10,11},
   {12,13,14}
};

static int gVgaSwflag;
typedef struct EXT_VGA_DEV_E 
{
// 
  HIS_IIC_HNDL      iicHndl;
  HIS_GPIO_DEV_E    inSel[4];
  HIS_GPIO_DEV_E    edidSel[4];
  HIS_GPIO_DEV_E    outSel[4];  
  HIS_GPIO_DEV_E    swRset;
  HIS_SSP_HNDL      sspHndl;
  // statute 
  WV_U8       		iicBus; 
  WV_U8       		iicAddr;
  //
  WV_U8             sw[20];
   WV_U8            hv[5]; 
  //
  WV_U32          decWidth;
  WV_U32          decHeight;
  WV_U32          decFrameRate; 
  //
   WV_U8          edid[256]; 
  
	
}EXT_VGA_DEV_E ;

static EXT_VGA_DEV_E  gVgaDev;

#if 0
WV_U8  EXT_VGA_EDID[128] = 
{ 
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x52,0x49,0xDB,0x76,0x63,0x1C,0x00,0x00,
    0x2A,0x10,0x01,0x03,0x08,0x34,0x21,0x78,0x2A,0x5A,0xD5,0xA7,0x56,0x4B,0x9B,0x24,
    0x13,0x50,0x54,0xBD,0xEF,0x80,0x71,0x4F,0x81,0xC0,0x81,0x80,0x8B,0xC0,0x90,0x40,
    0x95,0x00,0xA9,0x40,0x8C,0xC0,0x28,0x3C,0x80,0xA0,0x70,0xB0,0x23,0x40,0x30,0x20,
    0x36,0x00,0x07,0x44,0x21,0x00,0x00,0x1E,0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,
    0x58,0x2C,0x45,0x00,0xDD,0x0C,0x11,0x00,0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x38,
    0x4C,0x1E,0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
    0x00,0x54,0x52,0x49,0x5F,0x56,0x47,0x41,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x00,0x4B
}; //extention and check_sum

#endif
//SWKJ
#if 0
WV_U8  EXT_VGA_EDID[128] = 
{ 
	0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0x00 ,0x4E ,0xEB ,0x22 ,0x01 ,0x38 ,0x4B ,0x4A ,0x30,
	0x24 ,0x18 ,0x01 ,0x03 ,0x0E ,0x30 ,0x1B ,0x78 ,0x2A ,0x90 ,0xC1 ,0xA2 ,0x59 ,0x55 ,0x9C ,0x27,
	0x0E ,0x50 ,0x54 ,0xBF ,0xEF ,0x80 ,0x71 ,0x4F ,0x81 ,0xC0 ,0x81 ,0x00 ,0x81 ,0x80 ,0x95 ,0x00,
	0xA9 ,0xC0 ,0xB3 ,0x00 ,0x01 ,0x01 ,0x02 ,0x3A ,0x80 ,0x18 ,0x71 ,0x38 ,0x2D ,0x40 ,0x58 ,0x2C,
	0x45 ,0x00 ,0xDD ,0x0C ,0x11 ,0x00 ,0x00 ,0x1E ,0x00 ,0x00 ,0x00 ,0xFD ,0x00 ,0x38 ,0x4B ,0x1E,
	0x51 ,0x11 ,0x00 ,0x0A ,0x20 ,0x20 ,0x20 ,0x20 ,0x20 ,0x20 ,0x00 ,0x00 ,0x00 ,0xFC ,0x00 ,0x53,
	0x57 ,0x4B ,0x4A ,0x30 ,0x30 ,0x31 ,0x0A ,0x0A ,0x0A ,0x0A ,0x0A ,0x0A ,0x00 ,0x00 ,0x00 ,0xFF,
	0x00 ,0x48 ,0x54 ,0x4D ,0x46 ,0x39 ,0x30 ,0x30 ,0x36 ,0x37 ,0x32 ,0x0A ,0x20 ,0x20 ,0x00 ,0xAB
};
#endif
//SWKJ2 1920*1080 samsangC1A.dat
#if 1
WV_U8  EXT_VGA_EDID[128] = 
{ 
	0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0x00 ,0x4E ,0xEB ,0x6D ,0x0C ,0x32 ,0x4A ,0x5A ,0x5A,
	0x03 ,0x1A ,0x01 ,0x03 ,0x0E ,0x30 ,0x1B ,0x78 ,0x2A ,0x90 ,0xC1 ,0xA2 ,0x59 ,0x55 ,0x9C ,0x27,
	0x0E ,0x50 ,0x54 ,0xBF ,0xEF ,0x80 ,0x71 ,0x4F ,0x81 ,0xC0 ,0x81 ,0x00 ,0x81 ,0x80 ,0x95 ,0x00,
	0xA9 ,0xC0 ,0xB3 ,0x00 ,0x01 ,0x01 ,0x02 ,0x3A ,0x80 ,0x18 ,0x71 ,0x38 ,0x2D ,0x40 ,0x58 ,0x2C,
	0x45 ,0x00 ,0xDD ,0x0C ,0x11 ,0x00 ,0x00 ,0x1E ,0x00 ,0x00 ,0x00 ,0xFD ,0x00 ,0x38 ,0x4B ,0x1E,
	0x51 ,0x11 ,0x00 ,0x0A ,0x20 ,0x20 ,0x20 ,0x20 ,0x20 ,0x20 ,0x00 ,0x00 ,0x00 ,0xFC ,0x00 ,0x53,
	0x57 ,0x4B ,0x4A ,0x32 ,0x0A ,0x0A ,0x0A ,0x0A ,0x0A ,0x0A ,0x0A ,0x0A ,0x00 ,0x00 ,0x00 ,0xFF,
	0x00 ,0x48 ,0x34 ,0x4C ,0x48 ,0x31 ,0x30 ,0x30 ,0x33 ,0x32 ,0x38 ,0x0A ,0x20 ,0x20 ,0x00 ,0xB5
};

#endif
/*****************************************************

WV_S32  EXT_VGA_IicInSel(WV_S32  chn,EXT_VGA_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_VGA_IicInSel(WV_S32  chn,EXT_VGA_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> inSel[chn],1)  );
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_VGA_IicInDeSel(WV_S32  chn,EXT_VGA_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_VGA_IicInDeSel(WV_S32  chn,EXT_VGA_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> inSel[chn],0)  );
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_VGA_IicOutSel(WV_S32  chn,EXT_VGA_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_VGA_IicOutSel(WV_S32  chn,EXT_VGA_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> outSel[chn],1)  );
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_VGA_IicOutDeSel(WV_S32  chn,EXT_VGA_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_VGA_IicOutDeSel(WV_S32  chn,EXT_VGA_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> outSel[chn],0)  );
	return WV_SOK;
}


/*****************************************************

WV_S32  EXT_VGA_IicEdidSel(WV_S32  chn,EXT_VGA_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_VGA_IicEdidSel(WV_S32  chn,EXT_VGA_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> edidSel[chn],1)  );
	return WV_SOK;
}

/*****************************************************

WV_S32  EXT_VGA_IicEdidDeSel(WV_S32  chn,EXT_VGA_DEV_E *pDev);

*****************************************************/
WV_S32  EXT_VGA_IicEdidDeSel(WV_S32  chn,EXT_VGA_DEV_E *pDev)
{
	WV_CHECK_RET( HIS_GPIO_Set(pDev-> edidSel[chn],0)  );
	return WV_SOK;
}

/**********************************************************************************

WV_S32 EXT_VGA_Read(EXT_VGA_DEV_E * pDev ,WV_U8 *pData);

**********************************************************************************/
WV_S32 EXT_VGA_IicRead(EXT_VGA_DEV_E * pDev ,WV_U8 reg,WV_U8 *pData)
{ 
 
   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev->iicAddr, reg, pData, 1) );
  return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_VGA_Write(EXT_VGA_DEV_E * pDev ,WV_U8 data);

**********************************************************************************/
WV_S32 EXT_VGA_IicWrite(EXT_VGA_DEV_E * pDev ,WV_U8 reg,WV_U8 data)
{ 
 
  WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev->iicAddr, reg,&data, 1) );
  return WV_SOK;	
}


/**********************************************************************************

WV_S32 EXT_VGA_EdidWrite(EXT_VGA_DEV_E * pDev ,WV_U8 chn);

**********************************************************************************/
WV_S32  EXT_VGA_EdidWrite(EXT_VGA_DEV_E * pDev ,WV_U8 chn)
{ 
  WV_S32  i;
  EXT_VGA_IicInDeSel( chn,pDev); //
  EXT_VGA_IicEdidSel(chn,pDev);
  usleep(100000); 
  for(i=0;i<128;i++){
   		//WV_CHECK_RET(  HIS_IIC_Write8 (pDev-> iicHndl,0xa0, i,&Default_Dual_EDID[i], 1) );
        //HIS_IIC_Write8 (pDev->iicHndl,0xa0, i,&EXT_VGA_EDID[i], 1);
   } 
   EXT_VGA_IicEdidDeSel(chn,pDev);
   EXT_VGA_IicInSel( chn,pDev);  
  return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_VGA_SetSw(EXT_VGA_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_VGA_SetSwConf(WV_U8 out,WV_U8 in,WV_U8 ena,WV_U8 * pBuf)
{   
     WV_U8  inChn,outChn,swByte,bitNum,byteNum;
     WV_S32 i;
     inChn   =  in;
     outChn  =  out; 
 
     //  WV_printf("switch  %d to %d  ena = %d\r\n",in,out,ena); 
     swByte  = inChn & 0xF;
     if(ena == 1)
      {
       swByte  |=  0x10;
      }
     
      pBuf[out] =  swByte;
    //  WV_printf("EXT_VGA_SetSwConf swByte = %#x\r\n",swByte); 
     /*  
      for(i =0;i<5;i++)
      {
        // MSB first   
        byteNum = 9 -(outChn *5 + i)/8;
        bitNum  =  (outChn *5 + i)%8;
        bitNum  =   7 - bitNum;   //spi Msb first
        //bitNum  =   bitNum;   // spi Lsb first
        
        if( (swByte &  0x10) == 0x10 )
        {       
          pBuf[byteNum]  |=  0x01<<  bitNum;
        }
        else
        {
          pBuf[byteNum]  &=   ~(0x01<<  bitNum);
		}
		
         swByte =swByte<<1;
         //WV_printf("EXT_VGA_SetSwConf swByte = %d bitNum = %d  buf[%d]= %#x \r\n",swByte,bitNum,byteNum,pBuf[byteNum]); 
      }
      
   //  for(i=0;i<10;i++)
  	//{
    //   WV_printf("VGA swith  reg   %d  = %#x\r\n",i,pBuf[i] );
	//}
 
      */
     return WV_SOK;	
}



/**********************************************************************************

WV_S32 EXT_VGA_DevInit(EXT_VGA_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_VGA_DevInit(EXT_VGA_DEV_E * pDev )
{ 
    WV_S32 i;  
	
 //HIS_IIC_Open(&pDev-> iicHndl,pDev->iicBus);
    for(i=0;i<4;i++){
		HIS_GPIO_SetCfg(pDev-> inSel[i],0);
		HIS_GPIO_Set(pDev-> inSel[i],1);
		HIS_GPIO_SetCfg(pDev-> edidSel[i],0);
		HIS_GPIO_Set(pDev-> edidSel[i],0);
		//WV_printf("vga  edidsel[%d] bank[%d],pin[%d]\n",i,pDev-> edidSel[i].bank,pDev-> edidSel[i].num);
		HIS_GPIO_SetCfg(pDev-> outSel[i],0);
		HIS_GPIO_Set(pDev-> outSel[i],0); 
	   }
	HIS_GPIO_SetCfg(pDev->swRset,0);//
	HIS_GPIO_Set(pDev-> swRset,1);
	usleep(10000);
	HIS_GPIO_Set(pDev-> swRset,0);
	usleep(10000);
	HIS_GPIO_Set(pDev-> swRset,1);

	//WV_CHECK_RET( HIS_SSP_Open(&pDev-> sspHndl,1));
	//write vga edid	
	//WV_printf("************** vga write edid **********\n");
	HIS_IIC_Open(&pDev-> iicHndl,pDev->iicBus);
	for(i=0;i<4;i++)
	{
 		EXT_VGA_EdidWrite(pDev ,i);
	}

	for(i=0;i<5;i++)
	{
		EXT_VGA_SetSwConf(vgaOutMap[i].r,vgaInMap[i].r,1,pDev->sw);
		EXT_VGA_SetSwConf(vgaOutMap[i].g,vgaInMap[i].g,1,pDev->sw);
		EXT_VGA_SetSwConf(vgaOutMap[i].b,vgaInMap[i].b,1,pDev->sw);
		pDev->sw[16+i] = i;
	}
	  /************************************************/
	
	
	gVgaSwflag = 0;
//	 for(i=0;i<15;i++)
//{
//    WV_printf("vga SW reg  %d  = %#x\r\n",i,pDev->sw[i]);
//}
    
	  EXT_FPGA_SetADV3226(pDev->sw,30);//	
           
 return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_VGA_DevDeInit(EXT_VGA_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_VGA_DevDeInit(EXT_VGA_DEV_E * pDev )
{ 
     WV_CHECK_RET( HIS_IIC_Close  ( &pDev-> iicHndl) ); 
   // WV_CHECK_RET( HIS_SSP_Close(&pDev-> sspHndl)  );	
 
     return WV_SOK;	
}


/****************************************************************************

WV_S32 EXT_VGA_GetSw(WV_U8 *pSw)

****************************************************************************/
WV_S32 EXT_VGA_GetSw(WV_U8 *pSw)
{
	WV_S32  i;

	for(i=0;i<5;i++)
	{

		switch(gVgaDev.sw[i*3])
		{

			case 16:
				pSw[i] = 0;
				break;
			case 19:
				pSw[i] = 1;
				break;
			case 22:
				pSw[i] = 2;
				break;
			case 25:
				pSw[i] = 3;
				break;			
			case 28:
				pSw[i] = 4;
				break;
			default :
				pSw[i] = 5;
				break;									
		}
	}
	
	return 0;
}
/****************************************

WV_S32  EXT_VGA_SetSw(WV_U8  outChn, WV_U8  inChn, WV_U8 ena );

****************************************/

WV_S32  EXT_VGA_SetSw(WV_U8  outChn, WV_U8  inChn, WV_U8 ena )
{

	if(gVgaSwflag == 0 )
	{
		gVgaSwflag = 1;
		WV_S8 sw[5];
		WV_S8 key=0;
		
		EXT_VGA_GetSw(sw);
		EXT_FPGA_SendKB(sw[outChn],&key,1);//pop all key
		
        EXT_VGA_SetSwConf(vgaOutMap[outChn].r,vgaInMap[inChn].r,ena,gVgaDev.sw);
		EXT_VGA_SetSwConf(vgaOutMap[outChn].g,vgaInMap[inChn].g,ena,gVgaDev.sw);
		EXT_VGA_SetSwConf(vgaOutMap[outChn].b,vgaInMap[inChn].b,ena,gVgaDev.sw); 
		
		gVgaDev.sw[16+outChn] = inChn;
		
 	    //EXT_VGA_UpdateSw(&gVgaDev);
 	    EXT_FPGA_SetADV3226(gVgaDev.sw,30);//
		gVgaSwflag = 0;
	}else{
		WV_printf(" vga set sw is working,please Wait a few seconds...\n ");
		return WV_EFAIL;
	}
  return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_VGA_CMDGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_VGA_CMDGet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
	   WV_S32 i;
		WV_U8 data;  
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"vga get <sw>\r\n");
		return WV_SOK;
		}
    if(strcmp(argv[0],"sw") ==0)
	{
	   prfBuff += sprintf(prfBuff,"VGA SW config:\r\n");
	    for(i=0;i<5;i++)
	    {
			switch(gVgaDev.sw[i*3])
			{

				case 16:
					data = 0;
					break;
				case 19:
					data = 1;
					break;
				case 22:
					data = 2;
					break;
				case 25:
					data = 3;
					break;			
				case 28:
					data = 4;
					break;
				default :
					data = 5;
					break;									
			}
	       prfBuff += sprintf(prfBuff,"%d  : %d\r\n",i,data); 
			//prfBuff += sprintf(prfBuff,"%d  : %d\r\n",i,gVgaDev.sw[i]); 
	    }
	   
	   return WV_SOK;
	}
  return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_VGA_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_VGA_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
 
   
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"vga set  <sw> \r\n");
		return WV_SOK;
		}
   if(strcmp(argv[0],"sw") ==0)
	{
		if(argc < 4)
		{ 
			prfBuff += sprintf(prfBuff,"set vga  sw  <outChn> <inChn> <ena>\r\n");  
			return WV_SOK; 
		}
		WV_U32   inChn,outChn,ena;
		WV_STR_S2v(argv[1],&outChn);
	    WV_STR_S2v(argv[2],&inChn);	 
		WV_STR_S2v(argv[3],&ena); 
		EXT_VGA_SetSw(outChn,inChn,ena ); 
		return WV_SOK;  
	} 
    
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
	        
	return WV_SOK;
 	
}


/****************************************

WV_S32  EXT_VGA_Init();

****************************************/
WV_S32  EXT_VGA_Init()
{
    WV_S32 i;
    gVgaDev.iicBus  = 0;
    gVgaDev.iicAddr  = 0x98;
   //
   WV_CMD_Register("vga","get","get vga info",EXT_VGA_CMDGet);
   WV_CMD_Register("vga","set","set vga ",EXT_VGA_CMDSet); 
    for(i = 0;i<4;i++){
    	 gVgaDev.inSel[i].bank = 10;
    	 gVgaDev.inSel[i].num  = 0 + i*2;
    	 
    	 gVgaDev.edidSel[i].bank = 10;
    	 gVgaDev.edidSel[i].num  = 1 + i*2; 
    	 
    	 gVgaDev.outSel[i].bank = 14;
    	 gVgaDev.outSel[i].num  = 4 + i; 
    	
    	}
     gVgaDev.swRset.bank = 5;
     gVgaDev.swRset.num  = 7;
     
     EXT_VGA_DevInit(&gVgaDev ); 
      
     EXT_VGA_SetSw(4,0,1 ); 
     EXT_VGA_SetSw(0,0,1 ); 
      
     
	return WV_SOK;
}



/****************************************

WV_S32  EXT_VGA_DeInit();

****************************************/
WV_S32  EXT_VGA_DeInit()
{
	 EXT_VGA_DevDeInit( &gVgaDev);
	return WV_SOK;
}




  
