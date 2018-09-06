#include"tsk_usb.h"

#define  TSK_USB_PATH            "/usb"
#define  TSK_USB_UPDATE_PATH     "/update"
#define  TSK_USB_MAIN_NAME       "hello"
#define  TSK_USB_DEV             "/dev/sda1"
  
typedef struct TSK_USB_DEV_E 
{
	WV_THR_HNDL_T thrHndl; 
	WV_U32      open;
	WV_U32      close; 
	WV_U32      plug;
	WV_U32      mount;  
	WV_U32      plugCnt;
}TSK_USB_DEV_E;

static TSK_USB_DEV_E gUsbDev;

/******************************************************************************

WV_S32 TSK_USB_Mount(TSK_USB_DEV_E  * pDev);
******************************************************************************/
WV_S32 TSK_USB_Mount(TSK_USB_DEV_E  * pDev)
{ 
  WV_S32  ret; 
  ret = WV_FILE_Access(TSK_USB_DEV);
		 if(ret == WV_SOK)
		 {
		   if(pDev->plug == 0)
		   {
		      pDev -> plug = 1;
		      WV_printf("usb disk plug in\r\n");  
		      ret = mount(TSK_USB_DEV, TSK_USB_PATH,"vfat",0,"codepage=936,iocharset=gb2312"); 
				  if(ret == 0)
				  {
					   pDev -> mount = 1;
					   pDev -> plugCnt++;
					   WV_printf("usb disk mount ok\r\n"); 
				     
				  }		       
				  else
				  {
				      ret = mount(TSK_USB_DEV,TSK_USB_PATH,"vfat",MS_REMOUNT,"codepage=936,iocharset=gb2312"); 
				       
				       if(ret == 0)
				       {
				        pDev -> mount = 1;
						pDev -> plugCnt++;
						WV_printf("usb disk re mount ok\r\n"); 
				       }
				       else
				       { 
				 	   WV_printf("usb disk mount fail\r\n");  
				 	   }
				  }
				if(pDev->mount == 1){
					//update
					TSK_UP_Update();
				}
		   } 
		 }
		 else
		 {
		    if(pDev -> mount == 1)
		    {
				  pDev -> plug = 0;
				  WV_printf("usb disk plug out\r\n"); 
				  //ret = umount2(TSK_USB_PATH,MNT_FORCE);
				  ret = umount2(TSK_USB_PATH,0);
					if(ret == 0)
					{
						pDev -> mount = 0;
						WV_printf("usb disk unmount ok\r\n"); 
					}       
					else
					{  
					    WV_printf("%d\r\n",errno);
					 	WV_printf("usb disk unmount fail\r\n");  
					}
		    }
		 } 
		 
   return WV_SOK;		 
}


/******************************************************************************

void *TSK_USB_Proc(Void * prm);
******************************************************************************/
void * TSK_USB_Proc ( void * prm)
{
 TSK_USB_DEV_E  * pDev;  
 pDev = (TSK_USB_DEV_E  *) prm;
 
 pDev-> open  = 1;
 pDev-> close  = 0;  
  //sleep(5);		
  while(pDev -> open == 1)
    {  
	  TSK_USB_Mount(pDev);	
      sleep(1);		 
    } 
    
 pDev-> open  = 0;
 pDev-> close = 1;
 return NULL;  
} 

