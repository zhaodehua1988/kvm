#include"his_aio.h"
#include"hi_comm_aio.h"
#include"mpi_ai.h"
#include"mpi_ao.h"

/*****************************************************************

WV_S32  HIS_AI_Init(WV_S32  devAi);

*****************************************************************/
WV_S32  HIS_AI_Init(WV_S32  devAi)
{
	WV_S32 s32Ret,i;
	AUDIO_DEV AioDevId;  // i 1 2  o:  0 1  0 2
	AIO_ATTR_S stAioAttr;
	
    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_48000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_SLAVE;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_STEREO;//STEREO  MONO
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = 320;//640  1280?
    stAioAttr.u32ChnCnt      = 1;//?
    stAioAttr.u32ClkChnCnt   = 2;//?
    stAioAttr.u32ClkSel      = 0;  
    
    AioDevId = devAi;
    s32Ret = HI_MPI_AI_SetPubAttr(AioDevId, &stAioAttr);
    if (s32Ret)
    {
        WV_ERROR("HI_MPI_AI_SetPubAttr(%d) failed with %#x\n", AioDevId, s32Ret);
        return WV_EFAIL;
    }

    s32Ret = HI_MPI_AI_Enable(AioDevId);
    if (s32Ret)
    {
        WV_ERROR("HI_MPI_AI_Enable(%d) failed with %#x\n", AioDevId, s32Ret);
        return WV_EFAIL;
    }

    for (i = 0; i < 2; i++)
    {
        s32Ret = HI_MPI_AI_EnableChn(AioDevId, i);
        if (s32Ret)
        {
             WV_ERROR("HI_MPI_AI_EnableChn(%d:%d) failed with %#x\n", AioDevId,i, s32Ret);
        	return WV_EFAIL;
        }

		
    }
    
    
  return WV_SOK;  
    
}


/*****************************************************************

WV_S32  HIS_AI_DeInit(WV_S32  devAi);

*****************************************************************/
WV_S32  HIS_AI_DeInit(WV_S32  devAi)
{
	WV_S32 s32Ret,i;
	AUDIO_DEV AioDevId;  // i 1 2  o:  0 1  0 2
	 
    AioDevId = devAi; 

    for (i = 0; i < 2; i++)
    {
        s32Ret = HI_MPI_AI_DisableChn(AioDevId, i);
        if (s32Ret)
        {
             WV_ERROR("HI_MPI_AI_DisableChn(%d:%d) failed with %#x\n", AioDevId,i, s32Ret);
        	return WV_EFAIL;
        }

		
    }
   s32Ret = HI_MPI_AI_Disable(AioDevId);
    if (s32Ret)
    {
        WV_ERROR("HI_MPI_AI_Disable(%d) failed with %#x\n", AioDevId, s32Ret);
        return WV_EFAIL;
    } 
    
  return WV_SOK;  
    
}


/*****************************************************************

WV_S32  HIS_AI_GetFrame(WV_S32  devAi,WV_U8 *pBuf, WV_U32 * pLen ,WV_U64 *pPts);

*****************************************************************/
WV_S32  HIS_AI_GetFrame(WV_S32  devAi,WV_U8 *pBuf,WV_U32 * pLen ,WV_U64 *pPts)
{
	AUDIO_FRAME_S  audioFrame;
	WV_U32 len;
	WV_S32 ret;
	ret = HI_MPI_AI_GetFrame(devAi,0, &audioFrame,NULL,-1);
	  if(ret != WV_SOK)
	   {
	     WV_ERROR("HI_MPI_AI_GetFrame(%d) failed with %#x\r\n",devAi,ret);
	     return WV_EFAIL;
	    }
	 len =  (audioFrame.enBitwidth + 1) * audioFrame.u32Len;
	*pPts =  audioFrame.u64TimeStamp;
	 memcpy(pBuf,audioFrame.pVirAddr[0],len);
	 memcpy(pBuf+len,audioFrame.pVirAddr[1],len); 
	 *pLen = len;
	 
	 return  WV_SOK;
}



