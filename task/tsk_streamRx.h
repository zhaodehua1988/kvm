#ifndef _WV_TSK_STREAM_RX_H_
#define _WV_TSK_STREAM_RX_H_
#include "wv_common.h" 
#include "sys.h" 
WV_S32  TSK_SRX_Init(); 
WV_S32  TSK_SRX_DeInit();
WV_S32  TSK_SRX_SetRxIp(WV_S32 chn, WV_S8 * ipAddr,WV_U32 port);
WV_S32  TSK_SRX_TestTx(WV_U8 * pTxBuf,WV_U32 len);//for teset
#endif
