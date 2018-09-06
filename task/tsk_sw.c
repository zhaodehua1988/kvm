#include "tsk_sw.h"


typedef struct TSK_SW_CONF_E
{
	WV_U8 vgaSw[5];//ctrl channel
	WV_U8 dviSw[5];//ctrl level
}TSK_SW_CONF_E;

TSK_SW_CONF_E gSwDev;


/***********************************************************

WV_S32 TSK_SW_GetConf()

************************************************************/
WV_S32 TSK_SW_GetConf()
{

	
	return 0;

}
/***********************************************************

WV_S32 TSK_SW_GetSW(WV_U8 *pDviSw,WV_U8*pVgaSw)

************************************************************/
WV_S32 TSK_SW_GetSW(WV_U8 *pDviSw,WV_U8*pVgaSw)
{
	WV_S32 i;

	

	EXT_DVI_GetSw(pDviSw);
	EXT_VGA_GetSw(pVgaSw);
	for(i=0;i<5;i++)
	{
		gSwDev.vgaSw[i] = pVgaSw[i];
		gSwDev.dviSw[i] = pDviSw[i];			
	}


	return 0;
}
/***********************************************************

WV_S32 TSK_SW_GetSW(WV_U8 *pDviSw,WV_U8*pVgaSw)

************************************************************/
WV_S32 TSK_SW_SetSW()
{
	
	return 0;
}
/***********************************************************

WV_S32 TSK_SW_Init()

************************************************************/
WV_S32 TSK_SW_Init()
{   

	memset(&gSwDev ,0 ,sizeof(gSwDev));
		
	TSK_SW_GetConf();
	TSK_SW_SetSW();
	
	return WV_SOK;
	
}
/***********************************************************

WV_S32 TSK_SW_DeInit()

************************************************************/
WV_S32 TSK_SW_DeInit()
{

	return WV_SOK;
	
}



