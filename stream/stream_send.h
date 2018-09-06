#ifndef _STRM_SEND_STREAM_H_
#define _STRM_SEND_STREAM_H_
#include "wv_common.h"
#include"stream.h"
WV_S32  STRM_SendEs(WV_ETH_UDP_TX_E *pDev,WV_U8 *pBuf, WV_U32 len,WV_U8 pid,WV_U32 *pCnt); 

#endif
