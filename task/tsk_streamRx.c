#include"tsk_streamRx.h"
#include"tsk_vdec.h"
#include"tsk_ao.h" 

typedef struct TSK_STREAM_RX_DEV_E 
{
// 
 WV_THR_HNDL_T thrHndl; 
 WV_U32      open;
 WV_U32      close; 
 WV_ETH_UDP_RX_E  rxSock;  
 // config for video
 WV_U8       pid[8];  
  // statute
 WV_U64       pkgPts[8];
 WV_U32       pkgCnt[8];
 WV_U32       erroCnt;  
}TSK_STREAM_RX_DEV_E;

static TSK_STREAM_RX_DEV_E  gSrxDev;

 WV_QUE_DEV_E     	gSrxQueRx; 
 WV_QUE_DEV_E     	gSrxQueFree; 
 WV_U8            * pSrxQueBuf;


/*******************************************************************

WV_S32 TSK_SRX_RegisterConf();

*******************************************************************/
WV_S32 TSK_SRX_RegisterConf()
{
	WV_S8  name[WV_CONF_NAME_MAX_LEN],value[WV_CONF_VALUE_MAX_LEN]; 
	WV_S32 i ;
	sprintf(name,"Stream Recive conf"); 
	SYS_ENV_Register(0,name,NULL,NULL);
	//
	sprintf(name,"Srx_ip"); 
	sprintf(value, "226.100.100.2"); 
    SYS_ENV_Register(1 ,name,value,"ip addr"); 
           //
   	sprintf(name,"Srx_port"); 
	sprintf(value, "%d",7001); 
    SYS_ENV_Register(1 ,name,value,"ip port");     
	//
	for(i=0;i<8;i++)
		{
		sprintf(name,"Srx_%d_pid",i); 
		sprintf(value, "%d",10+i);
		SYS_ENV_Register(1 ,name,value,"pid num");
		
		}
    return WV_SOK;  
       
}



/*******************************************************************

WV_S32 TSK_SRX_GetConf();

*******************************************************************/
WV_S32 TSK_SRX_GetConf(TSK_STREAM_RX_DEV_E *pDev)
{
    WV_S32 i;
	WV_U32 data,*pData; 
	pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN],value[WV_CONF_VALUE_MAX_LEN]; 
 
	// ip
	sprintf(name,"Srx_ip"); 
	SYS_ENV_Get(name,value);
	pDev-> rxSock.addrRx.sin_addr.s_addr = inet_addr(value); 
	bzero(&(pDev-> rxSock.addrRx.sin_zero),8);
	//port
	sprintf(name,"Srx_port"); 
	SYS_ENV_GetU32(name,pData); 
	pDev-> rxSock.addrRx.sin_port = htons(data);
	pDev-> rxSock.addrRx.sin_family = AF_INET;
 
	//local IP IP_Ip
	sprintf(name,"IP_Ip"); 
	SYS_ENV_Get(name,value);
	pDev-> rxSock.addrLocal.sin_addr.s_addr = inet_addr(value); 
	bzero(&(pDev-> rxSock.addrLocal.sin_zero),8);
	pDev-> rxSock.addrLocal.sin_port = htons(7001);
	pDev-> rxSock.addrLocal.sin_family = AF_INET; 
	
	for(i=0;i<8;i++)
		{
		   sprintf(name,"Srx_%d_pid",i); 
		   SYS_ENV_GetU32(name,pData); 
		   pDev->pid[i] = data; 
		 }
	return  WV_SOK;
}


/*******************************************************************

WV_S32 TSK_SRX_SaveConf(TSK_STREEAM_RX_DEV_E  *pDev);

*******************************************************************/
WV_S32 TSK_SRX_SaveConf(TSK_STREAM_RX_DEV_E *pDev)
{
	WV_S32 i;
	WV_U32 data;//,*pData; 
	//pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN];//,value[WV_CONF_VALUE_MAX_LEN]; 
 
	// ip
	 sprintf(name,"Srx_ip");   
	 WV_S8 * pIp  = inet_ntoa(pDev-> rxSock.addrRx.sin_addr);  
	 SYS_ENV_Set(name,pIp);
	//port
	sprintf(name,"Srx_port");
	data =ntohs(pDev-> rxSock.addrRx.sin_port);  
	SYS_ENV_SetU32(name,data);
	 
	 //PID 
	for(i=0;i<8;i++)
		{
		   sprintf(name,"Srx_%d_pid",i); 
		   data  = pDev->pid[i];
		   SYS_ENV_SetU32(name,data);
		 }
	 
     SYS_ENV_Save();
	return  WV_SOK;
}




/*********************************************************

WV_S32  TSK_SRX_SendToVdec(WV_S32  chn, WV_U8 *pBbuf,WV_S32 len)

*********************************************************/  

WV_S32  TSK_SRX_SendToVdec(WV_S32  chn, WV_U8 *pBbuf,WV_S32 len)
{
   WV_U8 * decBuf;
   
    TSK_VDEC_GetBuf(chn,(WV_U32 *)&decBuf);
    memcpy(decBuf,pBbuf,len);
    TSK_VDEC_PutBuf(chn,(WV_U32)decBuf); 
   return WV_SOK;
}



