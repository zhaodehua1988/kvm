#include "sys.h"



/*******************************************************************
 WV_S32 SYS_Init();
*******************************************************************/
 WV_S32 SYS_Init() 
{
    WV_ASSERT_RET(WV_CMD_Create());
	WV_ASSERT_RET(SYS_ENV_Open());
	WV_ASSERT_RET(SYS_IP_Init());  
	WV_ASSERT_RET(WV_TTY_Create()); 
	WV_ASSERT_RET(WV_TEL_Open()); 

 return WV_SOK;
}


/*******************************************************************
 WV_S32 SYS_DeInit();
*******************************************************************/
 WV_S32 SYS_DeInit()
{
	WV_ASSERT_RET(WV_TEL_Close());
	WV_ASSERT_RET(WV_TTY_Destroy());
	WV_ASSERT_RET(SYS_IP_DeInit()); 
	WV_ASSERT_RET(SYS_ENV_Close()); 
	WV_ASSERT_RET(WV_CMD_Destroy()); 
 return WV_SOK;
}
