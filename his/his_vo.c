#include "his_vo.h"
#include"mpi_vo.h"
#include"hi_comm_vo.h"


/***********************************************************************************************

WV_S32 HIS_VO_GetWhf(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm)

***********************************************************************************************/
WV_S32 HIS_VO_GetWhf(VO_INTF_SYNC_E enIntfSync, HI_U32 *pu32W,HI_U32 *pu32H, HI_U32 *pu32Frm)
{
    switch (enIntfSync)
    {
        case VO_OUTPUT_PAL       :  *pu32W = 720;  *pu32H = 576;  *pu32Frm = 25; break;
        case VO_OUTPUT_NTSC      :  *pu32W = 720;  *pu32H = 480;  *pu32Frm = 30; break;        
        case VO_OUTPUT_576P50    :  *pu32W = 720;  *pu32H = 576;  *pu32Frm = 50; break;
        case VO_OUTPUT_480P60    :  *pu32W = 720;  *pu32H = 480;  *pu32Frm = 60; break;
        case VO_OUTPUT_800x600_60:  *pu32W = 800;  *pu32H = 600;  *pu32Frm = 60; break;
        case VO_OUTPUT_720P50    :  *pu32W = 1280; *pu32H = 720;  *pu32Frm = 50; break;
        case VO_OUTPUT_720P60    :  *pu32W = 1280; *pu32H = 720;  *pu32Frm = 60; break;        
        case VO_OUTPUT_1080I50   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 50; break;
        case VO_OUTPUT_1080I60   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 60; break;
        case VO_OUTPUT_1080P24   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 24; break;        
        case VO_OUTPUT_1080P25   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 25; break;
        case VO_OUTPUT_1080P30   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 30; break;
        case VO_OUTPUT_1080P50   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 50; break;
        case VO_OUTPUT_1080P60   :  *pu32W = 1920; *pu32H = 1080; *pu32Frm = 60; break;
        case VO_OUTPUT_1024x768_60:  *pu32W = 1024; *pu32H = 768;  *pu32Frm = 60; break;
        case VO_OUTPUT_1280x1024_60: *pu32W = 1280; *pu32H = 1024; *pu32Frm = 60; break;
        case VO_OUTPUT_1366x768_60:  *pu32W = 1366; *pu32H = 768;  *pu32Frm = 60; break;
        case VO_OUTPUT_1440x900_60:  *pu32W = 1440; *pu32H = 900;  *pu32Frm = 60; break;
        case VO_OUTPUT_1280x800_60:  *pu32W = 1280; *pu32H = 800;  *pu32Frm = 60; break;        
        case VO_OUTPUT_1600x1200_60: *pu32W = 1600; *pu32H = 1200; *pu32Frm = 60; break;
        case VO_OUTPUT_1680x1050_60: *pu32W = 1680; *pu32H = 1050; *pu32Frm = 60; break;
        case VO_OUTPUT_1920x1200_60: *pu32W = 1920; *pu32H = 1200; *pu32Frm = 60; break;
        case VO_OUTPUT_2560x1600_60: *pu32W = 2560; *pu32H = 1600; *pu32Frm = 60; break;
        case VO_OUTPUT_3840x2160_30: *pu32W = 3840; *pu32H = 2160; *pu32Frm = 30; break;
        case VO_OUTPUT_3840x2160_60: *pu32W = 3840; *pu32H = 2160; *pu32Frm = 60; break;
        default: 
            WV_ERROR("vo enIntfSync not support!\n");
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/***********************************************************************************************

WV_S32 HIS_VO_GetIntfSync(HI_U32 u32W,HI_U32 u32H, HI_U32 u32Frm ,VO_INTF_SYNC_E *pIntfSync)

***********************************************************************************************/
WV_S32 HIS_VO_GetIntfSync(HI_U32 u32W,HI_U32 u32H, HI_U32 u32Frm ,VO_INTF_SYNC_E *pIntfSync)
{
  
	if(u32W == 720  &&   u32H == 576  && u32Frm == 25)
		{
		*pIntfSync =  VO_OUTPUT_PAL;
		return WV_SOK;
		}


	if(u32W == 720  &&   u32H == 480  && u32Frm == 30)
		{
		*pIntfSync =  VO_OUTPUT_NTSC;
		return WV_SOK;
		}

	if(u32W == 720  &&   u32H == 576  && u32Frm == 50)
		{
		*pIntfSync =  VO_OUTPUT_576P50;
		return WV_SOK;
		} 

	if(u32W == 720  &&   u32H == 480  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_480P60;
		return WV_SOK;
		} 

	if(u32W == 800  &&   u32H == 600  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_800x600_60;
		return WV_SOK;
		} 

	if(u32W == 1280  &&   u32H == 720  && u32Frm == 50)
		{

		*pIntfSync =  VO_OUTPUT_720P50;
		return WV_SOK;
		} 

	if(u32W == 1280  &&   u32H == 720  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_720P60;
		return WV_SOK;
		} 
	if(u32W == 1920  &&   u32H == 1080  && u32Frm == 50)
		{
		*pIntfSync =  VO_OUTPUT_1080P50;
		return WV_SOK;
		}  
	if(u32W == 1920  &&   u32H == 1080  && u32Frm == 24)
		{
		*pIntfSync =  VO_OUTPUT_1080P24;
		return WV_SOK;
		}

	if(u32W == 1920  &&   u32H == 1080  && u32Frm == 25)
		{
		*pIntfSync =  VO_OUTPUT_1080P25;
		return WV_SOK;
		} 
	if(u32W == 1920  &&   u32H == 1080  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1080P60;
		return WV_SOK;
		}      
	if(u32W == 1024  &&   u32H == 768  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1024x768_60;
		return WV_SOK;
		}

	if(u32W == 1280  &&   u32H == 1024  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1280x1024_60;
		return WV_SOK;
		}    
	
	if(u32W == 1366  &&   u32H == 768  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1366x768_60;
		return WV_SOK;
		}         
	if(u32W == 1440  &&   u32H == 900  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1440x900_60;
		return WV_SOK;
		}  

	if(u32W == 1280  &&   u32H == 800  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1280x800_60;
		return WV_SOK;
		}                          
	if(u32W == 1600  &&   u32H == 1200  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1600x1200_60;
		return WV_SOK;
		}          

	if(u32W == 1680  &&   u32H == 1050  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1680x1050_60;
		return WV_SOK;
		} 

	if(u32W == 1920  &&   u32H == 1200  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_1920x1200_60;
		return WV_SOK;
		}                  
	if(u32W == 2560  &&   u32H == 1600  && u32Frm == 60)
		{
		*pIntfSync =  VO_OUTPUT_2560x1600_60;
		return WV_SOK;
		}         
	if(u32W == 3840  &&   u32H == 2160  && u32Frm == 30)
		{
		*pIntfSync =  VO_OUTPUT_3840x2160_30;
		return WV_SOK;
		}
	WV_ERROR("vo enIntfSync cant get\r\n");
	  *pIntfSync =  VO_OUTPUT_1080P60;
	return WV_EFAIL;
}

/*************************************************************************************

WV_S32  HIS_VO_Init(WV_S32  devVo,WV_U32 width,WV_U32 height,WV_U32 frate)

*************************************************************************************/
WV_S32  HIS_VO_Init(WV_S32 devVo,WV_U32 width,WV_U32 height,WV_U32 frate)
{
 	VO_DEV VoDev;    
    VO_LAYER VoLayer;
    VO_PUB_ATTR_S stVoPubAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VO_CHN_ATTR_S stChnAttr;
    VO_CAS_ATTR_S stCasAttr;
    WV_S32 s32Ret;
	WV_U32 refdivOld;
	WV_U32 refdivNew;
		
	
    //enable VO 
    
    stCasAttr.bSlave = HI_FALSE;//HI_TRUE;//HI_FALSE;//sleav master
    stCasAttr.enCasMode = VO_CAS_MODE_SINGLE;
    stCasAttr.enCasRgn = VO_CAS_64_RGN;
    stCasAttr.enCasDataTranMode = VO_CAS_DATA_DOUBLE_TRAN_MODE;//VO_CAS_DATA_DOUBLE_TRAN_MODE;  // VO_CAS_DATA_SINGLE_TRAN_MODE;
    
    //  
 	if(devVo == 0)
 	{
 		VoDev = 0;
    	VoLayer = 0;
    	stVoPubAttr.enIntfType = VO_INTF_BT1120;// | VO_INTF_HDMI;
    }	
    else
    {
      VoDev = 1;
      VoLayer = 1;
      stVoPubAttr.enIntfType = VO_INTF_VGA;
    }
    
  if(devVo == 0)
 	{
	 	s32Ret = HI_MPI_VO_SetCascadeAttr(&stCasAttr);
		if (HI_SUCCESS != s32Ret)
			{
				WV_ERROR("Set HI_MPI_VO_SetCascadeAttr failed!\r\n");
				return WV_EFAIL;
			}    	 
    }  
    
    
  s32Ret = HIS_VO_GetIntfSync(width,height,frate ,&(stVoPubAttr.enIntfSync));
     if (s32Ret != HI_SUCCESS)
		{
		   WV_ERROR("GetIntfSync dev[%d] with %#x!\n",VoDev, s32Ret);
		    return HI_FAILURE;
		}
  
  
   stVoPubAttr.u32BgColor = 0x000000FF;

	if(devVo == 0)
	{	
		//user模式下是单沿，为了配置双沿模式，所以设置需要先设置3840*2160
		stVoPubAttr.enIntfSync =VO_OUTPUT_3840x2160_30;
		s32Ret = HI_MPI_VO_SetPubAttr(VoDev, &stVoPubAttr);

		//sleep(1);
		s32Ret = HI_MPI_VO_Enable(VoDev);
		if (s32Ret != HI_SUCCESS)
		{
		    WV_ERROR("Enable dev[%d]failed with %#x!\n",VoDev, s32Ret);
		    return HI_FAILURE;
		}

		s32Ret=HI_MPI_VO_Disable(VoDev);
		if (s32Ret != HI_SUCCESS)
		{
		    WV_ERROR("Enable dev[%d]failed with %#x!\n",VoDev, s32Ret);
		    return HI_FAILURE;
		}


#if 1

		HI_MPI_VO_SetCascadeAttr(&stCasAttr);

		//59HZ
   		stVoPubAttr.enIntfSync =VO_OUTPUT_USER;//VO_OUTPUT_2560x1600_60;
		stVoPubAttr.stSyncInfo.bSynm = 0;
		stVoPubAttr.stSyncInfo.bIop = 1;
		stVoPubAttr.stSyncInfo.u8Intfb = 10;

		stVoPubAttr.stSyncInfo.u16Vact = 1600;
		stVoPubAttr.stSyncInfo.u16Vbb = 37;//49;//49
		stVoPubAttr.stSyncInfo.u16Vfb = 3;

		stVoPubAttr.stSyncInfo.u16Hact = 2560;
		stVoPubAttr.stSyncInfo.u16Hbb = 80;//80;//472-78;//472;
		stVoPubAttr.stSyncInfo.u16Hfb = 48;//48;//192-78;//192;
		stVoPubAttr.stSyncInfo.u16Hmid = 32;//280-78;//280;

		stVoPubAttr.stSyncInfo.u16Bvact = 1600;
		stVoPubAttr.stSyncInfo.u16Bvbb = 37;//49;
		stVoPubAttr.stSyncInfo.u16Bvfb = 3;

		stVoPubAttr.stSyncInfo.u16Hpw = 32;//100;//280;
		stVoPubAttr.stSyncInfo.u16Vpw = 6;

		stVoPubAttr.stSyncInfo.bIdv = 0;
		stVoPubAttr.stSyncInfo.bIhs = 0;
		stVoPubAttr.stSyncInfo.bIvs = 0;
#endif

				
	}  

	s32Ret = HI_MPI_VO_SetPubAttr(VoDev, &stVoPubAttr);
		if (s32Ret != HI_SUCCESS)
		{
		   WV_ERROR("SetPubAttr dev[%d] with %#x! %d %d \r\n",VoDev, s32Ret,stVoPubAttr.enIntfType,stVoPubAttr.enIntfSync);
		    return HI_FAILURE;
		}
	
	if(devVo == 0)
	{
		//set VPLL0 整数倍频系数,pcl=267(268.5)
		HI_MPI_SYS_GetReg(0x12040014,&refdivOld);
		refdivNew = (refdivOld & 0xfffff000) | 0x59;
		HI_MPI_SYS_SetReg(0x12040014, refdivNew);
		/*
		printf("\nHI_MPI_VO_SetDevFrameRate(VoDev, 60);\n");
		HI_U32 u32FrameRate;
		HI_MPI_VO_GetDevFrameRate(VoDev,&u32FrameRate);
		printf(" 1 get framerate [%d]\n",u32FrameRate);
		HI_MPI_VO_SetDevFrameRate(VoDev, 60);
		HI_MPI_VO_GetDevFrameRate(VoDev,&u32FrameRate);
		printf("2 get framerate [%d]\n",u32FrameRate);
		*/

	}

	


    s32Ret = HI_MPI_VO_Enable(VoDev);
    if (s32Ret != HI_SUCCESS)
    {
        WV_ERROR("Enable dev[%d]failed with %#x!\n",VoDev, s32Ret);
        return HI_FAILURE;
    }

	//HI_MPI_VO_SetCascadeAttr(&stCasAttr);   

   //layer
   
    memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stLayerAttr.u32DispFrmRt = frate;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = width;
    stLayerAttr.stDispRect.u32Height  = height;
    stLayerAttr.stImageSize.u32Width  = width;
    stLayerAttr.stImageSize.u32Height = height;
    
   s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoLayer, &stLayerAttr);
    if (s32Ret != HI_SUCCESS)
    {
        WV_ERROR("failed with[%d] SetVideoLayerAttr  %#x!\n",VoLayer,s32Ret);
        return WV_EFAIL;
    }

    s32Ret = HI_MPI_VO_EnableVideoLayer(VoLayer);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("EnableVideoLayer [%d]failed with %#x!\n",VoLayer, s32Ret);
        return WV_EFAIL;
    }

    /* start cascade dev */
    if(devVo == 0)
    {
		s32Ret = HI_MPI_VO_EnableCascadeDev(VO_CAS_DEV_1);
		if (HI_SUCCESS != s32Ret)
		{
		    WV_ERROR("Start VO_CAS_DEV_1 failed!\n");
		     return WV_EFAIL;
		}
    }
    /************vo get buflen ********************/
	HI_U32 u32BufLen;
	HI_MPI_VO_GetDispBufLen(VoLayer, &u32BufLen);
	printf("vo get dis buf len dev[%d]= %d \n ",devVo,u32BufLen);
    //star chn

		if(devVo == 0)
			{
			VoLayer = VO_CAS_DEV_1+1;
			}
        stChnAttr.stRect.s32X       = 0;
        stChnAttr.stRect.s32Y       = 0;
        stChnAttr.stRect.u32Width   =  width;
        stChnAttr.stRect.u32Height  =  height;
        stChnAttr.u32Priority       = 0;
        stChnAttr.bDeflicker        = HI_FALSE;

        s32Ret = HI_MPI_VO_SetChnAttr(VoLayer, 0, &stChnAttr);
        if (s32Ret != HI_SUCCESS)
        {
           WV_ERROR("(%d)SetChnAttr failed with %#x!\n",VoLayer,s32Ret);
            return HI_FAILURE;
        }

        s32Ret = HI_MPI_VO_EnableChn(VoLayer, 0);
        if (s32Ret != HI_SUCCESS)
        {
           WV_ERROR("(%d)failed with %#x!\n",VoLayer, s32Ret);
            return HI_FAILURE;
        }
        
       	if(devVo == 0)
			{
			 	s32Ret = HI_MPI_VO_CascadePosBindChn(0, VO_CAS_DEV_1, 0);
					if (HI_SUCCESS != s32Ret)
					{
						    WV_ERROR("(%d)CascadePosBindChn failed with %#x!\n",VoLayer, s32Ret);
           					 return HI_FAILURE;
					}
		          
				s32Ret = HI_MPI_VO_SetCascadePattern(VO_CAS_DEV_1, 4);
				if (HI_SUCCESS != s32Ret)
				{
					 WV_ERROR("(%d)SetCascadePattern failed with %#x!\n",VoLayer, s32Ret);
           			 return HI_FAILURE;
				}

				s32Ret = HI_MPI_VO_EnableCascade();
				if (HI_SUCCESS != s32Ret)
				{
					 WV_ERROR(" EnableCascade failed with %#x!\n",s32Ret);
           			 return HI_FAILURE;
				}			
			}
 
	 	if(devVo == 1)
			{
			  VO_VGA_PARAM_S  stVgaParam;
			    s32Ret =HI_MPI_VO_GetVgaParam(devVo, &stVgaParam);
			   if (HI_SUCCESS != s32Ret)
				{
					 WV_ERROR(" HI_MPI_VO_GetVgaParam failed with %#x!\n",s32Ret);
           			 return HI_FAILURE;
				}	
			   stVgaParam.u32Gain = 0;			   
			   stVgaParam.s32SharpenStrength = 128;
			   stVgaParam.stCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;//VO_CSC_MATRIX_BT601_TO_RGB_PC
			   
			   stVgaParam.stCSC.u32Luma 		= 50;
			   stVgaParam.stCSC.u32Contrast		= 33;
			   stVgaParam.stCSC.u32Hue		    = 50;
			   stVgaParam.stCSC.u32Saturation	= 50;
			   
			  s32Ret = HI_MPI_VO_SetVgaParam(devVo,&stVgaParam);
			  if (HI_SUCCESS != s32Ret)
				{
					 WV_ERROR(" HI_MPI_VO_SetVgaParam failed with %#x!\n",s32Ret);
           			 return HI_FAILURE;
				}		
			}	
  
	return  WV_SOK;
}


