#ifndef _EXT_FPGA_PROC_H__
#define _EXT_FPGA_PROC_H__
#include "sys.h"
WV_S32  EXT_FPGA_Init();
WV_S32  EXT_FPGA_DeInit(); 
WV_S32  EXT_FPGA_SendUart(WV_U8  chn, WV_U8 *pBuf, WV_U32 len );
WV_S32  EXT_FPGA_GetUart(WV_U8  chn, WV_U8 *pBuf, WV_U32 *len ); 
WV_S32  EXT_FPGA_SendKB(WV_U8  chn, WV_U8 *pBuf, WV_U32 len ); 
WV_S32 EXT_FPGA_GetResolution(WV_U8  chn, WV_U32 *pWidth, WV_U32 *pHeight,WV_U32 *pMode);
WV_S32  EXT_FPGA_SetADV3226(WV_U8 *pBuf,WV_U32 len);
WV_S32  EXT_FPGA_ConfUart(WV_U8 chn,WV_U32 burd,WV_U32 check,WV_U32 stop);  

WV_S32 EXT_FPGA_GetVgaHnum(WV_U32 *nClkNum);
WV_S32 EXT_FPGA_Write(WV_U16 addr,WV_U16 data);
#endif
