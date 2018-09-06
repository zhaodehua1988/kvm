#ifndef _WV_HIS_GPIO_H__
#define _WV_HIS_GPIO_H__
#include "wv_common.h" 
#define HIS_GPIO_MODE_INPUT     1
#define HIS_GPIO_MODE_OUTPUT    0

typedef struct HIS_GPIO_DEV_E
{ 
   WV_U32 bank;
   WV_U32 num;   
} HIS_GPIO_DEV_E;

 
WV_S32  HIS_GPIO_Init();
WV_S32  HIS_GPIO_DeInit(); 
WV_S32  HIS_GPIO_SetCfg(HIS_GPIO_DEV_E  pin,WV_U32  mode);
WV_S32  HIS_GPIO_Set(HIS_GPIO_DEV_E  pin,WV_U32 value); 
WV_S32  HIS_GPIO_Get(HIS_GPIO_DEV_E  pin,WV_U32 *pValue);
#endif
