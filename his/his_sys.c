#include"his_sys.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include"mpi_sys.h"
#include"mpi_vb.h"
#define  HIS_SYS_VB_MAX_USER             22
#define  HIS_SYS_VB_MAX_POOLS			256

#define  HIS_SYS_MAX_POOLCNT             128
#define  HIS_SYS_MAX_BLKCNT           	 8
#define  HIS_SYS_VB_HEADER_STRIDE   	 16
#define  HIS_SYS_ALIGNWIDTH              16




/***************************************************************************

WV_S32  HIS_SYS_Init();

***************************************************************************/

WV_S32  HIS_SYS_Init()   
{


	WV_S32 i;
	VB_CONF_S vbConf; 
	MPP_SYS_CONF_S sysConf; 
	// init vb
	memset(&vbConf, 0, sizeof(VB_CONF_S)); 
	vbConf.u32MaxPoolCnt =  HIS_SYS_MAX_POOLCNT;
	vbConf.astCommPool[0].u32BlkSize = WV_U32ALIGN_16UP( HIS_SYS_MAX_PICHEIGH);
	vbConf.astCommPool[0].u32BlkSize *=  WV_U32ALIGN_16UP( (HIS_SYS_MAX_PICWIDTH)+ HIS_SYS_VB_HEADER_STRIDE );
	vbConf.astCommPool[0].u32BlkSize *= 2; 
	vbConf.astCommPool[0].u32BlkCnt = HIS_SYS_MAX_BLKCNT;

	vbConf.astCommPool[1].u32BlkSize = WV_U32ALIGN_16UP( 1920);
	vbConf.astCommPool[1].u32BlkSize *=  WV_U32ALIGN_16UP( (1080)+ HIS_SYS_VB_HEADER_STRIDE );
	vbConf.astCommPool[1].u32BlkSize *= 2; 
	vbConf.astCommPool[1].u32BlkCnt = 8;
   
	HI_MPI_SYS_Exit();

	for(i=0;i<HIS_SYS_VB_MAX_USER;i++)
		{
		HI_MPI_VB_ExitModCommPool(i);
		}
	for(i=0; i<HIS_SYS_VB_MAX_POOLS; i++)
		{
		HI_MPI_VB_DestroyPool(i);
		}
	HI_MPI_VB_Exit(); 


	WV_CHECK_RET(HI_MPI_VB_SetConf(&vbConf));  
	WV_CHECK_RET(HI_MPI_VB_Init()); 
	//init sys 
	memset(&sysConf, 0, sizeof(MPP_SYS_CONF_S));   
	sysConf.u32AlignWidth = HIS_SYS_ALIGNWIDTH;
	WV_CHECK_RET(HI_MPI_SYS_SetConf(&sysConf));
	WV_CHECK_RET(HI_MPI_SYS_Init());  
	return WV_SOK;
}


/***************************************************************************

WV_S32  HIS_SYS_DeInit();

***************************************************************************/

WV_S32  HIS_SYS_DeInit()
{
	WV_S32 i;
	HI_MPI_SYS_Exit();

	for(i=0;i<HIS_SYS_VB_MAX_USER;i++)
		{
		HI_MPI_VB_ExitModCommPool(i);
		}
	for(i=0; i<HIS_SYS_VB_MAX_POOLS; i++)
		{
		HI_MPI_VB_DestroyPool(i);
		}
	HI_MPI_VB_Exit(); 
	return WV_SOK;
}


/***************************************************************************

WV_S32  HIS_SYS_VO_BindVpss(WV_S32 VoLayer,WV_S32 VoChn,WV_S32 VpssGrp,WV_S32 VpssChn)

***************************************************************************/

WV_S32  HIS_SYS_VO_BindVpss(WV_S32 VoLayer,WV_S32 VoChn,WV_S32 VpssGrp,WV_S32 VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return WV_SOK;
}

/***************************************************************************

WV_S32  HIS_SYS_VO_UnBindVpss(WV_S32 VoLayer,WV_S32 VoChn,WV_S32 VpssGrp,WV_S32_CHN VpssChn)

***************************************************************************/

