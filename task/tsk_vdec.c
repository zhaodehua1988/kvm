#include"tsk_vdec.h"
#include"stream.h"
#include"his.h"
#include"mpi_vo.h"
#include"hi_comm_vo.h"



typedef struct TSK_VDEC_DEV_E 
{
	// 
	WV_THR_HNDL_T thrHndl; 
	WV_U32      open;
	WV_U32      close;  
	WV_U8       *pEsBuf;
	WV_U8       *pQueBuf;
	WV_QUE_DEV_E  queRx;
	WV_QUE_DEV_E  queFree;
	//
	WV_THR_HNDL_T vpssHndl; 
	WV_U32      vpssOpen;
	WV_U32      vpssClose;  
	//  config for video
	WV_U8       vPid;
	WV_S32      voDev;
	WV_U32      voWidth;
	WV_U32      voHeight;
	WV_U32      voFrameRate;
	WV_U32      curWidth;
	WV_U32      curHeight;
	WV_U32      curFrameRate;
	WV_S32      vpssGrp;
	WV_S32      vdecChn;

	// statute
	STRM_PACK_INFO_S  pkgInfo;
	STRM_ES_INFO_S   esInfo;
	WV_U32      esLen;
	WV_U64      esPts;
	WV_U32      esCnt;
	WV_U32      pkgCnt;
	WV_U32      erroCnt;

	WV_U32      delay; 


}TSK_VDEC_DEV_E;


#define  TSK_VDEC_BUF_LENMAX             2560*1600*2

#define  TSK_VDEC_QUE_LEN                1600
#define  TSK_VDEC_QUE_DEPTH              1000

static TSK_VDEC_DEV_E gVdecDev[2];

/*******************************************************************

WV_S32 TSK_VDEC_RegisterConf(WV_S32 chn);

*******************************************************************/
WV_S32 TSK_VDEC_RegisterConf(WV_S32 chn)
{
	WV_S8  name[WV_CONF_NAME_MAX_LEN],value[WV_CONF_VALUE_MAX_LEN]; 
	
	sprintf(name,"VDEC_%d_conf",chn); 
	SYS_ENV_Register(0,name,NULL,NULL);
	//
	sprintf(name,"VDEC_%d_voDev",chn); 
	sprintf(value, "%d",chn);
    SYS_ENV_Register(1 ,name,value,"voDev num");
    
    //
   	sprintf(name,"VDEC_%d_vpss",chn); 
	sprintf(value, "%d",chn+2); 
    SYS_ENV_Register(1 ,name,value,"vpssGrp");
    
        //
   	sprintf(name,"VDEC_%d_vDec",chn); 
	sprintf(value, "%d",chn); 
    SYS_ENV_Register(1 ,name,value,"vdec chn");
    
              //
   	sprintf(name,"VDEC_%d_pid",chn); 
	sprintf(value, "%d",12+chn); 
    SYS_ENV_Register(1 ,name,value,"pid"); 
   
     //
   	sprintf(name,"VDEC_%d_width",chn); 
	sprintf(value, "%d",1920); 
    SYS_ENV_Register(1 ,name,value,"video width"); 
     //
   	sprintf(name,"VDEC_%d_height",chn); 
	sprintf(value, "%d",1080); 
    SYS_ENV_Register(1 ,name,value,"video height"); 
     //
   	sprintf(name,"VDEC_%d_frt",chn); 
	sprintf(value, "%d",60); 
    SYS_ENV_Register(1 ,name,value,"video FrameRate"); 
    
     
    return WV_SOK;  
       
}




