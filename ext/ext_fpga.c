#include"ext_fpga.h"
#include"his.h"
#include"ext.h"

#define    EXT_FPGA_QUE_BUFLEN_MAX    256
#define    EXT_FPGA_QUE_DEPTH_MAX     1024 

typedef struct EXT_FPGA_DEV_E 
{
//
  WV_THR_HNDL_T     thrHndl; 
  WV_U32      		open;
  WV_U32      		close;  

  HIS_SSP_HNDL      sspHndl0;
  HIS_SSP_HNDL      sspHndl1;
  WV_U8             *pBuf;
  //WQUE
 WV_QUE_DEV_E     	queKeyTx[4];
 WV_QUE_DEV_E      	queSvrRx[4];
 WV_QUE_DEV_E     	queSvrTx[4]; 
 WV_QUE_DEV_E     	queSwRx; 
 WV_QUE_DEV_E     	queFree;
  //
  WV_U16           confFlag[4];
  WV_U16           uartChek[4];
  WV_U16           uartBaud[4];
  WV_U16           uartStop[4];
  // statute 
  WV_U32       		width[2]; 
  WV_U32       		height[2];
  WV_U32       		hvsw[5];
  WV_U32       		vgaHfrq;
  //
 
	
}EXT_FPGA_DEV_E;

static  EXT_FPGA_DEV_E  gFpgaDev;


/****************************************************************************

WV_S32 EXT_FPGA_SpiWrite(WV_U16 addr,WV_U16,data)

****************************************************************************/

WV_S32 EXT_FPGA_SpiWrite(EXT_FPGA_DEV_E *pDev,WV_U16 addr,WV_U16 data)
{
    WV_U8   addrBuf[2]; 
    WV_U8   dataBuf[2];
     WV_U16 addr_b; 
   addr_b  = addr & 0xff;
   addrBuf[0] = (addr_b >> 2) |0x80;//| 0x8000;
   addrBuf[1] = (addr_b << 6) &  0xff;
   dataBuf[0] = (data >> 8) & 0xff;
   dataBuf[1] = data & 0xff;
   WV_CHECK_RET(  HIS_SSP_Write(pDev->sspHndl0,addrBuf,2,dataBuf,2)); 
  //WV_printf("EXT_FPGA_SpiWrite reg[%#x] = %#x\r\n",addr,data);  
   return WV_SOK;
}

/****************************************************************************

WV_S32 EXT_FPGA_Write(WV_U16 addr,WV_U16,data)

****************************************************************************/

WV_S32 EXT_FPGA_Write(WV_U16 addr,WV_U16 data)
{
    
  EXT_FPGA_SpiWrite(&gFpgaDev,addr,data);
  
   return WV_SOK;
}

/****************************************************************************

WV_S32 EXT_FPGA_SpiRead(WV_U16 addr,WV_U16,data)

****************************************************************************/

WV_S32 EXT_FPGA_SpiRead(EXT_FPGA_DEV_E *pDev,WV_U16 addr,WV_U16 *pData)
{
   WV_U8  addrBuf[2];
   WV_U8   dataBuf[2]; 
   WV_U16 addr_b;  
   addr_b  = addr & 0xff;
   addrBuf[0] = (addr_b >> 2) &  0X3f ;//| 0x8000;
   addrBuf[1] = (addr_b << 6) &  0xff;
 
   WV_CHECK_RET(  HIS_SSP_Read(pDev->sspHndl0,addrBuf,2,dataBuf,2));   
   
   *pData = ((dataBuf[0] & 0xff)  << 8) | (dataBuf[1] & 0xff);
  // WV_printf("EXT_FPGA_SpiRead reg[%#x] = %#x\r\n",addr,*pData);  
   return WV_SOK;
}



/****************************************************************************

WV_S32 EXT_FPGA_SwSet(EXT_FPGA_DEV_E *pDev,WV_U8 *pData)

****************************************************************************/