WV_S32  HIS_SYS_VO_UnBindVpss(WV_S32 VoLayer,WV_S32 VoChn,WV_S32 VpssGrp,WV_S32  VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VOU;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return WV_SOK;
}


/***************************************************************************

WV_S32  HIS_SYS_VI_BindVpss(WV_S32 ViChn,WV_S32 VpssGrp)

***************************************************************************/

WV_S32  HIS_SYS_VI_BindVpss(WV_S32 ViChn,WV_S32 VpssGrp)
{
	HI_S32 s32Ret = HI_SUCCESS;
	MPP_CHN_S stSrcChn;
	MPP_CHN_S stDestChn;


	stSrcChn.enModId = HI_ID_VIU;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = ViChn;

	stDestChn.enModId = HI_ID_VPSS;
	stDestChn.s32DevId = VpssGrp;
	stDestChn.s32ChnId = 0;

	s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != HI_SUCCESS)
		{
		WV_ERROR("failed with %#x!\n", s32Ret);
		return HI_FAILURE;
		}
	return WV_SOK;
}



/***************************************************************************

WV_S32  HIS_SYS_VI_UnBindVpss(WV_S32 ViChn,WV_S32 VpssGrp)

***************************************************************************/

WV_S32  HIS_SYS_VI_UnBindVpss(WV_S32 ViChn,WV_S32 VpssGrp)
{
	HI_S32 s32Ret = HI_SUCCESS;
	MPP_CHN_S stSrcChn;
	MPP_CHN_S stDestChn;
	
	stSrcChn.enModId = HI_ID_VIU;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = ViChn;

	stDestChn.enModId = HI_ID_VPSS;
	stDestChn.s32DevId = VpssGrp;
	stDestChn.s32ChnId = 0;

	s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != HI_SUCCESS)
		{
			WV_ERROR("failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
	return WV_SOK;
}




/***************************************************************************

WV_S32  HIS_SYS_VENC_BindVpss(WV_S32 VencChn,WV_S32 VpssGrp,WV_S32  VpssChn)

***************************************************************************/

WV_S32  HIS_SYS_VENC_BindVpss(WV_S32 VencChn,WV_S32 VpssGrp,WV_S32  VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return WV_SOK;
}
/***************************************************************************

WV_S32  HIS_SYS_VENC_UnBindVpss(WV_S32 VencChn,WV_S32 VpssGrp,WV_S32 VpssChn)

***************************************************************************/

WV_S32  HIS_SYS_VENC_UnBindVpss(WV_S32 VencChn,WV_S32 VpssGrp,WV_S32 VpssChn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VPSS;
    stSrcChn.s32DevId = VpssGrp;
    stSrcChn.s32ChnId = VpssChn;

    stDestChn.enModId = HI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VencChn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return WV_SOK;
}


/***************************************************************************

WV_S32  HIS_SYS_VDEC_BindVpss(WV_S32 VdecChn,WV_S32 VpssGrp)

***************************************************************************/

WV_S32  HIS_SYS_VDEC_BindVpss(WV_S32 VdecChn,WV_S32 VpssGrp)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return WV_SOK;
}


/***************************************************************************

WV_S32  HIS_SYS_VDEC_UnBindVpss(WV_S32 VdecChn,WV_S32 VpssGrp)

***************************************************************************/

WV_S32  HIS_SYS_VDEC_UnBindVpss(WV_S32 VdecChn,WV_S32 VpssGrp)
{
    HI_S32 s32Ret = HI_SUCCESS;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    stSrcChn.enModId = HI_ID_VDEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VdecChn;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp;
    stDestChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
       WV_ERROR("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return WV_SOK;
}


/***************************************************************************

WV_S32  HIS_SYS_GetCurPts(WV_U64 *pCurPts)

***************************************************************************/

WV_S32  HIS_SYS_GetCurPts(WV_U64 *pCurPts)
{
  return   HI_MPI_SYS_GetCurPts(pCurPts);
}