/*****************************************************************

WV_S32  HIS_AO_Init(WV_S32  devAo);

*****************************************************************/
WV_S32  HIS_AO_Init(WV_S32  devAo)
{
	WV_S32 s32Ret,i;
	AUDIO_DEV  AioDevId;  // i 1 2  o:  0 1  0 2
	AIO_ATTR_S stAioAttr;
	
    stAioAttr.enSamplerate   = AUDIO_SAMPLE_RATE_48000;
    stAioAttr.enBitwidth     = AUDIO_BIT_WIDTH_16;
    stAioAttr.enWorkmode     = AIO_MODE_I2S_SLAVE;
    stAioAttr.enSoundmode    = AUDIO_SOUND_MODE_STEREO;//STEREO  MONO
    stAioAttr.u32EXFlag      = 1;
    stAioAttr.u32FrmNum      = 30;
    stAioAttr.u32PtNumPerFrm = 320;//640  1280
    stAioAttr.u32ChnCnt      = 1;//?
    stAioAttr.u32ClkChnCnt   = 2;//?
    stAioAttr.u32ClkSel      = 0;  
    
    AioDevId = devAo;
    s32Ret = HI_MPI_AO_SetPubAttr(AioDevId, &stAioAttr);
    if (s32Ret)
    {
        WV_ERROR("HI_MPI_AO_SetPubAttr(%d) failed with %#x\n", AioDevId, s32Ret);
        return WV_EFAIL;
    }
    s32Ret = HI_MPI_AO_Enable(AioDevId);
    if (s32Ret)
    {
        WV_ERROR("HI_MPI_AO_Enable(%d) failed with %#x\n", AioDevId, s32Ret);
        return WV_EFAIL;
    }

    for (i = 0; i < 2; i++)
    {
        s32Ret = HI_MPI_AO_EnableChn(AioDevId, i);
        if (s32Ret)
        {
             WV_ERROR("HI_MPI_AO_EnableChn(%d:%d) failed with %#x\n", AioDevId,i, s32Ret);
        	return WV_EFAIL;
        }

		
    }
    
    
  return WV_SOK;  
    
}


/*****************************************************************

WV_S32  HIS_AO_DeInit(WV_S32  devAo);

*****************************************************************/
WV_S32  HIS_AO_DeInit(WV_S32  devAo)
{
	WV_S32 s32Ret,i;
	AUDIO_DEV AioDevId;  // i 1 2  o:  0 1  0 2

	
    AioDevId = devAo; 

    for (i = 0; i < 2; i++)
    {
        s32Ret = HI_MPI_AO_DisableChn(AioDevId, i);
        if (s32Ret)
        {
             WV_ERROR("HI_MPI_AO_DisableChn(%d:%d) failed with %#x\n", AioDevId,i, s32Ret);
        	return WV_EFAIL;
        }

		
    }
   s32Ret = HI_MPI_AO_Disable(AioDevId);
    if (s32Ret)
    {
        WV_ERROR("HI_MPI_AO_Disable(%d) failed with %#x\n", AioDevId, s32Ret);
        return WV_EFAIL;
    } 
    
  return WV_SOK;  
    
}



/*****************************************************************

WV_S32  HIS_AO_SendFrame(WV_S32  devAo,WV_U8 *pBuf, WV_U32 * pLen ,WV_U64 *pPts);

*****************************************************************/
WV_S32  HIS_AO_SendFrame(WV_S32  devAo,WV_U8 *pBuf, WV_U32 len ,WV_U64 pts)
{
	AUDIO_FRAME_S  audioFrame;
	WV_S32 ret;
	audioFrame.enBitwidth = 1;
	audioFrame.u32Len = len /4;
	  memcpy(audioFrame.pVirAddr[0],pBuf,len/2);
	  memcpy(audioFrame.pVirAddr[0],pBuf+len /2,len/2);
     audioFrame.u64TimeStamp = pts;  
	 ret = HI_MPI_AO_SendFrame(devAo,0, &audioFrame,0);
	  if(ret != WV_SOK)
	   {
	     WV_ERROR("HI_MPI_AO_SendFrame(%d) failed with %#x\r\n",devAo,ret);
	     return WV_EFAIL;
	    }
	
	 
	 return  WV_SOK;
}
