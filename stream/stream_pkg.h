#ifndef _STRM_STREAM_PAKAGE_H_
#define _STRM_STREAM_PAKAGE_H_
#include"stream.h"
typedef struct  STRM_PACK_INFO_S
{
WV_U8 	sync; 
WV_U8 	endFlag; 
WV_U8 	cc; 
WV_U8 	pid;
WV_U16 	loadOffset;
WV_U16 	loadLen;
WV_U8   *pLoad;
WV_U64   txPts;

}STRM_PACK_INFO_S;


WV_S32  STRM_PackAnlyse(WV_U8 *pBuf,STRM_PACK_INFO_S *pInfo); 

#endif
