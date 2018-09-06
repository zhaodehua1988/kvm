#include"his_vdec.h"
#include"mpi_vdec.h"
#include"hi_comm_vdec.h"

/******************************************************************************

 WV_S32  HIS_VDEC_InitMode()

******************************************************************************/
 WV_S32  HIS_VDEC_InitMode()
 {
 
     VDEC_MOD_PARAM_S stModParam;
     WV_S32 ret;
    stModParam.u32MiniBufMode = 0;
    stModParam.u32VBSource = 1;
    ret = HI_MPI_VDEC_SetModParam(&stModParam);
       if(ret != WV_SOK)
         {
             WV_ERROR(" HI_S32 HI_MPI_VDEC_SetModParam  :ret = %#x\r\n",ret);
             return WV_EFAIL;
         }  
 }

/******************************************************************************

 WV_S32  HIS_VDEC_Init(WV_S32 vdecChn,WV_U32 width,WV_U32 height)

******************************************************************************/

WV_S32  HIS_VDEC_Init(WV_S32 vdecChn,WV_U32 width,WV_U32 height)
{
        VDEC_CHN_ATTR_S  stVdecChnAttr;
        stVdecChnAttr.enType       = PT_H264;
        stVdecChnAttr.u32BufSize   = WV_U32ALIGN_16UP (3 * width *width);
        stVdecChnAttr.u32Priority  = 5;
        stVdecChnAttr.u32PicWidth  = width;
        stVdecChnAttr.u32PicHeight = height;
        stVdecChnAttr.stVdecVideoAttr.enMode=VIDEO_MODE_FRAME;
        stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 5;
        stVdecChnAttr.stVdecVideoAttr.bTemporalMvpEnable = 0;
         
         WV_S32 ret;
         
         ret = HI_MPI_VDEC_CreateChn(vdecChn, &stVdecChnAttr);
       if(ret != WV_SOK)
         {
             WV_ERROR("HI_MPI_VDEC_CreateChn [%d] :ret = %#x\r\n",vdecChn,ret);
             return WV_EFAIL;
         }  
         ret = HI_MPI_VDEC_SetDisplayMode(vdecChn, VIDEO_DISPLAY_MODE_PREVIEW); //VIDEO_DISPLAY_MODE_PLAYBACK  PREVIEW
           if(ret != WV_SOK)
		     {
		         WV_ERROR("HI_MPI_VDEC_SetDisplayMode [%d] :ret = %#x\r\n",vdecChn,ret);
		         return WV_EFAIL;
		     } 
               
         ret = HI_MPI_VDEC_StartRecvStream(vdecChn); 
        if(ret != WV_SOK)
         {
             WV_ERROR("HI_MPI_VDEC_StartRecvStream [%d] :ret = %#x\r\n",vdecChn,ret);
             return WV_EFAIL;
         } 
         
   return  WV_SOK;
}


/******************************************************************************

WV_S32  HIS_VDEC_DeInit(WV_S32  vdecChn)

******************************************************************************/

WV_S32  HIS_VDEC_DeInit(WV_S32  vdecChn)
{
    WV_S32 ret;
    ret = HI_MPI_VDEC_StopRecvStream(vdecChn);   
    if(ret != WV_SOK)
         {
             WV_ERROR("HI_MPI_VDEC_StopRecvStream [%d] :ret = %#x\r\n",vdecChn,ret);
             return WV_EFAIL;
         }       
    ret = HI_MPI_VDEC_DestroyChn(vdecChn);
    if(ret != WV_SOK)
         {
             WV_ERROR("HI_MPI_VDEC_DestroyChn [%d] :ret = %#x\r\n",vdecChn,ret);
             return WV_EFAIL;
         }     
  return WV_SOK;
}



/******************************************************************************

WV_S32  HIS_VDEC_SendSream(WV_S32  vdecChn,WV_U8  *Pbuf,WV_U32 len ,WV_U64  pts)

******************************************************************************/

WV_S32 HIS_VDEC_SendSream(WV_S32  vdecChn,WV_U8  *Pbuf,WV_U32 len ,WV_U64  pts)
{
 
	VDEC_STREAM_S stStream;

	stStream.u64PTS  = pts;
	stStream.pu8Addr = Pbuf;
	stStream.u32Len  =  len; 
	stStream.bEndOfFrame   = HI_TRUE;
	stStream.bEndOfStream  = HI_FALSE;  

	return  HI_MPI_VDEC_SendStream(vdecChn, &stStream,0);  
}				
