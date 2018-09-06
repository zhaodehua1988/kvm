#ifndef _EXT_ADECOD_AIC3268_H__
#define _EXT_ADECOD_AIC3268_H__
#include "sys.h"
WV_S32  EXT_3268_Init();
WV_S32  EXT_3268_DeInit();
WV_S32  EXT_3268_SetMux(WV_U32 chn ,WV_U32 ena);
WV_S32  EXT_3268_SetLinEnc(WV_U32 chn ,WV_U32 ena);
WV_S32  EXT_3268_SetMic(WV_U32 chn ,WV_U32 ena);
WV_S32  EXT_3268_GetMux(WV_U8 *pMuxEna);
#endif
