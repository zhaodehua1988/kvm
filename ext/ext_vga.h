#ifndef _EXT_VGA_INOUT_H__
#define _EXT_VGA_INOUT_H__
#include "sys.h"
WV_S32  EXT_VGA_Init();
WV_S32  EXT_VGA_DeInit(); 
WV_S32  EXT_VGA_SetSw(WV_U8  outChn, WV_U8  inChn, WV_U8 ena ); 
#endif