/*********************************************************

WV_S32  TSK_SRX_SendToAdec(WV_S32  chn, WV_U8 *pBbuf,WV_S32 len);

*********************************************************/  

WV_S32  TSK_SRX_SendToAdec(WV_S32  chn, WV_U8 *pBbuf,WV_S32 len)
{
   
    WV_U8 * pAoBuf;     
    TSK_AO_GetBuf(chn,(WV_U32 *)&pAoBuf);
     memcpy(pAoBuf,pBbuf,len);
    TSK_AO_PutBuf(chn,(WV_U32)pAoBuf); 
   
   return WV_SOK;
}

/*********************************************************

WV_S32  TSK_SRX_SendToKm(WV_U8 *Pbuf,WV_S32 len);

*********************************************************/  

WV_S32  TSK_SRX_SendToKm(WV_U8 *Pbuf,WV_S32 len)
{
   return WV_SOK;
}

/*********************************************************

WV_S32  TSK_SRX_TestInit(TSK_STREAM_RX_DEV_E  *pDev );

*********************************************************/  
WV_S32  TSK_SRX_TestInit()
{  
 
   pSrxQueBuf = (WV_U8*) malloc(2048 *1000);
   WV_CHECK_RET(   WV_QUE_Create(  &gSrxQueRx,1000)  );  
   WV_CHECK_RET(   WV_QUE_Create(  &gSrxQueFree,1000)  );   
     WV_S32 i;
      for(i =0;i<1000;i++){
  	  WV_CHECK_RET(  WV_QUE_Put(&gSrxQueFree, (WV_U32)(pSrxQueBuf+ 2048*i),0) );
  	}  
 
   return WV_SOK;
}
/*********************************************************

WV_S32  TSK_SRX_TestDeInit(TSK_STREAM_RX_DEV_E  *pDev );

*********************************************************/  

WV_S32  TSK_SRX_TestDeInit()
{  
  WV_CHECK_RET(   WV_QUE_Destroy( &gSrxQueRx  )  );  
  WV_CHECK_RET(   WV_QUE_Destroy( &gSrxQueFree)  );  
  free(pSrxQueBuf);
   return WV_SOK;
}



/*********************************************************

WV_S32  TSK_SRX_TestTx();

*********************************************************/  

WV_S32  TSK_SRX_TestTx(WV_U8 * pTxBuf,WV_U32 len)
{  
   
	WV_U8  *pQue; 
	WV_CHECK_RET( WV_QUE_Get(&gSrxQueFree,(WV_U32 *)(&pQue),1) ); 
	*((WV_U32 *)pQue) = len;
	memcpy(pQue + 4,pTxBuf,len);
	WV_CHECK_RET( WV_QUE_Put(&gSrxQueRx,(WV_U32)pQue,1) );
	return WV_SOK; 
}

/*********************************************************

WV_S32  TSK_SRX_TestRx();

*********************************************************/  

WV_S32  TSK_SRX_TestRx(WV_U8 * pRxBuf,WV_U32 *len)
{   
    WV_S32 ret; 
	WV_U8  *pQue; 
	//WV_printf("TSK_SRX_TestRx strat \r\n"); 
	ret = WV_QUE_Get(&gSrxQueRx,(WV_U32 *)(&pQue),1);
	//WV_printf("TSK_SRX_TestRx get  \r\n" ); 
	
	if(ret != WV_SOK)
		{
		return WV_EFAIL;
		}  
	*len = *((WV_U32 *)pQue);
	 memcpy(pRxBuf,pQue + 4,*len);
	 WV_CHECK_RET( WV_QUE_Put(&gSrxQueFree,(WV_U32)pQue,1) );
	 
	//WV_printf("TSK_SRX_TestRx %d \r\n",*len); 
	return WV_SOK; 
}
/******************************************************************************

void *TSK_SRX_Proc(Void * prm);
******************************************************************************/
void * TSK_SRX_Proc ( void * prm)
{
 TSK_STREAM_RX_DEV_E   * pDev;
 WV_S32  len; 
 WV_S32 ret =0;
 WV_U8 *pBuf,pid; 
 pDev = (TSK_STREAM_RX_DEV_E  *) prm;
 pBuf = (WV_U8 *)malloc(2048*2);
 WV_UDPRX_Creat(&pDev->rxSock);
 
 // 
 TSK_SRX_TestInit();// for test
 pDev-> open  = 1;
 pDev-> close  = 0; 
  
	while(pDev -> open == 1)
	{ 
	  //for test
	    
	    //ret = TSK_SRX_TestRx(pBuf,&len);
		ret = WV_UDPRX_Rx(&pDev->rxSock,pBuf,&len);
		if(ret != WV_SOK)
		{
		 continue;
		}
		if(len < 12 )
		{
		//WV_ERROR("SRX  pkg len erro  [%d]\r\n",len);
		continue;
		} 
		if(pBuf[0] != 0x47 )
		{
		WV_ERROR("SRX  sync erro  [%d]\r\n",pBuf[0]);
		continue;
		}  
		pid = pBuf[2];

		if(pid == pDev->pid[0])
			{ 

			TSK_SRX_SendToVdec(0,pBuf,len);
			//TSK_SRX_SendToVdec(1,pBuf,len);
			pDev-> pkgPts[0] = *((WV_U64*)(pBuf+4));
             pDev->pkgCnt[0] ++;
			}
		if(pid == pDev->pid[1])
			{

			TSK_SRX_SendToVdec(1,pBuf,len);
			 pDev-> pkgPts[1] = *((WV_U64*)(pBuf+4));
             pDev->pkgCnt[1] ++;
			}
		if(pid == pDev->pid[2])
			{ 
			TSK_SRX_SendToAdec(0,pBuf,len);
			pDev-> pkgPts[2] = *((WV_U64*)(pBuf+4));
             pDev->pkgCnt[2] ++;
			}	
		if(pid == pDev->pid[3])
			{ 
			TSK_SRX_SendToAdec(1,pBuf,len);
			pDev-> pkgPts[3] = *((WV_U64*)(pBuf+4));
             pDev->pkgCnt[3] ++;
			}	
		 if(pid == pDev->pid[4])
			{ 
			TSK_SRX_SendToKm(pBuf,len);
			pDev-> pkgPts[4] = *((WV_U64*)(pBuf+4));
             pDev->pkgCnt[4] ++;
			}					
			
	}
 WV_UDPRX_Destroy(&pDev->rxSock);	
 free(pBuf);
 //
  //TSK_SRX_TestDeInit();   
 pDev-> open  = 0;
 pDev-> close = 1;
return NULL;  
} 

 

 
/****************************************************************************

WV_S32 TSK_SRX_SetRxIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port)

****************************************************************************/

