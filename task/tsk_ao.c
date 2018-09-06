#include"tsk_ao.h" 
#include"his_aio.h"
#include"stream.h"

#define TSK_AO_BUF_LEN       2*1024

#define  TSK_AO_QUE_LEN                1600
#define  TSK_AO_QUE_DEPTH              100
typedef struct TSK_AO_DEV_E 
{
// 
 WV_THR_HNDL_T 		thrHndl; 
 WV_U32      		open;
 WV_U32      		close;  
 WV_U8       		*pBuf;
 WV_U8       		*pQueBuf;
 
 WV_QUE_DEV_E  		queRx;
 WV_QUE_DEV_E  		queFree;  
 //  config for video
  WV_U8       		aPid;
  WV_S32      		aoDev;
  // statute
  STRM_PACK_INFO_S  pkgInfo;
  WV_U32      		aoLen;
  WV_U64      		aoPts;
  WV_U32      		aoCnt; 
   WV_U32      		pkgCnt;
  WV_U32      		erroCnt;
  
}TSK_AO_DEV_E;

static  TSK_AO_DEV_E  gAoDev[2];



/*******************************************************************

WV_S32 TSK_AO_RegisterConf(WV_S32 chn);

*******************************************************************/
WV_S32 TSK_AO_RegisterConf(WV_S32 chn)
{
	WV_S8  name[WV_CONF_NAME_MAX_LEN],value[WV_CONF_VALUE_MAX_LEN]; 
	
	sprintf(name,"Ao_%d_conf",chn); 
	SYS_ENV_Register(0,name,NULL,NULL);
	//
	sprintf(name,"Ao_%d_aoDev",chn); 
	sprintf(value, "%d",chn);
    SYS_ENV_Register(1 ,name,value,"ao Dev num");  
    //
   	sprintf(name,"Ao_%d_pid",chn); 
	sprintf(value, "%d",12+chn); 
    SYS_ENV_Register(1 ,name,value,"AO recive pid"); 
   
    return WV_SOK;  
       
}


/*******************************************************************

WV_S32 TSK_AO_GetConf(WV_S32 chn,TSK_AO_DEV_E *pDev);

*******************************************************************/
WV_S32 TSK_AO_GetConf(WV_S32 chn,TSK_AO_DEV_E *pDev)
{
	WV_U32 data,*pData; 
	pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN];//,value[WV_CONF_VALUE_MAX_LEN]; 

	//
	sprintf(name,"Ao_%d_aoDev",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->aoDev = data;   
	//vPid
	sprintf(name,"Ai_%d_pid",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->aPid = data;  
	
	return  WV_SOK;
}




/*******************************************************************

WV_S32 TSK_AO_SaveConf(WV_S32 chn,TSK_AO_DEV_E *pDev);

*******************************************************************/
WV_S32 TSK_AO_SaveConf(WV_S32 chn,TSK_AO_DEV_E *pDev)
{
	WV_U32 data;//*pData; 
	//pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN];//,value[WV_CONF_VALUE_MAX_LEN]; 

	// aiDev
	 sprintf(name,"Ao_%d_aoDev",chn);  
	 data = pDev->aoDev;  
	 SYS_ENV_SetU32(name,data);    
 
	 
	//vPid
	sprintf(name,"Ao_%d_pid",chn); 
    data = pDev->aPid;  
	SYS_ENV_SetU32(name,data);   
	
	return  WV_SOK;
}




/**********************************************************************************

WV_S32  HIS_AO_DevOpen(TSK_AO_DEV_E * pdev );

**********************************************************************************/

WV_S32  HIS_AO_DevOpen(TSK_AO_DEV_E *pDev )
{
   
   	WV_CHECK_RET(   HIS_AO_Init(pDev->aoDev) ); 
 	pDev->pBuf  =  (WV_U8 *)malloc( TSK_AO_BUF_LEN ); 
 	// 
	pDev->pQueBuf = (WV_U8 *)malloc(TSK_AO_QUE_LEN *TSK_AO_QUE_DEPTH);   
	WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queRx),TSK_AO_QUE_DEPTH)  );  
	WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queFree),TSK_AO_QUE_DEPTH)  ); 

	WV_S32 i;
	WV_U32  buf;

	buf = (WV_U32) (pDev->pQueBuf);
	for(i=0;i< TSK_AO_QUE_DEPTH; i++)   
	{
		WV_CHECK_RET(  WV_QUE_Put(&(pDev->queFree),buf+ TSK_AO_QUE_LEN*i ,0) );
	} 
 	return WV_SOK;	
}

 
/**********************************************************************************

WV_S32  HIS_AO_DevClose(TSK_AO_DEV_E * pdev );

**********************************************************************************/

