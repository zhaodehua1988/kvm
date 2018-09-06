#ifndef _WV_TASK_VENC_H__
#define _WV_TASK_VENC_H__
#include "sys.h"  
WV_S32  TSK_VENC_Init();
WV_S32  TSK_VENC_DeInit();
WV_S32  TSK_VENC_SetSendIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port);
#endif
