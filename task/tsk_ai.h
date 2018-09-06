#ifndef _WV_TASK_AI_H__
#define _WV_TASK_AI_H__
#include "sys.h"  
WV_S32  TSK_AI_Init();
WV_S32  TSK_AI_DeInit();
WV_S32  TSK_AI_SetSendIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port);
#endif