WV_S32 EXT_FPGA_SwWrite(EXT_FPGA_DEV_E *pDev,WV_U8 *pData)
{
   WV_U16  addrBuf;
   WV_S32 i,j;
  WV_U8  data;
 /* WV_printf("vga sw reg: \r\n");
   for(i=0;i<21;i++){
      WV_printf("%d  : %#x\r\n",i,pData[i]); 
		
	}*/
  //EXT_FPGA_SpiWrite(pDev,0x20,1);
   //EXT_FPGA_SpiWrite(pDev,0x21,1);
   for(i=0;i<5;i++){ 
   pDev->hvsw[i] = pData[16+i] & 0xff;
   EXT_FPGA_SpiWrite(pDev,0x23+i, pDev->hvsw[i]); 
   }
  /*
   EXT_FPGA_SpiWrite(pDev,0x20,0);
   WV_CHECK_RET(  HIS_SSP_Write(pDev->sspHndl1,(WV_U8 *) &addrBuf,0,pData,10)); 
   EXT_FPGA_SpiWrite(pDev,0x21,0);
   //usleep(100);
   EXT_FPGA_SpiWrite(pDev,0x21,1); 
  // usleep(100);
   EXT_FPGA_SpiWrite(pDev,0x20,1);
   */
   EXT_FPGA_SpiWrite(pDev,0x28,0xff);  //ck = 1 
   EXT_FPGA_SpiWrite(pDev,0x28,0xfd);  //ce = 0 
  for(i=0;i<16;i++)
  {
       data = pData[15-i];//0x12;
       for(j=0;j<5;j++)
         {
		    if((data & 0x10) == 0x10){            

		    EXT_FPGA_SpiWrite(pDev,0x28,0xfd);  //ck = 1  data  
		    EXT_FPGA_SpiWrite(pDev,0x28,0xf9);  //ck = 0  data 
		    }
		    else{
		     EXT_FPGA_SpiWrite(pDev,0x28,0xfc);  //ck = 1  data  
		     EXT_FPGA_SpiWrite(pDev,0x28,0xf8);  //ck = 0  data   
		    } 
		    data = data <<1;       
           }
   }
 EXT_FPGA_SpiWrite(pDev,0x28,0xfd);  //ch = 1
 EXT_FPGA_SpiWrite(pDev,0x28,0xf5);  //updata = 0
 EXT_FPGA_SpiWrite(pDev,0x28,0xfd);  //updata = 1
 EXT_FPGA_SpiWrite(pDev,0x28,0xff);  //ce = 1
  
   return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_FPGA_UartSet(EXT_FPGA_DEV_E *pDev,WV_U32 chn)

****************************************************************************/

WV_S32 EXT_FPGA_UartSet(EXT_FPGA_DEV_E *pDev,WV_U32 chn)
{
    WV_U16   addr,data;
     
     if( pDev ->confFlag[chn] == 0)  return WV_SOK;
   
    addr = 0x30+ chn;//rx
    
    data = 100000000/pDev->uartBaud[chn];
    EXT_FPGA_SpiWrite(pDev,addr,data/16);  
     addr = 0x34+ chn;//tx
    EXT_FPGA_SpiWrite(pDev,addr,data);  
    data =0;
    if(pDev->uartChek[chn] == 1)
    {
       data |= 0x01;
    }
   if(pDev->uartStop[chn] == 2)
    {
       data |= 0x02;
    }
    addr = 0x38+ chn;//tx
    EXT_FPGA_SpiWrite(pDev,addr,data);  
    pDev ->confFlag[chn] = 0;
	return WV_SOK;
}

/****************************************************************************

WV_S32 EXT_FPGA_KeyTx(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_KeyTx(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 len)
{
   WV_U8  *pQue; 
   WV_CHECK_RET( WV_QUE_Get(&(pDev->queFree),(WV_U32 *)(&pQue),0) );
   
   *((WV_U32 *)pQue) = len;
    memcpy(pQue + 4,pBuf,len);
    WV_CHECK_RET( WV_QUE_Put(&(pDev->queKeyTx[chn]),(WV_U32)pQue,0) );
   return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_FPGA_KeySend(EXT_FPGA_DEV_E *pDev,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_KeySend(EXT_FPGA_DEV_E *pDev,WV_U32 chn)
{
   WV_U8  *pQue;
   WV_U16  data,addr;
   WV_S32 ret,i;
   WV_S32  len;
  ret =WV_QUE_Get(&(pDev->queKeyTx[chn]),(WV_U32 *)(&pQue),0);
  if(ret != WV_SOK)
  	{
  	   return WV_SOK;
  	}
   len =*((WV_U32 *)pQue); 
  addr = 0x08 + chn*2;
//WV_U32 j;
//for(j=0;j<4;j++)
 // {
  //  addr = 0x08 + j*2;
    for(i=0;i<len;i++)
    {
      data = pQue[4+i] & 0Xff;      
      EXT_FPGA_SpiWrite(pDev,addr,data); 

     // while(1)
      //{
       //       EXT_FPGA_SpiRead(pDev,addr+1,&data); 
       //       if((data &  0x1000) == 0x1000)  break;
      //   }
      //usleep(1); 
    }

 // }
  WV_CHECK_RET( WV_QUE_Put(&(pDev->queFree),(WV_U32)pQue,0));
  
   return WV_SOK;
}



/****************************************************************************

WV_S32 EXT_FPGA_SvrTx(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_SvrTx(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 len)
{
   WV_U8  *pQue; 
   WV_CHECK_RET( WV_QUE_Get(&(pDev->queFree),(WV_U32 *)(&pQue),0) );
   
   *((WV_U32 *)pQue) = len;
    memcpy(pQue + 4,pBuf,len);
    WV_CHECK_RET( WV_QUE_Put(&(pDev->queSvrTx[chn]),(WV_U32)pQue,0) );
   return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_FPGA_SvrSend(EXT_FPGA_DEV_E *pDev,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_SvrSend(EXT_FPGA_DEV_E *pDev,WV_U32 chn)
{
   WV_U8  *pQue;
   WV_U16  data,addr;
   WV_S32 i,ret;
   WV_U32  len;
   
  ret = WV_QUE_Get(&(pDev->queSvrTx[chn]),(WV_U32 *)(&pQue),0);
  if(ret != WV_SOK)
  	{
  	   return WV_SOK;
  	}
   len =*((WV_U32 *)pQue); 
   addr = 0x10 + chn*4;
    for(i=0;i<len;i++)
    {
      data = pQue[4+i] & 0Xff;
      EXT_FPGA_SpiWrite(pDev,addr,data);        
    }
  
  WV_CHECK_RET( WV_QUE_Put(&(pDev->queFree),(WV_U32)pQue,0));
  
   return WV_SOK;
}





/****************************************************************************

WV_S32 EXT_FPGA_SvrGet(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_SvrGet(EXT_FPGA_DEV_E *pDev,WV_U32 chn)
{
   WV_U16 addr,data;
   WV_S32  i;
   WV_U8  *pQue,buf[EXT_FPGA_QUE_BUFLEN_MAX],empty; 
  
   empty = 0;
   i = 0;
    addr = 0x11 +chn *4; 
   while(empty==0)
   {
	 
      EXT_FPGA_SpiRead(pDev,addr,&data);
      if((data & 0x8000) == 0x8000){
 
    	empty = 1; 
             
      }
      else{ 
       buf[i] =  data & 0xff;
       i++;
       if(i == EXT_FPGA_QUE_BUFLEN_MAX-4) empty = 1;
      }
   } 
   if(i == 0)
   {
     return  WV_SOK;
   }
    
    WV_CHECK_RET( WV_QUE_Get(&(pDev->queFree),(WV_U32 *)(&pQue),0) );
   
   *((WV_U32 *)pQue) = i;
    memcpy(pQue + 4,buf,i);
    WV_CHECK_RET( WV_QUE_Put(&(pDev->queSvrRx[chn]),(WV_U32)pQue,0) );
	
   return WV_SOK;
}


/****************************************************************************

WV_S32 EXT_FPGA_SvrRx(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_SvrRx(EXT_FPGA_DEV_E *pDev,WV_U32 chn ,WV_U8 *pBuf,WV_U32 *len)
{
   WV_U8  *pQue;
	WV_S32 ret=-1;
   ret= WV_QUE_Get(&(pDev->queSvrRx[chn]),(WV_U32 *)(&pQue),0);
	if(ret != 0 )
	{
		return -1;
	}
   *len = *((WV_U32 *)pQue);
    memcpy(pBuf,pQue + 4,*len);
    WV_CHECK_RET( WV_QUE_Put(&(pDev->queFree),(WV_U32)pQue,0) );
   return WV_SOK;
}


//
/****************************************************************************

WV_S32 EXT_FPGA_SwSet(EXT_FPGA_DEV_E *pDev,WV_U8 *pBuf,WV_U32 len)

****************************************************************************/

WV_S32 EXT_FPGA_SwSet(EXT_FPGA_DEV_E *pDev,WV_U8 *pBuf,WV_U32 len)
{
   WV_U8  *pQue;
   WV_CHECK_RET( WV_QUE_Get(&(pDev->queFree),(WV_U32 *)(&pQue),0) );
   *((WV_U32 *)pQue) = len;
   memcpy(pQue + 4,pBuf,len);
   WV_S32 i;
   //for(i=0;i<len;i++)
  //	{
    //   WV_printf("QUE SW set  %d  = %#x - %#x\r\n",i,pQue[i+4],pBuf[i] );
	//}

   WV_CHECK_RET( WV_QUE_Put(&(pDev->queSwRx),(WV_U32)pQue,0) );
   return WV_SOK;
}

/****************************************************************************

WV_S32 EXT_FPGA_SwSend(EXT_FPGA_DEV_E *pDev)

****************************************************************************/

WV_S32 EXT_FPGA_SwSend(EXT_FPGA_DEV_E *pDev)
{
   WV_U8  *pQue,*pBuf; 
   WV_S32 ret;
   ret = WV_QUE_Get(&(pDev->queSwRx),(WV_U32 *)(&pQue),0);
   if(ret != WV_SOK)  return WV_EFAIL;
   pBuf = pQue +4;
	WV_S32 i;
  // for(i=0;i<19;i++)
  //	{
  //     WV_printf("QUE SW send  %d  = %#x\r\n",i,pQue[i]);
	//}
   EXT_FPGA_SwWrite(pDev,pBuf); 
   WV_CHECK_RET( WV_QUE_Put(&(pDev->queFree),(WV_U32)pQue,0) );
   return WV_SOK;
}



/****************************************************************************

WV_S32 EXT_FPGA_ReadResolution(EXT_FPGA_DEV_E *pDev)

****************************************************************************/

WV_S32 EXT_FPGA_ReadResolution(EXT_FPGA_DEV_E *pDev)
{
     WV_U16 data;
    EXT_FPGA_SpiRead(pDev,0x02,&data);  
    pDev -> width[0] = data & 0xfff;
  // WV_printf(" ReadResolution %#x = %#x\r\n",0x55,data);
    EXT_FPGA_SpiRead(pDev,0x03,&data);  
  pDev -> height[0] = data & 0xfff;


   EXT_FPGA_SpiRead(pDev,0x04,&data);  
   pDev -> width[1] = data & 0xfff; 


 	EXT_FPGA_SpiRead(pDev,0x05,&data);  
    pDev -> height[1] = data & 0xfff; 

    //get  vga h clonum 100M 
    EXT_FPGA_SpiRead(pDev,0x2e,&data);   
    pDev ->vgaHfrq    = data; 

   return WV_SOK;
}
 
 
  
/**********************************************************************************

WV_S32  EXT_FPGA_DevOpen(EXT_FPGA_DEV_E  * pDev);

 **********************************************************************************/
 
WV_S32 EXT_FPGA_DevOpen( EXT_FPGA_DEV_E  * pDev)
 {
    WV_S32 i;
     WV_CHECK_RET( HIS_SSP_Open(&pDev->sspHndl0,0,3));
    WV_CHECK_RET( HIS_SSP_Open(&pDev->sspHndl1,1,3));
    pDev->pBuf = (WV_U8*) malloc(EXT_FPGA_QUE_BUFLEN_MAX *EXT_FPGA_QUE_DEPTH_MAX *13);
    for(i = 0;i<4;i++){
		WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queKeyTx[i]),EXT_FPGA_QUE_DEPTH_MAX)  );  
		WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queSvrRx[i]),EXT_FPGA_QUE_DEPTH_MAX)  ); 
		WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queSvrTx[i]),EXT_FPGA_QUE_DEPTH_MAX)  );  
		 
		}
  WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queSwRx),EXT_FPGA_QUE_DEPTH_MAX)  );  
  WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queFree),EXT_FPGA_QUE_DEPTH_MAX*13)  );  
  for(i =0;i<EXT_FPGA_QUE_DEPTH_MAX*13;i++){
  	  WV_CHECK_RET(  WV_QUE_Put(&(pDev->queFree), (WV_U32)(pDev->pBuf+ EXT_FPGA_QUE_BUFLEN_MAX*i),0) );
  	}
  	
     EXT_FPGA_SpiWrite(pDev,0x22,0);
     usleep(1);
     EXT_FPGA_SpiWrite(pDev,0x22,1);
        
  	for(i=0;i<4;i++){
  	     pDev ->uartChek[i] = 0;
  	     pDev ->uartStop[i] = 1;
  	     pDev ->uartBaud[i] = 9600;
  	     pDev ->confFlag[i] = 1;
  		}
    for(i=0;i<4;i++){
  	     pDev->width[i] = 0;
  	     pDev ->height[i] = 0; 
  		}	
  	
 return  WV_SOK;
 }
  
  


  