/*******************************************************************

WV_S32 TSK_VDEC_GetConf(WV_S32 chn,TSK_VDEC_DEV_E *pDev);

*******************************************************************/
WV_S32 TSK_VDEC_GetConf(WV_S32 chn,TSK_VDEC_DEV_E *pDev)
{
	WV_U32 data,*pData; 
	pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN];//,value[WV_CONF_VALUE_MAX_LEN]; 
 
   //
	sprintf(name,"VDEC_%d_voDev",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->voDev = data;  
    
    //
   	sprintf(name,"VDEC_%d_vpss",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->vpssGrp = data; 
    
        //
   	sprintf(name,"VDEC_%d_vDec",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->vdecChn = data; ;
    
              //
   	sprintf(name,"VDEC_%d_pid",chn); 
	 SYS_ENV_GetU32(name,pData);
	pDev->vPid = data; ; 
   
     //
   	sprintf(name,"VDEC_%d_width",chn); 
	 SYS_ENV_GetU32(name,pData);
	pDev->voWidth = data;  
     //
   	sprintf(name,"VDEC_%d_height",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->voHeight = data;  
     //
   	sprintf(name,"VDEC_%d_frt",chn); 
	SYS_ENV_GetU32(name,pData);
	pDev->voFrameRate = data;  
    
	return  WV_SOK;
}

/*******************************************************************

WV_S32 TSK_VDEC_SaveConf(WV_S32 chn,TSK_VDEC_DEV_E *pDev);

*******************************************************************/
WV_S32 TSK_VDEC_SaveConf(WV_S32 chn,TSK_VDEC_DEV_E *pDev)
{
	WV_U32 data;//,*pData; 
	//pData = & data;
	WV_S8  name[WV_CONF_NAME_MAX_LEN];//value[WV_CONF_VALUE_MAX_LEN]; 

	 //
	sprintf(name,"VDEC_%d_voDev",chn);  
	data = pDev->voDev;  
	 SYS_ENV_SetU32(name,data);   
    
    //
   	sprintf(name,"VDEC_%d_vpss",chn); 
 	data = pDev->vpssGrp;  
	SYS_ENV_SetU32(name,data);   
    
   //
   	sprintf(name,"VDEC_%d_vDec",chn); 
    data = pDev->vdecChn;  
	SYS_ENV_SetU32(name,data);
   //
   	sprintf(name,"VDEC_%d_pid",chn); 
 
	data = pDev->vPid;   
    SYS_ENV_SetU32(name,data); 
   //
   	sprintf(name,"VDEC_%d_width",chn); 
 
	data = pDev->voWidth;   
    //SYS_ENV_SetU32(name,data);   
     //
   	sprintf(name,"VDEC_%d_height",chn); 
 
	data = pDev->voHeight;   
    //SYS_ENV_SetU32(name,data); 
     //
   	sprintf(name,"VDEC_%d_frt",chn); 
 
	data = pDev->voFrameRate;   
   // SYS_ENV_SetU32(name,data); 
    
    //
     SYS_ENV_Save();
	return  WV_SOK;
}



/**********************************************************************************

HIS_VDEC_DevOpen(TSK_VDEC_DEV_E * pDev );

**********************************************************************************/

WV_S32  HIS_VDEC_DevOpen(TSK_VDEC_DEV_E *pDev )
{
    
 	WV_CHECK_RET( HIS_VO_Init(pDev-> voDev,pDev->voWidth,pDev->voHeight,pDev->voFrameRate)  );
 	
 	WV_CHECK_RET(  HIS_VPSS_Init(pDev->vpssGrp,1,pDev->voWidth,pDev->voHeight,0)  );
 	//WV_CHECK_RET(  HIS_VDEC_Init(pDev->vdecChn,2560,1600) );
    WV_CHECK_RET(  HIS_VDEC_Init(pDev->vdecChn,pDev->voWidth,pDev->voHeight) );
    if(pDev->voDev == 0)
      {
 			WV_CHECK_RET(  HIS_SYS_VO_BindVpss(8,0,pDev->vpssGrp,0) ); 
        
 	  }
 	  
 	  else
 	  {
 	      WV_CHECK_RET(  HIS_SYS_VO_BindVpss(pDev->voDev,0,pDev->vpssGrp,0) );
 	  }		
 
 	WV_CHECK_RET(  HIS_SYS_VDEC_BindVpss(pDev->vdecChn,pDev->vpssGrp) ); 
 	
    pDev->pEsBuf =  (WV_U8 *)malloc(TSK_VDEC_BUF_LENMAX);
   
    pDev->pQueBuf = (WV_U8 *)malloc(TSK_VDEC_QUE_LEN *TSK_VDEC_QUE_DEPTH);  
    
   WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queRx),TSK_VDEC_QUE_DEPTH)  );  
   WV_CHECK_RET(   WV_QUE_Create(  &(pDev->queFree),TSK_VDEC_QUE_DEPTH)  ); 
   
   WV_S32 i;
   WV_U32  buf;
   
   buf = (WV_U32) (pDev->pQueBuf);
   for(i=0;i< TSK_VDEC_QUE_DEPTH; i++)   
   {
     WV_CHECK_RET(  WV_QUE_Put(&(pDev->queFree),buf+ TSK_VDEC_QUE_LEN*i ,0) );
   }
      
 return WV_SOK;	
}


/**********************************************************************************

HIS_VDEC_DevClose(TSK_VDEC_DEV_E * pDev );

**********************************************************************************/

