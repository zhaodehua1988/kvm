#ifndef _EXT_VGA_AD9984_H__
#define _EXT_VGA_AD9984_H__
#include "sys.h" 
WV_S32  EXT_AD9984_Init();
WV_S32  EXT_AD9984_DeInit(); 
WV_S32  EXT_AD9984_Detec();
WV_S32  EXT_AD9984_GetInfo(WV_U32 *width,WV_U32 *height,WV_U32 * frameRate);
#endif