/**********************************************************************************

WV_S32  EXT_FPGA_DevClose(EXT_FPGA_DEV_E  * pDev);

 **********************************************************************************/
 

WV_S32  EXT_FPGA_DevClose( EXT_FPGA_DEV_E  * pDev)
 {
    WV_S32 i;
   	WV_CHECK_RET( HIS_SSP_Close(&pDev->sspHndl0));
    WV_CHECK_RET( HIS_SSP_Close(&pDev->sspHndl1));
    
    for(i = 0;i<4;i++){
		WV_CHECK_RET(   WV_QUE_Destroy(  &(pDev->queKeyTx[i]))  );  
		WV_CHECK_RET(   WV_QUE_Destroy(  &(pDev->queSvrRx[i]))  ); 
		WV_CHECK_RET(   WV_QUE_Destroy(  &(pDev->queSvrTx[i]))  );  
		 
		}
  WV_CHECK_RET(   WV_QUE_Destroy(  &(pDev->queSwRx))  );  
  WV_CHECK_RET(   WV_QUE_Destroy(  &(pDev->queFree))  );  
 
 
  	free(pDev->pBuf);
 return  WV_SOK;
 }
    
/**********************************************************************************

void *TSK_FPGA_Proc(Void * prm)

 **********************************************************************************/
 																																																																																										
 void *TSK_FPGA_Proc(void * prm)
 {
 
 EXT_FPGA_DEV_E  * pDev;  
 WV_S32 i;
 pDev = (EXT_FPGA_DEV_E *) prm; 
EXT_FPGA_DevOpen(pDev);
 pDev-> open  = 1;
 pDev-> close  = 0; 
 
  
 while(pDev -> open == 1)
    { 
       for(i =0;i<4;i++){
             EXT_FPGA_UartSet(pDev,i);
       		 EXT_FPGA_KeySend(pDev,i);
       		 EXT_FPGA_SvrSend(pDev,i);
       		 EXT_FPGA_SvrGet(pDev,i);
       		}
       EXT_FPGA_SwSend(pDev);
       EXT_FPGA_ReadResolution(pDev);
        //usleep(1);
     
    } 
EXT_FPGA_DevClose(pDev); 
 return NULL;
 }
 