/****************************************************************************************

WV_S32 TSK_USB_lsDir(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************************/
WV_S32 TSK_USB_GetUsb(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{ 
    DIR    *dir;
    WV_S8  dirPath[30];
     WV_S8 filePath[128];
    struct    dirent    *ptr; 
    struct stat statBuf;
    WV_S32 ret;
    if(argc <=  0)
	{
		prfBuff += sprintf(prfBuff,"get usb <cmd> \r\n");
		prfBuff += sprintf(prfBuff,"get usb ls  : get all file on udb disk \r\n");
		return WV_SOK;
	}
	
	// cmd ls
    if(0 == strcmp(argv[0],"ls"))
      
    {  
       prfBuff += sprintf(prfBuff,"plug   =  %d\r\n",gUsbDev.plug); 
       prfBuff += sprintf(prfBuff,"mount  =  %d\r\n",gUsbDev.mount);           
        
		if(gUsbDev.plug == 0)
		 {
		 prfBuff += sprintf(prfBuff,"usb not plug\r\n"); 
		 return 0;
		 }
		if(gUsbDev.mount == 0)
		 {
		 prfBuff += sprintf(prfBuff,"usb not mount\r\n"); 
		  return 0;
		 }
		
		if(argc < 1)
		 { 
			sprintf(dirPath,"%s",TSK_USB_PATH);
		 }
		 else
		 {
		    sprintf(dirPath,"%s/%s",TSK_USB_PATH,argv[1]);
		 } 
	    dir = opendir(dirPath); 
		 if(dir == NULL)
		 {
		   prfBuff += sprintf(prfBuff," %s not exist!!!\r\n",  dirPath); 
		   return WV_SOK;  
		 }	
	    prfBuff += sprintf(prfBuff,"FileName\r\n"); 
		while((ptr = readdir(dir)) != NULL)
		 {
		  
		  prfBuff += sprintf(prfBuff,"********************\r\n"); 
	      prfBuff += sprintf(prfBuff,"name     = %s\r\n", ptr->d_name); 
	      prfBuff += sprintf(prfBuff,"node num = %d\r\n", ptr->d_ino); 
	      prfBuff += sprintf(prfBuff,"node off = %d \r\n",ptr->d_off);
	      prfBuff += sprintf(prfBuff,"type     = %d \r\n",ptr->d_type); 
	      prfBuff += sprintf(prfBuff,"len      = %d \r\n",ptr->d_reclen ); 
	      sprintf(filePath,"%s/%s",dirPath,ptr->d_name);
	      ret = stat(filePath,&statBuf);
	      if(ret != 0)
	      { 
	      	prfBuff += sprintf(prfBuff,"    get %s stat erro !! \r\n",ptr->d_name); 
	      	continue; 
	      }
	       prfBuff += sprintf(prfBuff,"size   = %d \r\n",statBuf.st_size ); 
	      
	     }        
		closedir(dir);
    }
   // 
    
    return 0;

}

/****************************************************************************************

WV_S32 TSK_USB_GetLs(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************************/
WV_S32 TSK_USB_GetLs(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{ 
    DIR    *dir;
    WV_S8  dirPath[30];
     WV_S8 filePath[128];
    struct    dirent    *ptr; 
    struct stat statBuf;
    WV_S32 ret;
    sprintf(dirPath,"%s",TSK_USB_PATH);
	
	if(argc > 0) 
	{
	    sprintf(dirPath,"%s",argv[0]); 
	}	 
	// cmd ls
    if(0 == strcmp(dirPath,TSK_USB_PATH)) 
    {  
       prfBuff += sprintf(prfBuff,"plug   =  %d\r\n",gUsbDev.plug); 
       prfBuff += sprintf(prfBuff,"mount  =  %d\r\n",gUsbDev.mount);           
        
		if(gUsbDev.plug == 0)
		 {
		 prfBuff += sprintf(prfBuff,"usb not plug\r\n"); 
		 return 0;
		 }
		if(gUsbDev.mount == 0)
		 {
		 prfBuff += sprintf(prfBuff,"usb not mount\r\n"); 
		  return 0;
		 } 
	}
	
	    dir = opendir(dirPath); 
		 if(dir == NULL)
		 {
		   prfBuff += sprintf(prfBuff," %s not exist!!!\r\n",  dirPath); 
		   return WV_SOK;  
		  }	
	     prfBuff += sprintf(prfBuff,"File Info:\r\n"); 
		while((ptr = readdir(dir)) != NULL)
		 {
		 
	      sprintf(filePath,"%s/%s",dirPath,ptr->d_name);
	      ret = stat(filePath,&statBuf);
	      if(ret != 0)
	      { 
	      	prfBuff += sprintf(prfBuff,"    get %s stat erro !! \r\n",ptr->d_name); 
	      	continue; 
	      }
	       prfBuff += sprintf(prfBuff,"%40s %2d %8x %8d\r\n",ptr->d_name,ptr->d_type,statBuf.st_mode,statBuf.st_size ); 
	      
	     }        
		closedir(dir); 
   // 
    
    return 0;

}




/****************************************************************************************

WV_S32 TSK_USB_GetStat(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************************/
WV_S32 TSK_USB_GetStat(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{ 
    
     WV_S8  dirPath[30]; 
     struct statfs sfs; 
    WV_S32 ret,lenM;
    sprintf(dirPath,"%s", TSK_USB_PATH );
    
    if(argc > 0 )
	{
	  sprintf(dirPath,"%s", argv[0] );	 
	}
	prfBuff += sprintf(prfBuff,"get %s stat:\r\n",dirPath); 
	ret =  statfs(dirPath, &sfs);
	if(ret != 0)
	{
	prfBuff += sprintf(prfBuff,"get %s stat erro !!!\r\n",dirPath); 
	return 0;
	}
	
	prfBuff += sprintf(prfBuff,"block size : %d\r\n \r\n",sfs.f_bsize); 
	prfBuff += sprintf(prfBuff,"block all  : %d\r\n \r\n",sfs.f_blocks);
	prfBuff += sprintf(prfBuff,"block free : %d\r\n \r\n",sfs.f_bfree); 
	prfBuff += sprintf(prfBuff,"block avail : %d\r\n \r\n",sfs.f_bavail);
	
	WV_FILE_GetAvail(dirPath,&lenM);
	
	prfBuff += sprintf(prfBuff,"avail %d MB\r\n \r\n",lenM);
	
    return 0;

}



/****************************************************************************************

WV_S32 TSK_USB_GetCpy(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************************/
WV_S32 TSK_USB_GetCpy(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{  
    WV_S32 ret = 0; 
    if(argc <  2)
	{
		prfBuff += sprintf(prfBuff,"usb cpy <pathScr> <pathDst> \r\n"); 
		return WV_SOK;
	}
 
   ret = WV_FILE_Cpy(argv[0],argv[1]);
   if(ret == WV_SOK)
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s ok \r\n",argv[0],argv[1]);  
   }
   else
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s erro!!!! \r\n",argv[0],argv[1]);   
   }
    return WV_SOK;

}

/****************************************************************************************

WV_S32 TSK_USB_GetCpy(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************************/
WV_S32 TSK_USB_GetDel(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{  
    WV_S32 ret = 0; 
    if(argc <  1)
	{
		prfBuff += sprintf(prfBuff,"usb del  <file> \r\n"); 
		return WV_SOK;
	}
	ret = WV_FILE_Access(argv[0]);
	if(ret != WV_SOK)
	{
	   prfBuff += sprintf(prfBuff,"%s not exist !!! \r\n",argv[0]); 
	   return  WV_SOK;
    } 
    
   ret = WV_FILE_Remove(argv[0]);
   if(ret == WV_SOK)
   {
     prfBuff += sprintf(prfBuff,"del %s ok \r\n",argv[0]);  
   }
   else
   {
      prfBuff += sprintf(prfBuff,"del %s erro!!! \r\n",argv[0]); 
   }
    return 0;

}



/****************************************************************************************

WV_S32 TSK_USB_GetUpdate(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************************/
WV_S32 TSK_USB_GetUpdate(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{  
    WV_S32 ret = 0; 
    WV_S8 scrFile[64];
    WV_S8 desFile[64];
    WV_S8 backFile[64];
    
    sprintf(scrFile,"%s%s/%s",TSK_USB_PATH,TSK_USB_UPDATE_PATH,TSK_USB_MAIN_NAME); 
    sprintf(backFile,"./%s_b",TSK_USB_MAIN_NAME);
     sprintf(desFile,"./%s",TSK_USB_MAIN_NAME); 
    
    if(argc > 1)
	{
		sprintf(scrFile,"%s%s%s",TSK_USB_PATH,TSK_USB_UPDATE_PATH,argv[0]);  
	}
	ret = WV_FILE_Access(scrFile);
	if(ret != WV_SOK)
	{
	   prfBuff += sprintf(prfBuff,"%s file not exist !!! \r\n",scrFile); 
	   return  WV_SOK;
    }
    
  ret = WV_FILE_Access(desFile);
	if(ret != WV_SOK)
	{
	   prfBuff += sprintf(prfBuff,"%s file not exist !!! \r\n",desFile); 
	   return  WV_SOK;
    }
     
    // back fiele
   ret = WV_FILE_Cpy(desFile,backFile);
   if(ret == WV_SOK)
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s ok \r\n",desFile,backFile);  
   }
   else
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s erro !!!\r\n",desFile,backFile); 
      return  WV_SOK; 
   }
   
   //update file
   ret = WV_FILE_Cpy(scrFile,desFile);
   if(ret == WV_SOK)
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s ok \r\n",scrFile,desFile);  
   }
   else
   { 
     prfBuff += sprintf(prfBuff,"cpy %s to  %s erro !!!\r\n",desFile,backFile);   
   }
    
   //
   if(ret == WV_SOK)
   {
    prfBuff += sprintf(prfBuff,"update ok\r\n");  
    return WV_SOK;  
   }
   
   //recover old file
    ret = WV_FILE_Cpy(backFile,desFile);
   if(ret == WV_SOK)
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s ok \r\n",backFile,desFile);  
   }
   else
   {
     prfBuff += sprintf(prfBuff,"cpy %s to  %s erro !!!\r\n",backFile,desFile); 
    
   } 
     prfBuff += sprintf(prfBuff,"recovery erro !!!\r\n"); 
    return 0;

}
 


