#ifndef _WV_TASK_VDEC_H__
#define _WV_TASK_VDEC_H__
#include "sys.h"  
WV_S32  TSK_VDEC_Init();
WV_S32  TSK_VDEC_DeInit(); 

WV_S32 TSK_VDEC_GetBuf(WV_S32 chn,WV_U32 * pBuf);
WV_S32 TSK_VDEC_PutBuf(WV_S32 chn,WV_U32  pBuf);
#endif