/*************************************************************************************

WV_S32  HIS_VO_DeInit(WV_S32  devVo)

*************************************************************************************/
WV_S32  HIS_VO_DeInit(WV_S32  devVo)
{
   	VO_DEV VoDev;    
    VO_LAYER VoLayer;
    WV_S32  s32Ret;
     if(devVo == 0)
    {
       
       VoDev = 0;
       VoLayer =  VO_CAS_DEV_1+1; 
    }
    else
    {
    
       VoDev = 1;
       VoLayer =  1; 
    }
    
    
    /* stop cascade */
    if(devVo == 0)
    {
       
        HI_MPI_VO_DisableCascade(); 
        HI_MPI_VO_CascadePosUnBindChn(0, VO_CAS_DEV_1, 4);
        
    }
 
   s32Ret = HI_MPI_VO_DisableChn(VoLayer, 0);
        if (s32Ret != HI_SUCCESS)
        {
           WV_ERROR("failed with %#x!\n", s32Ret);
         return HI_FAILURE;
        }
    /* stop cascade dev */
      if(devVo == 0)
		{
		   HI_MPI_VO_DisableCascadeDev(VO_CAS_DEV_1);
		 }  
     
     VoLayer = devVo;//
     
	s32Ret = HI_MPI_VO_DisableVideoLayer(VoLayer);
		if (s32Ret != HI_SUCCESS)
		{
		    WV_ERROR("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
		}
	s32Ret = HI_MPI_VO_Disable(VoDev);
		if (s32Ret != HI_SUCCESS)
		{
		    WV_ERROR("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
		}
     return WV_SOK;    
}

