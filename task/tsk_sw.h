#ifndef TSK_SW_H_H_
#define TSK_SW_H_H_
#include "wv_common.h"
#include "ext_dvi.h"
#include "ext_vga.h"
#include "kmv_usb.h"

WV_S32 TSK_SW_GetSW(WV_U8 *pDviSw,WV_U8*pVgaSw);
WV_S32 TSK_SW_Init();
WV_S32 TSK_SW_DeInit();
#endif
