#ifndef _WV_HIS_VDEC_H__
#define _WV_HIS_VDEC_H__
#include "wv_common.h"  
WV_S32  HIS_VDEC_Init(WV_S32 vdecChn,WV_U32 width,WV_U32 height);
WV_S32  HIS_VDEC_DeInit(WV_S32  vdecChn);  
WV_S32 HIS_VDEC_SendSream(WV_S32  vdecChn,WV_U8  *Pbuf,WV_U32 len ,WV_U64  pts);
WV_S32  HIS_VDEC_InitMode();
#endif