/****************************************************************************************

WV_S32 TSK_USB_Open();

****************************************************************************************/

WV_S32 TSK_USB_Open()
{ 
	WV_S32 ret = 0; 
	WV_CMD_Register("get","usb","get usb info",TSK_USB_GetUsb);
	WV_CMD_Register("usb","ls","list dir file",TSK_USB_GetLs);
	WV_CMD_Register("usb","cpy","cpy file",TSK_USB_GetCpy);
	WV_CMD_Register("usb","del","remove file",TSK_USB_GetDel);
	WV_CMD_Register("usb","update","update file",TSK_USB_GetUpdate);

	memset(&gUsbDev,0,sizeof(gUsbDev));
	ret = WV_FILE_Access(TSK_USB_PATH);
	 if(ret != WV_SOK)
	{
		 
		   mkdir(TSK_USB_PATH,0755); 
		   WV_printf("creat usb dir\r\n");
	} 
	TSK_USB_Mount(&gUsbDev);
	WV_RET_ADD( WV_THR_Create(&gUsbDev.thrHndl, TSK_USB_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gUsbDev),ret);
 
	return ret;  
}

/****************************************************************************************

WV_S32  TSK_USB_Close();

****************************************************************************************/

WV_S32  TSK_USB_Close()
{ 

	WV_S32 ret = 0;
   if(gUsbDev.open == 1)
   {
   	  gUsbDev.open = 0;
   	  while(gUsbDev.close == 1) ;
      WV_CHECK_RET( WV_THR_Destroy(&gUsbDev.thrHndl));	
   } 
 return ret;  
}