WV_S32  HIS_VDEC_DevClose(TSK_VDEC_DEV_E *pDev )
{
    WV_CHECK_RET(  HIS_SYS_VDEC_UnBindVpss(pDev->vdecChn,pDev->vpssGrp) );
    WV_CHECK_RET(  HIS_SYS_VO_UnBindVpss(pDev->voDev,0,pDev->vpssGrp,0) );
    WV_CHECK_RET(  HIS_VDEC_DeInit(pDev->vdecChn) );
    WV_CHECK_RET(  HIS_VPSS_DeInit(pDev->vpssGrp,1)  );
    WV_CHECK_RET(  HIS_VO_DeInit(pDev-> voDev)  );
    
   WV_CHECK_RET(  WV_QUE_Destroy( &(pDev->queRx) )   );  
   WV_CHECK_RET(  WV_QUE_Destroy( &(pDev->queFree) )  ); 
 	free(pDev->pEsBuf);
 	free(pDev->pQueBuf);
 	
 return WV_SOK;
}


/**********************************************************************************

void *TSK_VDEC_VpssProc(Void * prm)

 **********************************************************************************/
 
 void *TSK_VDEC_VpssProc(void * prm)
 {
 
 TSK_VDEC_DEV_E  * pDev; 
 
 WV_S32 ret; 
 VIDEO_FRAME_INFO_S stVideoFrame;
 
 
 pDev = (TSK_VDEC_DEV_E  *) prm;  
 
 
 pDev-> vpssOpen   = 1;
 pDev-> vpssClose  = 0;  
 WV_printf("vpss chn[%d] runing\r\n",pDev->vpssGrp); 
 while(pDev -> vpssOpen == 1)
    { 
      ret = HI_MPI_VPSS_GetChnFrame(pDev->vpssGrp,0,&stVideoFrame, 10);
       if(ret != WV_SOK)
      	{
      	   WV_ERROR("MPI_VPSS_GetChnFrame [%d] ret = %#x\r\n",pDev->vpssGrp,ret);
      	    continue;
		} 
	     if(pDev->voDev == 1) 
	     {
	        ret =  HI_MPI_VO_SendFrame(pDev->voDev,0,&stVideoFrame,0);
	     }
	       else
	     {
	       ret =  HI_MPI_VO_SendFrame(8,0,&stVideoFrame,0);
	     } 
	       
			if(ret != WV_SOK)
		  	{
		  	   // WV_ERROR("HI_MPI_VO_SendFrame[%d]  ret = %#x\r\n",pDev->voDev,ret);
		  	     
			}    
		   
	   
       ret =  HI_MPI_VPSS_ReleaseChnFrame (pDev->vpssGrp, 0,&stVideoFrame);
	        if(ret != WV_SOK)
      	{
      	   // WV_ERROR("MPI_VPSS_ReleaseChnFrame[%d]  ret = %#x\r\n",pDev->vpssGrp,ret);
      	     
		} 
    }  
 pDev-> vpssOpen  = 0;
 pDev-> vpssClose = 1;
 
 return NULL;
 }
 
 