WV_S32  HIS_AO_DevClose(TSK_AO_DEV_E *pDev )
{
   
   	WV_CHECK_RET(   HIS_AO_DeInit(pDev->aoDev) ); 
 	WV_CHECK_RET(  WV_QUE_Destroy( &(pDev->queRx) )   );  
    WV_CHECK_RET(  WV_QUE_Destroy( &(pDev->queFree) )  ); 
 	free(pDev->pQueBuf);
 	free(pDev->pBuf);
 	return WV_SOK;	
}

/******************************************************************************

void *TSK_AO_Proc(Void * prm);
******************************************************************************/
void * TSK_AO_Proc ( void * prm)
{
 TSK_AO_DEV_E  * pDev; 
 WV_U8  * pTemp;
 WV_U32  rxLen;
 WV_U8 * pRxBuf;
 WV_S32 ret =0; 
 pDev = (TSK_AO_DEV_E  *) prm; 
 
 HIS_AO_DevOpen(pDev);
 
 
 pRxBuf = pDev->pBuf;
 rxLen  = 0;
 pDev-> open  = 1;
 pDev-> close  = 0; 
 while(pDev -> open == 1)
    { 
     
     // get buf 
      ret = WV_QUE_Get(&pDev-> queRx,(WV_U32*)&pTemp,0);
      if(ret != WV_SOK)
      	{
      	    usleep(1);
      	    continue;
		} 
      //proc pkg
       STRM_PackAnlyse(pTemp,& pDev->pkgInfo);
	  
	  if(pDev->pkgInfo.sync != 1  && pDev->pkgInfo.pid !=  pDev->aPid  )
	     {
	       continue; 
	     } 
	  memcpy(pRxBuf,pDev->pkgInfo.pLoad,pDev->pkgInfo.loadLen);
	  rxLen += pDev->pkgInfo.loadLen;
	  pRxBuf += pDev->pkgInfo.loadLen;
	  pDev->pkgCnt  ++;
	  ret = WV_QUE_Put(&pDev-> queFree,(WV_U32)pTemp,0);
		if(ret != WV_SOK)
		{
			WV_ERROR("relese que buf error \r\n");
			continue;
		} 

    // proc frame '
	if(pDev->pkgInfo.endFlag == 1)
	{
	    ret = HIS_AO_SendFrame(pDev-> aoDev ,pDev->pBuf +16 , rxLen ,*((WV_U64*)(pDev->pBuf + 8))); 
		if(ret != WV_SOK)
		{
		   pDev->erroCnt ++; 
		}  
		pDev->aoLen =  rxLen;
		pDev->aoPts = *((WV_U64*)(pDev->pBuf + 8));
		
		//clr rxbuf
		pDev->aoCnt  ++;
		pRxBuf = pDev->pBuf;
		rxLen  = 0;
		
    } 
     pDev->pkgCnt ++;
      
    } 
 HIS_AO_DevClose(pDev);
 pDev-> open  = 0;
 pDev-> close = 1;
return NULL;  
} 



 
/****************************************************************************

WV_S32 TSK_AO_GetBuf(WV_S32 chn,WV_U32 * pBuf)

****************************************************************************/
WV_S32 TSK_AO_GetBuf(WV_S32 chn,WV_U32 *pBuf)
{
    WV_S32 ret;
     ret = WV_QUE_Get(&(gAoDev[chn].queFree),pBuf,0);
     return ret;
}


 /****************************************************************************

WV_S32 TSK_AO_PutBuf(WV_S32 chn ,WV_U32  pBuf )

****************************************************************************/
WV_S32 TSK_AO_PutBuf(WV_S32 chn ,WV_U32  pBuf )
{
   WV_S32 ret;
   ret = WV_QUE_Put(&(gAoDev[chn].queRx),pBuf,0);
   return ret;  
} 




