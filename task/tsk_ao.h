#ifndef _WV_TASK_AO_H__
#define _WV_TASK_AO_H__
#include "sys.h"  
WV_S32  TSK_AO_Init();
WV_S32  TSK_AO_DeInit(); 
WV_S32  TSK_AO_GetBuf(WV_S32 chn,WV_U32 *pBuf);
WV_S32  TSK_AO_PutBuf(WV_S32 chn,WV_U32  pBuf);
#endif
