#include"tsk_venc.h"
#include"his.h"
#include"stream.h"

typedef struct TSK_VENC_DEV_E 
{
// 
 WV_THR_HNDL_T thrHndl; 
 WV_U32      open;
 WV_U32      close;  
 WV_U8       *pEsBuf;
 WV_ETH_UDP_TX_E  txSock; 
 

 //  config for video
  WV_U8       vPid;
  WV_S32      viDev;
  WV_S32      viChn;
  WV_S32      viClkMode;
  WV_U32      viWidth;
  WV_U32      viHeight;
  //WV_U32      viMode;
  WV_U32      viFrameRate;
  WV_U32      curWidth;
  WV_U32      curHeight;
  WV_U32      curClkMode;
  WV_U32      curFrameRate;
  WV_S32      vpssGrp;
  WV_S32      vencChn;
  WV_S32      encBitRate;
  WV_U32      encFratio;
  WV_U32      bitRates;
  // statute
  WV_U32      esLen;
  WV_U64      esPts;
  WV_U32      esCnt;
  WV_U32       pkgCnt;
  WV_U32       erroCnt;
  
}TSK_VENC_DEV_E;


#define  TSK_VENC_BUF_LENMAX             2560*1600*2

static TSK_VENC_DEV_E gVencDev[2];


/**********************************************************************************

HIS_VENC_DevOpen(TSK_VENC_DEV_E * pdev );

**********************************************************************************/

WV_S32  HIS_VENC_DevOpen(TSK_VENC_DEV_E *pDev )
{
    pDev-> bitRates = pDev->encBitRate * 1.0* pDev->viHeight*pDev->viWidth*pDev->viFrameRate/pDev-> encFratio/1920/1080/30;
 	WV_CHECK_RET(  HIS_VI_Init(pDev-> viDev,pDev->viClkMode,pDev->viWidth,pDev->viHeight)  );
 	WV_CHECK_RET(  HIS_VPSS_Init(pDev->vpssGrp,1,pDev->viWidth,pDev->viHeight,0)  );

 	WV_CHECK_RET(  HIS_VENC_Init(pDev->vencChn,1,0,pDev->viWidth,pDev->viHeight,pDev->viFrameRate,pDev->viFrameRate/pDev-> encFratio,pDev->bitRates) );
 	//WV_CHECK_RET(  HIS_VENC_Init(pDev->vencChn,0,0,1920,1080,pDev->viFrameRate/pDev-> encFratio,pDev->viFrameRate/pDev-> encFratio,pDev->bitRates) );
	//WV_CHECK_RET(  HIS_VENC_Init(pDev->vencChn,0,0,1920,1080,pDev->viFrameRate,pDev->viFrameRate/pDev-> encFratio,pDev->bitRates) );
 	WV_CHECK_RET(  HIS_SYS_VI_BindVpss(pDev->viChn,pDev->vpssGrp) );
 	WV_CHECK_RET(  HIS_SYS_VENC_BindVpss(pDev->vencChn,pDev->vpssGrp,0) );
 	WV_CHECK_RET(  WV_UDPTX_Creat(&(pDev->txSock)) );
 	pDev->pEsBuf = (WV_U8 *)malloc(HIS_SYS_MAX_PICWIDTH *HIS_SYS_MAX_PICWIDTH*2);
 return WV_SOK;	
}

/**********************************************************************************

WV_S32 HIS_VENC_DevClose(TSK_VENC_DEV_E *pDev );

**********************************************************************************/

WV_S32  HIS_VENC_DevClose(TSK_VENC_DEV_E *pDev )
{
    
    WV_CHECK_RET(  HIS_SYS_VENC_UnBindVpss(pDev->vencChn,pDev->vpssGrp,0) );
    WV_CHECK_RET(  HIS_SYS_VI_UnBindVpss(pDev->viChn,pDev->vpssGrp) );
    WV_CHECK_RET(  HIS_VENC_DeInit(pDev->vencChn) );
    WV_CHECK_RET(  HIS_VPSS_DeInit(pDev->vpssGrp,1)  );
    WV_CHECK_RET(  HIS_VI_DeInit(pDev->viDev)  ); 
 	WV_CHECK_RET(  WV_UDPTX_Destroy(&(pDev->txSock)) );
 	free(pDev->pEsBuf);
 return WV_SOK;	
}


