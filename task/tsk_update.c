#include"tsk_update.h"
#include<stdio.h>
#define  TSK_UP_PATH_NAME	 "/usb/update/"	
#define  TSK_UP_FILE_NAME	 "up.ini"
#define  TSK_UP_MAIN_NAME    "hello"
		
#define  BENCHMARK_YEAR         2016
#define  TSK_UP_PWD            "swkj2017"   
#define  TSK_CP_FILE_NUM	20
#if 1
/******************************************************************************
WV_S32 TSK_UP_UpdateInfoCheck();
******************************************************************************/
WV_S32 TSK_UP_UpdateInfoCheck()
{
	WV_S32 ret;
	WV_S8 upFile[20];
	sprintf(upFile,"%s%s",TSK_UP_PATH_NAME,TSK_UP_FILE_NAME);
	ret = WV_FILE_Access(upFile);
	if(ret != WV_SOK)
	{
		printf("access %s error \n",upFile);
		return WV_EFAIL;
	}
	return WV_SOK;
}


/******************************************************************************
WV_S32 TSK_UP_UpdateStart();
******************************************************************************/
WV_S32 TSK_UP_UpDateStart()
{
	WV_S32 ret = 0; 
	WV_S8 scrFile[64];
	WV_S8 desFile[64];
	WV_S8 backFile[64];

	sprintf(scrFile,"%s%s",TSK_UP_PATH_NAME,TSK_UP_MAIN_NAME); 
	sprintf(backFile,"./%s_b",TSK_UP_MAIN_NAME);
	sprintf(desFile,"./%s",TSK_UP_MAIN_NAME); 


	ret = WV_FILE_Access(scrFile);
	if(ret != WV_SOK)
	{
		WV_printf("%s file is not exit\n",scrFile);
		return  WV_EFAIL;
	}

	ret = WV_FILE_Access(desFile);
	if(ret != WV_SOK)
	{
		WV_printf("%s file not exist !!! \r\n",desFile); 
		return  WV_SOK;
	}

	// back fiele
	ret = WV_FILE_Cpy(desFile,backFile);
	if(ret == WV_SOK)
	{
		WV_printf("cpy %s to  %s ok \r\n",desFile,backFile);  
	}
	else
	{
		WV_printf("cpy %s to  %s erro !!!\r\n",desFile,backFile); 
	return  WV_SOK; 
	}

	//update file
	ret = WV_FILE_Cpy(scrFile,desFile);
	if(ret == WV_SOK)
	{
		WV_printf("cpy %s to  %s ok \r\n",scrFile,desFile);  
	}
	else
	{ 
		WV_printf("cpy %s to  %s erro !!!\r\n",desFile,backFile);   
	}

	//
	if(ret == WV_SOK)
	{
		WV_printf("update ok\r\n");  
		return WV_SOK;  
	}

	//recover old file
	ret = WV_FILE_Cpy(backFile,desFile);
	if(ret != WV_SOK)
	{
		WV_printf("cpy %s to  %s erro !! \r\n",backFile,desFile);  
		return WV_EFAIL;
	}

	WV_printf("cpy %s to  %s erro !!!\r\n",backFile,desFile); 

	return WV_SOK;
	
}

/******************************************************************************
WV_S32 TSK_UP_UpdateEnd();
******************************************************************************/
WV_S32 TSK_UP_UpdateEnd()
{
	
	WV_S8 upFile[20];
	WV_S32 ret;
	sprintf(upFile,"%s%s",TSK_UP_PATH_NAME,TSK_UP_FILE_NAME);
	ret = WV_FILE_Access(upFile);
	if(ret != WV_SOK)
	{
		return WV_SOK;
	}	

	ret = remove(upFile);
	
	if(ret != WV_SOK )
	{
		WV_printf("rm file [%s] error !\n",upFile);
	}
	return ret;
		
}

/******************************************************************************
WV_S32 TSK_UP_Update();
******************************************************************************/
WV_S32 TSK_UP_Update(){


	if(TSK_UP_UpdateInfoCheck() == 0){
		TSK_UP_UpDateStart();
		TSK_UP_UpdateEnd();
		//reboot
		WV_CMD_GetReset(); 	
	}
	return WV_SOK;
}

#endif