/**********************************************************************************

void *TSK_VDEC_Proc(Void * prm)

 **********************************************************************************/
 
 void *TSK_VDEC_Proc(void * prm)
 {
 
 TSK_VDEC_DEV_E  * pDev; 
 WV_U64  stampVo,stampCur;
 VDEC_CHN_STAT_S devStat;
 WV_S32 ret =0; 
 WV_U8 * pPkgBuf;
 WV_U32  esLen;
 WV_U8 * pEsBuf;

 
 pDev = (TSK_VDEC_DEV_E  *) prm; 
  HIS_VDEC_DevOpen(pDev);
 pDev-> open  = 1;
 pDev-> close  = 0;  
 
 pEsBuf = pDev->pEsBuf;
 esLen  = 0;
 while(pDev -> open == 1)
    { 
   
	   //check get Bbuf 
      
      ret = WV_QUE_Get(&pDev-> queRx,(WV_U32*)&pPkgBuf,1);
      if(ret != WV_SOK)
      	{
      	    usleep(1);
      	    continue;
		} 
	 //  proc udp pkg
	  STRM_PackAnlyse(pPkgBuf,& pDev->pkgInfo);
	  
	  if(pDev->pkgInfo.sync != 1  && pDev->pkgInfo.pid !=  pDev->vPid  )
	     {
	       continue; 
	     } 
	  memcpy(pEsBuf,pDev->pkgInfo.pLoad,pDev->pkgInfo.loadLen);
	  esLen += pDev->pkgInfo.loadLen;
	  pEsBuf += pDev->pkgInfo.loadLen;
	  pDev->pkgCnt  ++;
	  ret = WV_QUE_Put(&pDev-> queFree,(WV_U32)pPkgBuf,0);
	   if(ret != WV_SOK)
      	{
      	    WV_ERROR("relese que buf error \r\n");
      	    continue;
		} 
	  //   proc es pkg 
	  if(pDev->pkgInfo.endFlag == 1)
	   {
	    // STRM_EsAnlyse( pDev->pEsBuf + 16 ,&pDev->esInfo,esLen);  // 16 add width height and pts 
	     
	   //  if(pDev->esInfo.sync == 1)  //send to dec
	    // {
				
				//ret =  HIS_VDEC_SendSream(pDev->vdecChn, pDev->esInfo.pLoad,pDev->esInfo.loadLen,*((WV_U64*)(pDev->pEsBuf +8)) ); 
				ret =  HIS_VDEC_SendSream(pDev->vdecChn, pDev->pEsBuf + 16,esLen -16,*((WV_U64*)(pDev->pEsBuf +8)) ); 
				if (ret != WV_SOK)
				{
				   pDev->erroCnt ++; 
				} 
				
			
			
			if(pDev->voDev == 0)
			 {
		       HI_MPI_VO_GetChnPts(8,0, &stampVo);
		     }	
		     else
		     {
		        HI_MPI_VO_GetChnPts(1,0, &stampVo);
		     }	
		    HI_MPI_SYS_GetCurPts(&stampCur);	 
		    pDev-> delay  =  stampCur - stampVo;
		    
		    HI_MPI_VDEC_Query(pDev->vdecChn,&devStat);
		    
		    
		     
	    //}
		//clr es buf
		pDev->esCnt  ++;
		pEsBuf = pDev->pEsBuf;
		esLen  = 0;
		//usleep(1); 
	   } 
	  
	   
	  
	  	
    } 
 HIS_VDEC_DevClose(pDev);
 pDev-> open  = 0;
 pDev-> close = 1;
 
 return NULL;
 }
 
 
 /****************************************************************************

WV_S32 TSK_VDEC_GetBuf(WV_S32 chn,WV_U32 * pBuf)

****************************************************************************/
WV_S32 TSK_VDEC_GetBuf(WV_S32 chn,WV_U32 * pBuf)
{
   WV_S32 ret;
     ret = WV_QUE_Get(&(gVdecDev[chn].queFree),pBuf,0);
     return ret;
}


 /****************************************************************************

WV_S32 TSK_VDE_PutBuf(WV_S32 chn ,WV_U32  pBuf )

****************************************************************************/
WV_S32 TSK_VDEC_PutBuf(WV_S32 chn ,WV_U32  pBuf )
{
   WV_S32 ret;
   ret = WV_QUE_Put(&(gVdecDev[chn].queRx),pBuf,0);
   return ret;  
} 


/****************************************************************************

WV_S32 TSK_VDEC_SetVoResolution(WV_S32 chn ,WV_U32 width,WV_U32 height,WV_U32 frameRate )

****************************************************************************/
WV_S32 TSK_VDE_SetVoResolution(WV_S32 chn ,WV_U32 width,WV_U32 height,WV_U32 frameRate )
{
   gVdecDev[chn].voWidth = width;
   gVdecDev[chn].voHeight = height;
   gVdecDev[chn].voFrameRate = frameRate;
   
     WV_CHECK_RET(  HIS_SYS_VO_UnBindVpss(gVdecDev[chn].voDev,0,gVdecDev[chn].vpssGrp,0) );
     WV_CHECK_RET(  HIS_VO_DeInit(gVdecDev[chn].voDev)  );
     
     WV_CHECK_RET(  HIS_VO_Init(gVdecDev[chn].voDev,gVdecDev[chn].voWidth,gVdecDev[chn].voHeight,gVdecDev[chn].voFrameRate)  );
    WV_CHECK_RET(  HIS_SYS_VO_BindVpss(gVdecDev[chn].voDev,0,gVdecDev[chn].vpssGrp,0) );
    
   return WV_SOK;
}
 
 
  