/********************************************************************

WV_S32 EXT_FPGA_SendUart(WV_U8  chn, WV_U8 *pBuf, WV_U32 len )

********************************************************************/

WV_S32 EXT_FPGA_SendUart(WV_U8  chn, WV_U8 *pBuf, WV_U32 len )
{
  return  EXT_FPGA_SvrTx(&gFpgaDev,chn ,pBuf,len);
}



/********************************************************************

WV_S32  EXT_FPGA_SendKB(WV_U8  chn, WV_U8 *pBuf, WV_U32 len )

********************************************************************/

WV_S32  EXT_FPGA_SendKB(WV_U8  chn, WV_U8 *pBuf, WV_U32 len )
{
  return  EXT_FPGA_KeyTx(&gFpgaDev,chn ,pBuf,len);
}

/********************************************************************

WV_S32  EXT_FPGA_GetUart(WV_U8  chn, WV_U8 *pBuf, WV_U32 len )

********************************************************************/

WV_S32  EXT_FPGA_GetUart(WV_U8  chn, WV_U8 *pBuf, WV_U32 *len )
{
  return  EXT_FPGA_SvrRx(&gFpgaDev,chn ,pBuf,len);
} 

/********************************************************************

WV_S32 EXT_FPGA_GetResolution(WV_U8  chn, WV_U32 *pWidth, WV_U32 *pHeight)

********************************************************************/
WV_S32 EXT_FPGA_GetResolution(WV_U8  chn, WV_U32 *pWidth, WV_U32 *pHeight,WV_U32 *pMode)
{


	WV_U16 data;
	WV_U32 width=0,hight=0;

//	EXT_FPGA_SpiRead(&gFpgaDev,0x06,&data);  
//	width = data & 0xfff; 
//	EXT_FPGA_SpiRead(&gFpgaDev,0x07,&data);  
//	hight = data & 0xfff; 

	if(chn == 1)
	{
		EXT_FPGA_SpiRead(&gFpgaDev,0x41,&data);
		if( (data & 0x100) == 0 )
		{
			*pMode = 2;
			*pWidth = gFpgaDev.width[chn] * 2;
			*pHeight = gFpgaDev.height[chn];
	
		}

		if( (data & 0x100) == 0x100 )
		{
			*pMode = 0;
			*pWidth = gFpgaDev.width[chn];
			*pHeight = gFpgaDev.height[chn];	
		} 
	}else if(chn == 0)
	{

		*pMode = 0;
		*pWidth = gFpgaDev.width[chn];
		*pHeight = gFpgaDev.height[chn];	
	}

	
	
   return WV_SOK;
}

