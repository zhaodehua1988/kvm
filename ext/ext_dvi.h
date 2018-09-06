#ifndef _EXT_DVI_INOUT_H__
#define _EXT_DVI_INOUT_H__
#include "sys.h"
WV_S32  EXT_DVI_Init();
WV_S32  EXT_DVI_DeInit(); 
WV_S32  EXT_DVI_SetSw(WV_U8  outChn, WV_U8  inChn, WV_U8 ena ); 
WV_S32  EXT_DVI_GetSw(WV_U8 *pSw);
#endif
