#ifndef _WV_HIS_VCAP_H__
#define _WV_HIS_VCAP_H__
#include "wv_common.h"  
WV_S32  HIS_VI_Init(WV_S32  viDev, WV_S32 modeClk,WV_U32  width,WV_U32  hight);
WV_S32  HIS_VI_DeInit(WV_S32  viDev);
WV_S32  HIS_VI_GetResolution(WV_S32  viChn,WV_U32 *pWidth,WV_U32  *pHeight,WV_U32 *pFreamRate); 
WV_S32  HIS_VI_SetResolution(WV_S32  viChn,WV_S32  viDev,WV_U32 width,WV_U32  height,WV_U32 mode);
#endif 
