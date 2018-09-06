#ifndef _HIS_MODE_SSP_H_H_
#define _HIS_MODE_SSP_H_H_
#include "wv_common.h"

typedef struct {

  int fd;

} HIS_SSP_HNDL;

 
WV_S32  HIS_SSP_Open(HIS_SSP_HNDL *pHndl,WV_U8 csn,WV_U8 mode);
WV_S32  HIS_SSP_Close(HIS_SSP_HNDL *pHndl);

WV_S32  HIS_SSP_Write(HIS_SSP_HNDL hndl,WV_U8 *addrBuf,WV_S32 addrLen,WV_U8 *dataBuf,WV_S32 dataLen);
WV_S32  HIS_SSP_Read (HIS_SSP_HNDL hndl,WV_U8 *addrBuf,WV_S32 addrLen,WV_U8 *dataBuf,WV_S32 dataLen);
 
WV_S32  HIS_SSP_Init();
WV_S32  HIS_SSP_DeInit();
#endif