/****************************************************************************

WV_S32 TSK_AO_CMDStat(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_AO_CMDStat(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

   WV_U32 chn;
   if( argc <1  )
	{ 
	    prfBuff += sprintf(prfBuff,"ao  stat  <chn>  // get ai chn stat\r\n"); 
		return WV_SOK;  
    } 
    WV_STR_S2v(argv[0],&chn);
    
    if(chn > 1)
    {
     prfBuff += sprintf(prfBuff,"ao  chn  erro! : %d \r\n",chn);  
     }
    
	prfBuff += sprintf(prfBuff,"ao chn : %d \r\n",chn);  
	prfBuff += sprintf(prfBuff,"aPid     	: %d \r\n",gAoDev[chn].aPid);
	prfBuff += sprintf(prfBuff,"aoDev     	: %d \r\n",gAoDev[chn].aoDev);  
	prfBuff += sprintf(prfBuff,"aoLen  		: %d \r\n",gAoDev[chn].aoLen);
	prfBuff += sprintf(prfBuff,"aoPts  		: %lld \r\n",gAoDev[chn].aoPts);
	prfBuff += sprintf(prfBuff,"aoCnt  		: %d \r\n",gAoDev[chn].aoCnt);
	prfBuff += sprintf(prfBuff,"pkgCnt  	: %d \r\n",gAoDev[chn].pkgCnt);
	prfBuff += sprintf(prfBuff,"erroCnt  	: %d \r\n",gAoDev[chn].erroCnt); 
  
  return WV_SOK;
}
  

/****************************************************************************

WV_S32 TSK_AO_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_AO_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
   WV_U32 chn;
   WV_U32 data; 
   if( argc < 2  )
	{ 
	    prfBuff += sprintf(prfBuff,"ai  set  <cmd> . .. //  cmd : pid \r\n"); 
		return WV_SOK;  
    }
  //      set ip 
     
    if(strcmp(argv[0],"pid") ==0)
	{
		if(argc < 3)
		{ 
			prfBuff += sprintf(prfBuff,"ai  set  pid  <chn> <pid> \r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[2],&data);  
		gAoDev[chn].aPid = data;		
		TSK_AO_SaveConf(chn,&gAoDev[chn]);
		return WV_SOK;  
	} 
    
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
	        
 return WV_SOK;
}
 
 

/*****************************************************************************************************************

WV_S32  TSK_AO_Init();

*****************************************************************************************************************/
WV_S32  TSK_AO_Init()
{ 
   WV_S32 i;
   
   WV_CMD_Register("ao","sata","get ao info",TSK_AO_CMDStat);
   WV_CMD_Register("ao","set","set ao ",TSK_AO_CMDSet); 
   
   for( i=0;i<2;i++)
   {
   
    TSK_AO_RegisterConf(i);
    TSK_AO_GetConf(i, &gAoDev[i]); 
    WV_THR_Create(&(gAoDev[i].thrHndl), TSK_AO_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gAoDev[i]);
   } 
   return WV_SOK;
}



/*****************************************************************************************************************

WV_S32  TSK_AO_DeInit();

*****************************************************************************************************************/
WV_S32  TSK_AO_DeInit()
{ 
   WV_S32 i;
	for( i=0;i<2;i++)
	{

		if(gAoDev[i].open == 1)
			{
			gAoDev[i].open = 0;
			while(gAoDev[i].close == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gAoDev[i].thrHndl))   );	
			} 

		     
	} 
   return WV_SOK;
}
 

