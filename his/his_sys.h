#ifndef _WV_HIS_SYS_H__
#define _WV_HIS_SYS_H__
#include "wv_common.h" 

#define HIS_SYS_MAX_PICHEIGH            2160
#define HIS_SYS_MAX_PICWIDTH            3840
 
WV_S32  HIS_SYS_Init();
WV_S32  HIS_SYS_DeInit();  

WV_S32  HIS_SYS_VO_BindVpss(WV_S32 VoLayer,WV_S32 VoChn,WV_S32 VpssGrp,WV_S32 VpssChn);
WV_S32  HIS_SYS_VO_UnBindVpss(WV_S32 VoLayer,WV_S32 VoChn,WV_S32 VpssGrp,WV_S32  VpssChn);

WV_S32  HIS_SYS_VI_BindVpss(WV_S32 ViChn,WV_S32 VpssGrp);
WV_S32  HIS_SYS_VI_UnBindVpss(WV_S32 ViChn,WV_S32 VpssGrp);

WV_S32  HIS_SYS_VENC_BindVpss(WV_S32 VencChn,WV_S32 VpssGrp,WV_S32  VpssChn);
WV_S32  HIS_SYS_VENC_UnBindVpss(WV_S32 VencChn,WV_S32 VpssGrp,WV_S32 VpssChn);

WV_S32  HIS_SYS_VDEC_BindVpss(WV_S32 VdecChn,WV_S32 VpssGrp);
WV_S32  HIS_SYS_VDEC_UnBindVpss(WV_S32 VdecChn,WV_S32 VpssGrp);

WV_S32  HIS_SYS_GetCurPts(WV_U64 *pCurPts);
#endif
