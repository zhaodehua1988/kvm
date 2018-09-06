#ifndef _HIS_MODE_IIC_H_H_
#define _HIS_MODE_IIC_H_H_
#include "wv_common.h"
typedef struct {

  int fd;

} HIS_IIC_HNDL;

WV_S32 HIS_IIC_Open  (HIS_IIC_HNDL  *pHndl, WV_U8 busId);
WV_S32 HIS_IIC_Close (HIS_IIC_HNDL  *pHndl);

WV_S32 HIS_IIC_Read8 (HIS_IIC_HNDL  hndl, WV_U8 devAddr, WV_U8 regAddr, WV_U8 *value, WV_U32 count);
WV_S32 HIS_IIC_Write8(HIS_IIC_HNDL  hndl, WV_U8 devAddr, WV_U8 regAddr, WV_U8 *value, WV_U32 count);

WV_S32 HIS_IIC_Init();
WV_S32 HIS_IIC_DeInit();


#endif