/****************************************************************************

WV_S32 TSK_VDEC_CMDStat(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_VDEC_CMDStat(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

   WV_U32 chn;
   if( argc <1  )
	{ 
	    prfBuff += sprintf(prfBuff,"vsdec  stat  <chn>  // get vdec chn stat\r\n"); 
		return WV_SOK;  
    } 
    WV_STR_S2v(argv[0],&chn);
    
    if(chn > 1)
    {
     prfBuff += sprintf(prfBuff,"input  chn  erro! : %d \r\n",chn);  
    }
    
	prfBuff += sprintf(prfBuff,"vde chn : %d \r\n",chn); 

	prfBuff += sprintf(prfBuff,"vPid     	: %d \r\n",gVdecDev[chn].vPid);
	prfBuff += sprintf(prfBuff,"voDev     	: %d \r\n",gVdecDev[chn].voDev);
	prfBuff += sprintf(prfBuff,"voWidth     : %d \r\n",gVdecDev[chn].voWidth);
	prfBuff += sprintf(prfBuff,"voHeight    : %d \r\n",gVdecDev[chn].voHeight);
	prfBuff += sprintf(prfBuff,"voFrameRate : %d \r\n",gVdecDev[chn].voFrameRate);
	prfBuff += sprintf(prfBuff,"vpssGrp    	: %d \r\n",gVdecDev[chn].vpssGrp);
	prfBuff += sprintf(prfBuff,"vdecChn    	: %d \r\n",gVdecDev[chn].vdecChn);
	prfBuff += sprintf(prfBuff,"esCnt    	: %d \r\n",gVdecDev[chn].esCnt);
	prfBuff += sprintf(prfBuff,"pkgCnt    	: %d \r\n",gVdecDev[chn].pkgCnt);
	prfBuff += sprintf(prfBuff,"erroCnt    	: %d \r\n",gVdecDev[chn].erroCnt);
	prfBuff += sprintf(prfBuff,"delay    	: %d \r\n",gVdecDev[chn].delay);
    return WV_SOK;
}


 
/****************************************************************************

WV_S32 TSK_VDEC_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 TSK_VDE_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{

    WV_U32 chn;
   WV_U32 width,height,frameRate; 
   if( argc < 2  )
	{ 
	    prfBuff += sprintf(prfBuff,"venc  set  <cmd>  cmd : vo\r\n"); 
		return WV_SOK;  
    }
  //      set ip 
     
    if(strcmp(argv[0],"vo") ==0)
	{
		if(argc < 5)
		{ 
			prfBuff += sprintf(prfBuff,"set vo chn  width height frameRate\r\n");  
			return WV_SOK; 
		}
		WV_STR_S2v(argv[1],&chn); 
		WV_STR_S2v(argv[2],&width);
		WV_STR_S2v(argv[3],&height);
		WV_STR_S2v(argv[4],&frameRate); 
		TSK_VDE_SetVoResolution(chn ,width,height,frameRate); 
		return WV_SOK;  
	} 
   
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
	        
 return WV_SOK;
   
}


/*****************************************************************************************************************

WV_S32  TSK_VDEC_Init();

*****************************************************************************************************************/
WV_S32  TSK_VDEC_Init()
{ 
   WV_S32 i;
   
   WV_CMD_Register("vdec","get","get venc info",TSK_VDEC_CMDStat);
   WV_CMD_Register("vdec","set","set vdec ",TSK_VDE_CMDSet); 
   HIS_VDEC_InitMode();
   for( i=0;i<2;i++)
   {
    
    TSK_VDEC_RegisterConf(i);
    TSK_VDEC_GetConf(i, &gVdecDev[i]); 
    
    //HIS_VDEC_DevOpen(&gVdecDev[i]);
    WV_THR_Create(&(gVdecDev[i].thrHndl), TSK_VDEC_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gVdecDev[i]);
   // WV_THR_Create(&(gVdecDev[i].vpssHndl), TSK_VDEC_VpssProc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gVdecDev[i]);
    
   } 
   return WV_SOK;
}


/*****************************************************************************************************************

WV_S32  TSK_VDEC_DeInit();

*****************************************************************************************************************/
WV_S32  TSK_VDEC_DeInit()
{ 
	WV_S32 i;
	for( i=0;i<2;i++)
	{
       
        if(gVdecDev[i].vpssOpen == 1)
			{
			gVdecDev[i].vpssOpen = 0;
			while(gVdecDev[i].vpssClose == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gVdecDev[i].vpssHndl))   );	
			} 
 
       
		if(gVdecDev[i].open == 1)
			{
			gVdecDev[i].open = 0;
			while(gVdecDev[i].close == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gVdecDev[i].thrHndl))   );	
			} 
         // HIS_VDEC_DevClose(&gVdecDev[i]);
		     
	} 
   return WV_SOK;
}



  