/********************************************************************

WV_S32 EXT_FPGA_GetVgaHnum(WV_U32 *nClkNum)

********************************************************************/
WV_S32 EXT_FPGA_GetVgaHnum(WV_U32 *nClkNum)
{
   *nClkNum = gFpgaDev.vgaHfrq;
   
   return WV_SOK;
}

/********************************************************************

WV_S32  EXT_FPGA_SetADV3226(WV_U8 *pBuf);

********************************************************************/
WV_S32  EXT_FPGA_SetADV3226(WV_U8 *pBuf,WV_U32 len)
{
   return EXT_FPGA_SwSet(&gFpgaDev,pBuf,len);
}

/********************************************************************

WV_S32  EXT_FPGA_ConfUart(WV_U8 chn,WV_U32 burd,WV_32 check,WV_U32 stop);  

********************************************************************/
WV_S32  EXT_FPGA_ConfUart(WV_U8 chn,WV_U32 burd,WV_U32 check,WV_U32 stop)
{
	 

	gFpgaDev.uartBaud[chn] = burd;
	gFpgaDev.uartChek[chn] = check;
	gFpgaDev.uartStop[chn] = stop;
	gFpgaDev.confFlag[chn] = 1; 
    return WV_SOK;
}

/****************************************************************************

WV_S32 EXT_FPGA_CCMDGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_FPGA_CMDGet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

    WV_U32 chn;
   WV_U32 addr;   
   WV_U16 data;
   
   if( argc <1  )
	{ 
	    prfBuff += sprintf(prfBuff,"fpga  get  reg /uart/vi// get fpga info\r\n"); 
		return WV_SOK;  
    } 
   if(strcmp(argv[0],"reg") ==0)
	{
		if(argc < 2)
		{ 
			prfBuff += sprintf(prfBuff,"fpga get reg  <addr>\r\n"); 
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&addr);
		 
		EXT_FPGA_SpiRead(&gFpgaDev ,(WV_U16)addr,&data); 
	    prfBuff += sprintf(prfBuff,"reg[%d] = %d[%3x]\r\n",addr,data,data); 
		return WV_SOK;  
	} 
  if(strcmp(argv[0],"uart") ==0)
	{
		if(argc < 2)
		{ 
			prfBuff += sprintf(prfBuff,"fpga get uart  <chn>\r\n"); 
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn);  
	    prfBuff += sprintf(prfBuff,"Uart[%d] burd= %d[%d]\r\n",chn,gFpgaDev.uartBaud[chn],100000000/gFpgaDev.uartBaud[chn]); 
	    prfBuff += sprintf(prfBuff,"Uart[%d] stop= %d\r\n",chn,gFpgaDev.uartStop[chn]); 
  	    prfBuff += sprintf(prfBuff,"Uart[%d] check= %d\r\n",chn,gFpgaDev.uartChek[chn]);
		return WV_SOK;  
	}
  if(strcmp(argv[0],"vi") ==0)
	{
		if(argc < 2)
		{ 
			prfBuff += sprintf(prfBuff,"fpga get uart  <chn>\r\n"); 
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn);  
	    prfBuff += sprintf(prfBuff,"vi[%d]= %d*%d\r\n",chn,gFpgaDev.width[chn],gFpgaDev.height[chn]); 
	    prfBuff += sprintf(prfBuff,"vga  hfrq  =  %d   -  %d   - %f \r\n",gFpgaDev.vgaHfrq,gFpgaDev.vgaHfrq*5/4,  100000000.0/gFpgaDev.vgaHfrq); 
	   
		return WV_SOK;  
	}
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
    return WV_SOK;
}

/****************************************************************************

WV_S32 EXT_FPGA_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_FPGA_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

    WV_U32 chn;
   WV_U32 addr;   
   WV_U32 data,burd,check,stop;
   
   if( argc <1  )
	{ 
	    prfBuff += sprintf(prfBuff,"fpga  set  reg /uart  // set fpga info\r\n"); 
		return WV_SOK;  
    } 
   if(strcmp(argv[0],"reg") ==0)
	{
		if(argc < 2)
		{ 
			prfBuff += sprintf(prfBuff,"fpga set reg  <addr><data>\r\n"); 
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&addr); 
		WV_STR_S2v(argv[2],&data); 
		EXT_FPGA_SpiWrite(&gFpgaDev ,(WV_U16)addr,(WV_U16)data); 
	    prfBuff += sprintf(prfBuff,"reg[%d] = %d[%3x]\r\n",addr,data,data); 
		return WV_SOK;  
	} 
  if(strcmp(argv[0],"uart") ==0)
	{
		if(argc < 5)
		{ 
			prfBuff += sprintf(prfBuff,"fpga set uart <chn>  <baud> <chek> <stop>\r\n"); 
			return WV_SOK; 
		}
			WV_STR_S2v(argv[1],&chn);  
		WV_STR_S2v(argv[2],&burd);
		WV_STR_S2v(argv[3],&check);
		WV_STR_S2v(argv[4],&stop);
	
        EXT_FPGA_ConfUart(chn,burd,check,stop);
         prfBuff += sprintf(prfBuff,"config uart[%d] = %d - %d -%d\r\n",chn,burd,check,stop); 
		return WV_SOK;  
	} 
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
    return WV_SOK;
}








/*********************************************************************

WV_S32 EXT_FPGA_Init();

*********************************************************************/
WV_S32 EXT_FPGA_Init()
{
 	WV_CMD_Register("fpga","get","get fpga info",TSK_FPGA_CMDGet);
    WV_CMD_Register("fpga","set","get fpga conf",TSK_FPGA_CMDSet);

    WV_THR_Create(&(gFpgaDev.thrHndl), TSK_FPGA_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gFpgaDev);
    WV_printf("EXT_FPGA_Init ok!\r\n");
	return  WV_SOK;
}

/*********************************************************************

WV_S32 EXT_FPGA_DeInit();

*********************************************************************/
WV_S32 EXT_FPGA_DeInit()
{
	if(gFpgaDev.open == 1)
			{
			gFpgaDev.open = 0;
			while(gFpgaDev.close == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gFpgaDev.thrHndl))   );	
			} 
    WV_printf("EXT_FPGA_DeInit ok!\r\n");
	return  WV_SOK;
}

