#ifndef _STRM_STREAM_PES_H_
#define _STRM_STREAM_PES_H_
#include"stream.h"
typedef struct  STRM_ES_INFO_S
{ 
WV_U8    sync;
WV_U8    streamId;
WV_U16   lenPack;
WV_U16   lenHead;
WV_U8    ptsFlag;
WV_U64   pts;
WV_U32 	 loadLen;
WV_U8   *pLoad; 

}STRM_ES_INFO_S;


WV_S32  STRM_EsAnlyse(WV_U8 *pBuf,STRM_ES_INFO_S *pInfo,WV_U32 len); 

#endif