/*******************************************************************

WV_S32 TSK_VENC_RegisterConf(WV_S32 chn);

*******************************************************************/
WV_S32 TSK_VENC_RegisterConf(WV_S32 chn)
{
	WV_S8  name[WV_CONF_NAME_MAX_LEN],value[WV_CONF_VALUE_MAX_LEN]; 
	
	sprintf(name,"Venc_%d_conf",chn); 
	SYS_ENV_Register(0,name,NULL,NULL);
	//
	sprintf(name,"Venc_%d_viDev",chn); 
	sprintf(value, "%d",chn*2);
    SYS_ENV_Register(1 ,name,value,"viDev num");
    //
   	sprintf(name,"Venc_%d_viChn",chn); 
	sprintf(value, "%d",chn*8);
    SYS_ENV_Register(1 ,name,value,"viChn num");
       //
   	sprintf(name,"Venc_%d_viMode",chn); 
	sprintf(value, "%d",0); 
    SYS_ENV_Register(1 ,name,value,"vi clk mode");
    //
   	sprintf(name,"Venc_%d_vpss",chn); 
	sprintf(value, "%d",chn); 
    SYS_ENV_Register(1 ,name,value,"vi cpssGrp");
    
        //
   	sprintf(name,"Venc_%d_vEnc",chn); 
	sprintf(value, "%d",chn); 
    SYS_ENV_Register(1 ,name,value,"venc chn");
        //
   	sprintf(name,"Venc_%d_bRate",chn); 
	sprintf(value, "%d",4000); 
    SYS_ENV_Register(1 ,name,value,"venc bitRate [Kbps]"); 
           //
   	sprintf(name,"Venc_%d_ip",chn); 
	sprintf(value, "226.100.100.01"); 
    SYS_ENV_Register(1 ,name,value,"venc send ip addr"); 
           //
   	sprintf(name,"Venc_%d_port",chn); 
	sprintf(value, "%d",7001); 
    SYS_ENV_Register(1 ,name,value,"venc send ip port");     
              //
   	sprintf(name,"Venc_%d_pid",chn); 
	sprintf(value, "%d",10+chn); 
    SYS_ENV_Register(1 ,name,value,"video pid"); 
   
               //
   	sprintf(name,"Venc_%d_width",chn); 
	sprintf(value, "%d",1920); 
    SYS_ENV_Register(1 ,name,value,"video width"); 
                   //
   	sprintf(name,"Venc_%d_height",chn); 
	sprintf(value, "%d",1080); 
    SYS_ENV_Register(1 ,name,value,"video height"); 
     //
   	sprintf(name,"Venc_%d_frt",chn); 
	sprintf(value, "%d",60); 
    SYS_ENV_Register(1 ,name,value,"video FrameRate"); 
    
    //   //vencFratio
	sprintf(name,"Venc_%d_fRatio",chn); 
	sprintf(value, "%d",1); 
    SYS_ENV_Register(1 ,name,value,"video framerate Ratio");  
    
    return WV_SOK;  
       
}