WV_S32 TSK_SRX_SetRxIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port)
{
 
	gSrxDev.rxSock.addrRx.sin_addr.s_addr = inet_addr(ipAddr); 
	bzero(&(gSrxDev.rxSock.addrRx.sin_zero),8);	 
	gSrxDev.rxSock.addrRx.sin_port = htons(port);
	TSK_SRX_SaveConf(&gSrxDev); 
	
 return WV_SOK;
}

/****************************************************************************

WV_S32 TSK_SRX_SetPid(WV_S32 chn, WV_U32 pid)

****************************************************************************/

WV_S32 TSK_SRX_SetPid(WV_S32 chn, WV_U32 pid)
{
 
	gSrxDev.pid[chn] = pid;  
	TSK_SRX_SaveConf(&gSrxDev); 
	
 return WV_SOK;
}




 
/****************************************************************************

WV_S32 TSK_SRX_CMDStat(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_SRX_CMDStat(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

     WV_S32 i;

	prfBuff += sprintf(prfBuff,"Stream  recive stat\r\n"); 
   for(i=0;i<8;i++)
	{
		prfBuff += sprintf(prfBuff,"************%d************** \r\n",i); 
		prfBuff += sprintf(prfBuff,"pid     		: %d \r\n",gSrxDev.pid[i]); 
		prfBuff += sprintf(prfBuff,"pkgPts	    	: %lld \r\n",gSrxDev.pkgPts[i]);
		prfBuff += sprintf(prfBuff,"pkgCnt	     	: %d \r\n",gSrxDev.pkgCnt[i]);
	 }
 
  return WV_SOK;
}

/****************************************************************************

WV_S32 TSK_SRX_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_SRX_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

   WV_U32 chn;
   WV_U32 data; 
   if( argc < 1  )
	{ 
	    prfBuff += sprintf(prfBuff,"srx  set  <cmd>  cmd : ip /pid\r\n"); 
		return WV_SOK;  
    }
  //      set ip 
     
    if(strcmp(argv[0],"ip") ==0)
	{
		if(argc < 4)
		{ 
			prfBuff += sprintf(prfBuff,"srx set ip  <ipaddr> <port> \r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[3],&data); 
		WV_ASSERT_RET ( TSK_SRX_SetRxIp(chn,argv[2],data)); 
		return WV_SOK;  
	} 
   //  set bitrates
       
    
    if(strcmp(argv[0],"pid") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"set birate chn  bitrates\r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[2],&data); 
		WV_ASSERT_RET ( TSK_SRX_SetPid(chn,data)); 
		return WV_SOK;  
	}   
 
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
  return WV_SOK;
}

 
/*****************************************************************************************************************

WV_S32  TSK_SRX_Init();

*****************************************************************************************************************/
WV_S32  TSK_SRX_Init()
{ 
 
    TSK_SRX_RegisterConf();
    TSK_SRX_GetConf(&gSrxDev); 
    WV_THR_Create(&(gSrxDev.thrHndl), TSK_SRX_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gSrxDev);
   
   return WV_SOK;
}




/*****************************************************************************************************************

WV_S32  TSK_SRX_DeInit();

*****************************************************************************************************************/
WV_S32  TSK_SRX_DeInit()
{ 
 
   	if(gSrxDev.open == 1)
			{
			gSrxDev.open = 0;
			while(gSrxDev.close == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gSrxDev.thrHndl)));	
			} 
   return WV_SOK;
}