/*******************************************************************

WV_S32 TSK_VENC_GetConf(WV_S32 chn,TSK_VENC_DEV_E *pDev);

*******************************************************************/
WV_S32 TSK_VENC_GetConf(WV_S32 chn,TSK_VENC_DEV_E *pDev)
{
	WV_U32 data,*pData; 
	pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN],value[WV_CONF_VALUE_MAX_LEN]; 

	//
	sprintf(name,"Venc_%d_viDev",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->viDev = data;  

	sprintf(name,"Venc_%d_viChn",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->viChn = data;  
	//viClkMode
	sprintf(name,"Venc_%d_viMode",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->viClkMode = data;  
	//vpssGrp
	sprintf(name,"Venc_%d_vpss",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->vpssGrp = data;  

	//vencChn
	sprintf(name,"Venc_%d_vEnc",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->vencChn = data; 
	//encBitRate
	sprintf(name,"Venc_%d_bRate",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->encBitRate = data; 
	// ip
	sprintf(name,"Venc_%d_ip",chn); 
	SYS_ENV_Get(name,value);
	pDev-> txSock.addrTx.sin_addr.s_addr = inet_addr(value); 
	bzero(&(pDev-> txSock.addrTx.sin_zero),8);
	
	
	//port
	sprintf(name,"Venc_%d_port",chn); 
	SYS_ENV_GetU32(name,pData); 
	pDev-> txSock.addrTx.sin_port = htons(data); 
	pDev-> txSock.addrTx.sin_family = AF_INET; 
	//vPid
	sprintf(name,"Venc_%d_pid",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->vPid = data; 
	//local IP IP_Ip
	sprintf(name,"IP_Ip"); 
	SYS_ENV_Get(name,value);
	pDev-> txSock.addrLocal.sin_addr.s_addr = inet_addr(value); 
	bzero(&(pDev-> txSock.addrLocal.sin_zero),8);
	pDev-> txSock.addrLocal.sin_port = htons(7001); 
	pDev-> txSock.addrLocal.sin_family = AF_INET; 
	
	
	
	//viWidth
	sprintf(name,"Venc_%d_width",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->viWidth = data; 
	//viHeight
	sprintf(name,"Venc_%d_height",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->viHeight = data; 
	//viFrameRate
	sprintf(name,"Venc_%d_frt",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->viFrameRate = data; 
   //vencFratio
	sprintf(name,"Venc_%d_fRatio",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->encFratio = data; 
	return  WV_SOK;
}


/*******************************************************************

WV_S32 TSK_VENC_SaveConf(WV_S32 chn,TSK_VENC_DEV_E *pDev);

*******************************************************************/
WV_S32 TSK_VENC_SaveConf(WV_S32 chn,TSK_VENC_DEV_E *pDev)
{
	WV_U32 data,*pData; 
	pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN];//value[WV_CONF_VALUE_MAX_LEN]; 

	//
	 sprintf(name,"Venc_%d_viDev",chn);  
	 data = pDev->viDev;  
	 SYS_ENV_SetU32(name,data);  

	sprintf(name,"Venc_%d_viChn",chn);  
	 data = pDev->viChn;  
	 SYS_ENV_SetU32(name,data);  
  
	//viClkMode
	sprintf(name,"Venc_%d_viMode",chn); 
	data = pDev->viClkMode;  
	SYS_ENV_SetU32(name,data); 
	//vpssGrp
	sprintf(name,"Venc_%d_vpss",chn);  
	data = pDev->vpssGrp;  
	SYS_ENV_SetU32(name,data);  

	//vencChn
	sprintf(name,"Venc_%d_vEnc",chn); 
	SYS_ENV_GetU32(name,pData);
	data = pDev->vencChn;  
	 SYS_ENV_SetU32(name,data); 
	//encBitRate
	sprintf(name,"Venc_%d_bRate",chn);  
	data = pDev->encBitRate;  
	 SYS_ENV_SetU32(name,data); 
	// ip
	sprintf(name,"Venc_%d_ip",chn);  
	 WV_S8 * pIp  = inet_ntoa(pDev-> txSock.addrTx.sin_addr);  
	 SYS_ENV_Set(name,pIp);
	//port
	sprintf(name,"Venc_%d_port",chn); 
	data =ntohs(pDev-> txSock.addrTx.sin_port);  
	SYS_ENV_SetU32(name,data);
	//vPid
	sprintf(name,"Venc_%d_pid",chn);  
	data = pDev->vPid;  
    SYS_ENV_SetU32(name,data);  
	//viWidth
	sprintf(name,"Venc_%d_width",chn);  
	data = pDev->viWidth;  
	//SYS_ENV_SetU32(name,data); 
	//viHeight
	sprintf(name,"Venc_%d_height",chn); 
	data = pDev->viHeight;  
	//SYS_ENV_SetU32(name,data); 
	//viFrameRate
	sprintf(name,"Venc_%d_frt",chn);  
	data = pDev->viFrameRate;  
	//SYS_ENV_SetU32(name,data);
	
	//
	   //vencFratio
	sprintf(name,"Venc_%d_fRatio",chn); 
	data = pDev->encFratio;  
	SYS_ENV_SetU32(name,data);
	
     SYS_ENV_Save();
	return  WV_SOK;
}
/*******************************************************************

WV_S32 TSK_VENC_GetVi(TSK_VENC_DEV_E  * pDev)

*******************************************************************/
WV_S32 TSK_VENC_GetVi(TSK_VENC_DEV_E  * pDev)
{
    WV_S32 ret = 0;
	WV_U8 chn = 0 ;
     //  get from vi or fpga
	 WV_U32 width, height,mode=0;
	 if(pDev->viChn == 0)
	 {
		chn = 0;
 	}else if(pDev->viChn == 8)
	{
		
		chn = 1;
	}
     ret = EXT_FPGA_GetResolution(chn, &width,&height,&mode);
     pDev->curWidth     = width;
     pDev->curHeight    = height;
	 pDev->curClkMode = mode;
     pDev->curFrameRate = 60;


      return ret;
}

/*******************************************************************

WV_S32 TSK_VENC_viChange(TSK_VENC_DEV_E  * pDev)

*******************************************************************/
WV_S32 TSK_VENC_viChange(TSK_VENC_DEV_E  * pDev)
{
      
    if(pDev->curWidth   == pDev->viWidth  &&  pDev->viHeight == pDev->curHeight   &&  pDev->curFrameRate ==  pDev->viFrameRate && pDev->curClkMode == pDev->viClkMode)   
    
    {
       return  WV_SOK;
    }  

	//printf("TSK_VENC_vi change w=%d,h=%d,curW=%d,curH=%d \n",pDev->viWidth,pDev->viHeight,pDev->curWidth,pDev->curHeight);  
    pDev->viWidth   = pDev->curWidth;
    pDev->viHeight   = pDev->curHeight;
	pDev->viClkMode  = pDev->curClkMode;
    pDev->curFrameRate =  pDev->curFrameRate;

   return -1;
}

/*******************************************************************

WV_S32 TSK_VENC_SetResolution(TSK_VENC_DEV_E  * pDev)

*******************************************************************/
WV_S32 TSK_VENC_SetResolution(TSK_VENC_DEV_E  * pDev)
{
 WV_S32 ret=0;
 //pDev-> bitRates = pDev->encBitRate * 1.0* pDev->viHeight*pDev->viWidth*pDev->viFrameRate/pDev-> encFratio/1920/1080/30;
 pDev-> bitRates = 10000;
 //printf("set chn[%d] viWidth=%d,,viHeight=%d \n",pDev->viChn,pDev->viWidth,pDev->viHeight);
ret |= HIS_VI_SetResolution(pDev->viChn,pDev->viDev,pDev->viWidth,pDev->viHeight,pDev->viClkMode);
ret |= HIS_VENC_SetResolution(pDev->vencChn,pDev->viWidth,pDev->viHeight,pDev->viFrameRate,pDev->viFrameRate/pDev->encFratio,pDev->bitRates );
 return ret;
}

/******************************************************************************

void *TSK_VENC_Proc(Void * prm);
******************************************************************************/
void * TSK_VENC_Proc ( void * prm)
{
 TSK_VENC_DEV_E  * pDev; 
 WV_U8  * pTemp;
 WV_S32 ret =0; 
 pDev = (TSK_VENC_DEV_E  *) prm;
 
 
 HIS_VENC_DevOpen(pDev);
 pDev-> open  = 1;
 pDev-> close  = 0; 
 

 while(pDev -> open == 1)
    { 

	  //check vi 
#if 1  
      ret = TSK_VENC_GetVi(pDev);
      if(ret != WV_SOK)	
      {
         continue;
      }
      //scheck  resolution
      ret = TSK_VENC_viChange(pDev);
      if(ret != WV_SOK)
      {
        ret = TSK_VENC_SetResolution(pDev);
        if(ret != WV_SOK)
        {
          usleep(10000);
          continue;  
        }
      }
#endif
     // get sterrame
      pTemp  =  pDev->pEsBuf + 16;
     ret = HIS_VENC_GetStream(pDev->vencChn, pTemp, & pDev->esLen, & pDev->esPts);  
     if(ret != WV_SOK)
     {
          usleep(1);
          continue; 
      }
       pTemp  =  pDev->pEsBuf;
       
      *(WV_U16*)(pTemp + 0) = pDev->viWidth;  //width
      *(WV_U16*)(pTemp + 2) = pDev->viHeight; //height;
      *(WV_U64*)(pTemp + 8) = pDev->esPts;    // pts
     pDev->esLen += 16; 
     ret = STRM_SendEs(&pDev->txSock,pDev->pEsBuf,pDev->esLen ,pDev->vPid,&pDev->pkgCnt);      
     pDev->esCnt ++;
     //usleep(1);
    } 
  HIS_VENC_DevClose(pDev);
 pDev-> open  = 0;
 pDev-> close = 1;
return NULL;  
} 


 
/****************************************************************************

WV_S32 TSK_VENC_SetSendIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port)

****************************************************************************/

WV_S32 TSK_VENC_SetSendIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port)
{
 
	gVencDev[chn].txSock.addrTx.sin_addr.s_addr = inet_addr(ipAddr); 
	bzero(&(gVencDev[chn].txSock.addrTx.sin_zero),8);	 
	gVencDev[chn].txSock.addrTx.sin_port = htons(port);
	TSK_VENC_SaveConf(chn,&gVencDev[chn]); 

	return WV_SOK;
}


/****************************************************************************

WV_S32 TSK_VENC_SetBitrate(WV_S32 chn, WV_U32 bitrate)

****************************************************************************/

WV_S32 TSK_VENC_SetBitrate(WV_S32 chn, WV_U32 bitrate)
{
 
	gVencDev[chn].encBitRate = bitrate; 
	TSK_VENC_SaveConf(chn,&gVencDev[chn]); 
	TSK_VENC_SetResolution(&gVencDev[chn]); 
 return WV_SOK;
} 
/****************************************************************************

WV_S32 TSK_VENC_SetFratio(WV_S32 chn, WV_U32 ratio)

****************************************************************************/

WV_S32 TSK_VENC_SetFratio(WV_S32 chn, WV_U32 ratio)
{
 
	gVencDev[chn].encFratio = ratio; 
	TSK_VENC_SaveConf(chn,&gVencDev[chn]); 
	TSK_VENC_SetResolution(&gVencDev[chn]); 
 return WV_SOK;
} 

 
/****************************************************************************

WV_S32 TSK_VENC_CMDStat(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_VENC_CMDStat(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

   WV_U32 chn;
   if( argc <1  )
	{ 
	    prfBuff += sprintf(prfBuff,"venc  get  <chn>  // get venc chn stat\r\n"); 
		return WV_SOK;  
    } 
    WV_STR_S2v(argv[0],&chn);
    
    if(chn > 1)
    {
     prfBuff += sprintf(prfBuff,"input  chn  erro! : %d \r\n",chn);  
     }
    
	prfBuff += sprintf(prfBuff,"venc chn : %d \r\n",chn); 

	prfBuff += sprintf(prfBuff,"vPid     	: %d \r\n",gVencDev[chn].vPid);
	prfBuff += sprintf(prfBuff,"viDev     	: %d \r\n",gVencDev[chn].viDev);
	prfBuff += sprintf(prfBuff,"vichn     	: %d \r\n",gVencDev[chn].viChn);
	prfBuff += sprintf(prfBuff,"viClkMode   : %d \r\n",gVencDev[chn].viClkMode);
	prfBuff += sprintf(prfBuff,"viWidth     : %d[%d] \r\n",gVencDev[chn].viWidth,gVencDev[chn].curWidth); 
	prfBuff += sprintf(prfBuff,"viHeight    : %d[%d] \r\n",gVencDev[chn].viHeight,gVencDev[chn].curHeight); 
	prfBuff += sprintf(prfBuff,"viFrameRate : %d[%d] \r\n",gVencDev[chn].viFrameRate,gVencDev[chn].curFrameRate);
	prfBuff += sprintf(prfBuff,"vpssGrp   	: %d \r\n",gVencDev[chn].vpssGrp);
	prfBuff += sprintf(prfBuff,"vencChn   	: %d \r\n",gVencDev[chn].vencChn);
	prfBuff += sprintf(prfBuff,"frameRatio	: %d \r\n",gVencDev[chn].encFratio);
	prfBuff += sprintf(prfBuff,"encBitRate  : %d[%d] \r\n",gVencDev[chn].encBitRate,gVencDev[chn].bitRates); 
	prfBuff += sprintf(prfBuff,"esLen  		: %d \r\n",gVencDev[chn].esLen);
	prfBuff += sprintf(prfBuff,"esPts  		: %lld \r\n",gVencDev[chn].esPts);
	prfBuff += sprintf(prfBuff,"esCnt  		: %d \r\n",gVencDev[chn].esCnt);
	prfBuff += sprintf(prfBuff,"pkgCnt  	: %d \r\n",gVencDev[chn].pkgCnt);
	prfBuff += sprintf(prfBuff,"erroCnt  	: %d \r\n",gVencDev[chn].erroCnt); 
 
  return WV_SOK;
}
  
/****************************************************************************

WV_S32 TSK_VENC_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_VENC_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
   WV_U32 chn;
   WV_U32 data; 
   if( argc < 1  )
	{ 
	    prfBuff += sprintf(prfBuff,"venc  set  <cmd>  cmd : ip /bitrate /frameRatio/rsl\r\n"); 
		return WV_SOK;  
    }
  //      set ip 
     
    if(strcmp(argv[0],"ip") ==0)
	{
		if(argc < 4)
		{ 
			prfBuff += sprintf(prfBuff,"set ip  chn  ipaddr  port\r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[3],&data); 
		WV_ASSERT_RET ( TSK_VENC_SetSendIp(chn,argv[2],data)); 
		return WV_SOK;  
	} 
   //  set bitrates
       
    
    if(strcmp(argv[0],"bitrate") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"set birate chn  bitrates\r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[2],&data); 
		WV_ASSERT_RET ( TSK_VENC_SetBitrate(chn,data)); 
		return WV_SOK;  
	}   
  //  set frame ratio
       
    
    if(strcmp(argv[0],"fratio") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"set fratio  chn  ratio\r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[2],&data); 
		WV_ASSERT_RET ( TSK_VENC_SetFratio(chn,data)); 
		return WV_SOK;  
	}


  if(strcmp(argv[0],"rsl") ==0)
	{
		WV_U32 w,h,mode;
		if(argc < 5)
		{ 
			prfBuff += sprintf(prfBuff,"set rsl  chn  w  h mode[0,1,2]\r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[2],&w);
		WV_STR_S2v(argv[3],&h);
		WV_STR_S2v(argv[4],&mode);
 		if(chn >=2)
	    {

			prfBuff += sprintf(prfBuff,"input chn error chn[%d]\r\n",chn); 
		}
		gVencDev[chn].viWidth = w;
		gVencDev[chn].viHeight = h;
		gVencDev[chn].viClkMode = mode;
		WV_ASSERT_RET ( TSK_VENC_SetResolution(&gVencDev[chn])); 
		return WV_SOK;  
	} 

	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
	        
 return WV_SOK;
}
/*****************************************************************************************************************

WV_S32  TSK_VENC_Init();

*****************************************************************************************************************/
WV_S32  TSK_VENC_Init()
{ 
   WV_S32 i;
   
   WV_CMD_Register("venc","get","get venc info",TSK_VENC_CMDStat);
   WV_CMD_Register("venc","set","set venc ",TSK_VENC_CMDSet); 
   
   for( i=0;i<2;i++)
   {
   
    TSK_VENC_RegisterConf(i);
    TSK_VENC_GetConf(i, &gVencDev[i]); 

    //  HIS_VENC_DevOpen(&gVencDev[i]);
     WV_THR_Create(&(gVencDev[i].thrHndl), TSK_VENC_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gVencDev[i]);
   } 
   return WV_SOK;
}
/*****************************************************************************************************************

WV_S32  TSK_VENC_DeInit();

*****************************************************************************************************************/
WV_S32  TSK_VENC_DeInit()
{ 
	WV_S32 i;
	for( i=0;i<2;i++)
	{

		if(gVencDev[i].open == 1)
			{
			gVencDev[i].open = 0;
			while(gVencDev[i].close == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gVencDev[i].thrHndl))   );	
			} 
          //HIS_VENC_DevClose(&gVencDev[i]);
		     
	} 
   return WV_SOK;
}

