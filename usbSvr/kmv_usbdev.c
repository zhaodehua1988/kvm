#include "kmv_usb.h"

#define KMV_USB_DEV_PATH  "/dev/input/"
#define KMV_USB_DEV_INFO_PATH "/proc/bus/input/devices"


#define KMV_USB_KEYBD_NAME "Keyboard"
#define KMV_USB_MOUSE_NAME "MOUSE"


#define KMV_USB_KEYBD_NAME_ID 1
#define KMV_USB_MOUSE_NAME_ID 2
#define KMV_USB_DEV_INFO_MAX_BUF 1024*1024

#define KMV_USB_DEV_INFO_MAX_NUM 8
#define KMV_USB_DEV_TYPE_VGA  0
#define KMV_USB_DEV_TYPE_DVI  1


typedef struct KMV_USB_DEV_E 
{
	pthread_t thrHndl; 
	unsigned int open;
	unsigned int close; 
	unsigned int plug; 
	unsigned int plugCnt;
	KMV_USB_DEVICE_INFO usbDevInfo[KMV_USB_DEV_INFO_MAX_NUM ];
}KMV_USB_DEV_E;

//local usb port dev 
#if 1
static KMV_USB_DEV_E  gUsbDev_1;
static KMV_USB_DEV_E  gUsbDev_2;
static KMV_USB_DEV_E  gUsbDev_3;
static KMV_USB_DEV_E  gUsbDev_4;
static KMV_USB_DEV_E  gUsbDev_5;
static KMV_USB_DEV_E  gUsbDev_6;
static KMV_USB_DEV_E  gUsbDev_7;
static KMV_USB_DEV_E  gUsbDev_8;
#endif
//usb hub dev 
#if 0
static KMV_USB_DEV_E  gUsbDevHub1[4];
static KMV_USB_DEV_E  gUsbDevHub2[4];
static KMV_USB_DEV_E  gUsbDevHub3[4];
static KMV_USB_DEV_E  gUsbDevHub4[4];
static KMV_USB_DEV_E  gUsbDevHub5[4];
static KMV_USB_DEV_E  gUsbDevHub6[4];
static KMV_USB_DEV_E  gUsbDevHub7[4];
static KMV_USB_DEV_E  gUsbDevHub8[4];
#endif
static int usb_RemoteChnVga;
static int usb_RemoteChnDvi;

/************************************************************

int KMV_USB_DEV_changeRemoteChn(int devType,int chl,int level);

*************************************************************/
int KMV_USB_DEV_changeRemoteChn(int devType,int chl,int level)
{
	switch(devType)
	{
		case KMV_USB_DEV_TYPE_VGA:
			usb_RemoteChnVga = chl;
			printf("change vga remote mouse to [%d]\n",usb_RemoteChnVga);
			break;
		case KMV_USB_DEV_TYPE_DVI:
			usb_RemoteChnDvi = chl;
			printf("change dvi remote mouse to [%d]\n",usb_RemoteChnDvi);
			break;
		default:
			break;		
	}

	return 0;
}
/************************************************************

int KMV_USB_DEV_getRemoteChn(int devType);

*************************************************************/
int KMV_USB_DEV_getRemoteChn(int devType)
{
	
	if(devType == 0)
	{
		return usb_RemoteChnVga;
	}
	
	if(devType == 1)
	{
		return usb_RemoteChnDvi;
	}
	
	return 0;
}

/************************************************************

int KMV_USB_DEV_ChangeLevel(unsigned char chnIn,unsigned char chnOut,unsigned char ena);

*************************************************************/
/*
int KMV_USB_DEV_ChangeLevel(int chl,int level)
{
	if(level <0 || level >5) 
		return -1;
	
	switch(chl)
	{

		case 1:
			gUsbDev_1.usbDevInfo.level = level;
			gUsbDev_2.usbDevInfo.level = level;

			break;
		case 2:
			gUsbDev_3.usbDevInfo.level = level;
			gUsbDev_4.usbDevInfo.level = level;

			break;
		case 3:
			gUsbDev_5.usbDevInfo.level = level;
			gUsbDev_6.usbDevInfo.level = level;

			break;
		case 4:
			gUsbDev_7.usbDevInfo.level = level;
			gUsbDev_8.usbDevInfo.level = level;
			break;	
		default:
			break;
	}

	return 0;
	
}
*/
/************************************************************

int KMV_USB_DEV_ChlSw(unsigned char chnIn,unsigned char chnOut,unsigned char ena);

*************************************************************/

int KMV_USB_DEV_ChlSw(unsigned char outChn,unsigned char inChn,unsigned char ena,unsigned char devType)
{
	WV_S32 ret = 0;
	WV_S32 i;
	
	printf("mouse sw in[%d]out[%d]ena[%d]type[%d]\n",inChn,outChn,ena,devType);
	if(devType == KMV_USB_DEV_TYPE_VGA)
	{
	
		switch(outChn)
		{
			case 0:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					//gUsbDev_1.usbDevInfo[i].chn = inChn ;
					//gUsbDev_1.usbDevInfo[i].ena = ena & 0x01;
					gUsbDev_2.usbDevInfo[i].chn = inChn;
					gUsbDev_2.usbDevInfo[i].ena = ena & 0x01;
				}
				printf("mouse change %d to %d \n",inChn,outChn);
				break; 
			case 1:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					//gUsbDev_3.usbDevInfo[i].chn = inChn ;
					//gUsbDev_3.usbDevInfo[i].ena = ena & 0x01;
					gUsbDev_4.usbDevInfo[i].chn = inChn;
					gUsbDev_4.usbDevInfo[i].ena = ena & 0x01;
				}
				break;
			case 2:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					//gUsbDev_5.usbDevInfo[i].chn = inChn ;
					//gUsbDev_5.usbDevInfo[i].ena = ena & 0x01;
					gUsbDev_6.usbDevInfo[i].chn = inChn;
					gUsbDev_6.usbDevInfo[i].ena = ena & 0x01;
				}
				break;
			case 3:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					//gUsbDev_7.usbDevInfo[i].chn = inChn ;
					//gUsbDev_7.usbDevInfo[i].ena = ena & 0x01;
					gUsbDev_8.usbDevInfo[i].chn = inChn;
					gUsbDev_8.usbDevInfo[i].ena = ena & 0x01;
				}
				break;
			default:

				ret = -1;
				break;
	 	
		}
	}

	if(devType == KMV_USB_DEV_TYPE_DVI)
	{
	
		switch(outChn)
		{
			case 0:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					gUsbDev_1.usbDevInfo[i].chn = inChn ;
					gUsbDev_1.usbDevInfo[i].ena = ena & 0x01;
					//gUsbDev_2.usbDevInfo[i].chn = inChn;
					//gUsbDev_2.usbDevInfo[i].ena = ena & 0x01;
				}
				break; 
			case 1:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					gUsbDev_3.usbDevInfo[i].chn = inChn ;
					gUsbDev_3.usbDevInfo[i].ena = ena & 0x01;
					//gUsbDev_4.usbDevInfo[i].chn = inChn;
					//gUsbDev_4.usbDevInfo[i].ena = ena & 0x01;
				}
				break;
			case 2:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					gUsbDev_5.usbDevInfo[i].chn = inChn ;
					gUsbDev_5.usbDevInfo[i].ena = ena & 0x01;
					//gUsbDev_6.usbDevInfo[i].chn = inChn;
					//gUsbDev_6.usbDevInfo[i].ena = ena & 0x01;
				}
				break;
			case 3:
				for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
				{
					gUsbDev_7.usbDevInfo[i].chn = inChn ;
					gUsbDev_7.usbDevInfo[i].ena = ena & 0x01;
					//gUsbDev_8.usbDevInfo[i].chn = inChn;
					//gUsbDev_8.usbDevInfo[i].ena = ena & 0x01;
				}
				break;
			default:

				ret = -1;
				break;
	 	
		}
	}	
	return ret;
}

/************************************************************

int KMV_USB_DEV_SetGroupChl(char *pDevName,char chn);

*************************************************************/
int KMV_USB_DEV_SetGroupChl(char *pDevName,char chn)
{

	WV_S32 ret,i;	
	if(strncmp(pDevName, gUsbDev_1.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){	
		if(EXT_DVI_SetSw(0,chn,1) == 0)
		{
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				gUsbDev_1.usbDevInfo[i].chn = chn;
				gUsbDev_1.usbDevInfo[i].ena = 1;	

			}
		}
	}else if(strncmp(pDevName, gUsbDev_2.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){
		if(EXT_VGA_SetSw(0, chn,1) == 0)
		{
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{

				gUsbDev_2.usbDevInfo[i].chn = chn;
				gUsbDev_2.usbDevInfo[i].ena = 1;
			
			}
  		}
	}else if(strncmp(pDevName, gUsbDev_3.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){
		if(EXT_DVI_SetSw(1,chn,1) == 0)
		{
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				gUsbDev_3.usbDevInfo[i].chn = chn;
				gUsbDev_3.usbDevInfo[i].ena = 1;
				 
			}
		}
	}else if(strncmp(pDevName, gUsbDev_4.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){
	
		if(EXT_VGA_SetSw(1, chn,1) == 0)
		{
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				gUsbDev_4.usbDevInfo[i].chn = chn;
				gUsbDev_4.usbDevInfo[i].ena = 1;
			
			}
		}
	
	}else if(strncmp(pDevName, gUsbDev_5.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){

		if(EXT_DVI_SetSw(2,chn,1) == 0)
		{
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				gUsbDev_5.usbDevInfo[i].chn = chn;
				gUsbDev_5.usbDevInfo[i].ena = 1;
			
			}
		}
	
	}else if(strncmp(pDevName, gUsbDev_6.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){
		if(EXT_VGA_SetSw(2, chn,1) == 0)
		{
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				gUsbDev_6.usbDevInfo[i].chn = chn;
				gUsbDev_6.usbDevInfo[i].ena = 1;
				  
			}
		}
	}else if(strncmp(pDevName, gUsbDev_7.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){

		if(EXT_DVI_SetSw(3,chn,1) == 0)
		{ 
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{	
				gUsbDev_7.usbDevInfo[i].chn = chn;
				gUsbDev_7.usbDevInfo[i].ena = 1;
			
			}
		}
	}else if(strncmp(pDevName, gUsbDev_8.usbDevInfo[0].physDev,KMV_USB_DEVICE_DATAMAX_LEN) == 0){

		if(EXT_VGA_SetSw(3, chn,1) == 0)
		{		
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				gUsbDev_8.usbDevInfo[i].chn = chn;
				gUsbDev_8.usbDevInfo[i].ena = 1;

			}
		}
	}

	return 0;
}



/************************************************************

int KMV_USB_DEV_SetChl(char chn,unsigned char *pBuf,int len);

*************************************************************/
int KMV_USB_DEV_SetChl(KMV_USB_DEVICE_INFO *pInfo,unsigned char *pBuf,int len)
{

	char buf[2];//buf[0] = chn,buf[1]==0(vga),buf[1]=1 dvi

	if(len == 4)
	{
		if((pBuf[0] == 0x83) && (pBuf[2] == 0x00) && ((pBuf[1] == 0x01) || (pBuf[1] == 0x10)))
		{

			switch(pBuf[3])
			{
				case 0x3a://ctrl + F1
					KMV_KEYBD_PopAllKey(pInfo);
					KMV_USB_DEV_SetGroupChl(pInfo->physDev,0);

					KMV_USB_printf("change chn to %d\n",pInfo->chn);
					break;
				case 0x3b://ctrl + F2
					KMV_KEYBD_PopAllKey(pInfo);
					KMV_USB_DEV_SetGroupChl(pInfo->physDev,1);
					KMV_USB_printf("change chn to 1\n");
					break;
				case 0x3c://ctrl + F3
					KMV_KEYBD_PopAllKey(pInfo);
					KMV_USB_DEV_SetGroupChl(pInfo->physDev,2);
					KMV_USB_printf("change chn to 2 \n");
					break;
				case 0x3d://ctrl + F4
					KMV_KEYBD_PopAllKey(pInfo);
					KMV_USB_DEV_SetGroupChl(pInfo->physDev,3);
					KMV_USB_printf("change chn to 3\n");
					break;
				case 0x3e://ctrl +F5
					//KMV_KEYBD_PopAllKey(pInfo);
					//chang remote chl
					buf[0] = 0;
					KMV_UDP_ClientSend(buf,1,1,pInfo->devType);//KMV_UDP_ClientSend(char *buf,int len,unsigned char cmd)
					//set local chl
					KMV_USB_DEV_SetGroupChl(pInfo->physDev,4);
					KMV_USB_printf("change chn to remote 0\n");
					break;
				case 0x3f://ctrl+F6
					//KMV_KEYBD_PopAllKey(pInfo);
					buf[0] = 1;
					KMV_UDP_ClientSend(buf,1,1,pInfo->devType);
					KMV_USB_DEV_SetGroupChl(pInfo->physDev,4);
					KMV_USB_printf("change chn to remote 1\n");
					break;
				case 0x40://ctrl+F7
					//KMV_KEYBD_PopAllKey(pInfo);
					buf[0] = 2;
					KMV_UDP_ClientSend(buf,1,1,pInfo->devType);

					KMV_USB_DEV_SetGroupChl(pInfo->physDev,4);
					KMV_USB_printf("change chn to remote 2\n");
					break;
				case 0x41://ctrl+F8
					//_KEYBD_PopAllKey(pInfo);
					buf[0] = 3;
					KMV_UDP_ClientSend(buf,1,1,pInfo->devType);

					KMV_USB_DEV_SetGroupChl(pInfo->physDev,4);
					KMV_USB_printf("change chn to remote 3\n");
					break;
				default :
					break;					
			}
	
		
		}
	}
	

	return 0;
}

/************************************************************

int KMV_USB_DEV_GetDevName(const char *pLocal,const char *pDes);

*************************************************************/
char * KMV_USB_DEV_GetDevName(const char*pLocal,const char *pDes)
{

	return strstr(pLocal,pDes);
}

/************************************************************

int KMV_USB_DEV_GetInfo2(KMV_USB_DEVICE_INFO *pInfo);

*************************************************************/
int KMV_USB_DEV_GetInfo2(KMV_USB_DEVICE_INFO *pInfo)
{
	FILE *fp;
	char buf[1024];
	int state = 0,i,j;
	fp=fopen(KMV_USB_DEV_INFO_PATH,"r");
	if(fp==NULL)
	{
		
		return -1;
	}
	//pInfo->name = -1;
	while(fgets(buf,1024,fp))
	{
		//printf("buf=[%s]\n",buf);
		//compare usbDev physId
#if 1
		state = 0;
		if(buf[0] == 'P')
		{

			
			for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
			{
				//printf("info[%d],name=[%d],handler=[%s],plug=[%d]\n",i,pInfo[i].name,pInfo[i].handler,pInfo[i].plug);
				if(pInfo[i].name == -1)
					break;
			}

			if(i==KMV_USB_DEV_INFO_MAX_NUM && pInfo[i].name != -1)
			{
				break;
			}

			if(strncmp(&buf[8],pInfo[i].physDev,20) == 0 )
			{
				//KMV_USB_printf("dev name %s\n",buf);
				state = 1;
			}
			
		}
		//get handlers
		if(state == 1){
			
			while(fgets(buf,1024,fp)){
				//printf("111buf=[%s],len %d \n",buf,strlen(buf)-17);
				
					if(buf[0] == 'H'){

						  if(strncmp(&buf[12],"kbd",3) == 0){
								//printf("get kbd [%s],pInfo[%d].name=[%d]\n",buf,i,pInfo[i].name);
								pInfo[i].name = KMV_USB_KEYBD_NAME_ID;
								
								for(j=0;j<KMV_USB_DEV_INFO_MAX_NUM;j++)
								{
									if(strncmp(&buf[16],pInfo[j].handler,strlen(buf)-18) == 0 )
									{
										pInfo[i].name = -1;
										//KMV_USB_printf("buf[%s]there is have the handler [%s],Info[%d].hand,Info[%d].name=[%d]\n",buf,pInfo[j].handler,j,i,pInfo[i].name);
										
										//goto end;
										break;
									}
								}
								
								if(pInfo[i].name == -1)
								{
									//printf("*********there is have the handler [%s],Info[%d].hand\n",pInfo[j].handler,j);
									break;
								}
								memcpy(pInfo[i].handler,&buf[16],strlen(buf)-18);
								//KMV_USB_printf("2222222keyBd get pInfo[%d].handle:%s,pInfo[%d].name=[%d]\n",i,pInfo[i].handler,i,pInfo[i].name);
								
							}else if(strncmp(&buf[12],"mouse",5) == 0){
								//printf("get mouse [%s],pInfo[%d].name=[%d]\n",buf,i,pInfo[i].name);
								pInfo[i].name = KMV_USB_MOUSE_NAME_ID;
								for(j=0;j<KMV_USB_DEV_INFO_MAX_NUM;j++)
								{
									if(strncmp(&buf[12],pInfo[j].handler,6) == 0 )
									{
										pInfo[i].name = -1;
										//KMV_USB_printf("buf[%s]there is have the handler [%s],Info[%d].hand,Info[%d].name=[%d]\n",buf,pInfo[j].handler,j,i,pInfo[i].name);
										//goto end;
										break;
									}
								}
								
								if(pInfo[i].name == -1)
								{
									//printf("*********there is have the handler [%s],Info[%d].hand\n",pInfo[j].handler,j);
									break;
								}
								memcpy(pInfo[i].handler,&buf[12],6);
								//KMV_USB_printf("111111mouse get pInfo[%d].handle:%s,pInfo[%d].name=[%d]\n",i,pInfo[i].handler,i,pInfo[i].name);
						}//end  if(strncmp(&buf[12],"kbd",3)
				
					//continue;
				}//end if(buf[0] == 'H')
							
				if(pInfo[i].name != -1){
					if(strncmp(buf,"B: EV=",6) == 0)
					{
					
						if(strncmp(&buf[6],"17",2) == 0)
						{
							//printf("B: EV=17\n");
							pInfo[i].name = KMV_USB_MOUSE_NAME_ID;
							//printf("pInfo[%d].name=mouse,B: EV=17\n",i);
						}else if(strncmp(&buf[6],"120013",6) == 0){
							//printf("pInfo[%d].name=kbd,B: EV=120013\n",i);
							pInfo[i].name = KMV_USB_KEYBD_NAME_ID;
						}else{
					
							pInfo[i].name = -1;
							//printf("[%s]pInfo[%d].handler clean,pInfo[%d].name=[%d] \n",buf,i,i,pInfo[i].name);
							memset(pInfo[i].handler,0,sizeof(pInfo[i].handler));
						}
						//printf("go out state = 1\n");
						//break;								
					}//end if(strncmp(buf,"B: EV=",6) == 0)
					
					break;
				}
				continue;
			}//end while(fgets(buf,1024,fp))			
					
		}
#endif
	}//end while(1)

	fclose(fp);
	return 0;

}


/************************************************************

void * KMV_USB_GET_Proc(void *prm);

*************************************************************/
void * KMV_USB_GET_Proc(void *prm)
{

	pthread_detach(pthread_self());
	KMV_USB_DEVICE_INFO *pInfo;
	pInfo = (KMV_USB_DEVICE_INFO *)prm;
	if(pInfo->name == KMV_USB_KEYBD_NAME_ID){
		 KMV_KEYBD_Proc(pInfo);
	}else if(pInfo->name == KMV_USB_MOUSE_NAME_ID){
		KMV_MOUSE_Proc(pInfo);
	
	}
}

/************************************************************

void * KMV_USB_DEV_Proc(void *prm);

*************************************************************/
void * KMV_USB_DEV_Proc(void *prm)
{

	int ret = 0,i;
	KMV_USB_DEV_E  * pDev;  
	KMV_USB_DEVICE_INFO *pInfo;
	pDev = (KMV_USB_DEV_E  *) prm;
	pInfo = &pDev->usbDevInfo;
	pDev-> open  = 1;
	pDev-> close  = 0;
	//sleep(5);		
	while(pDev->open == 1)
	{  
		//printf("\n--------------------------------------------------------------\n");
		ret = KMV_USB_DEV_GetInfo2(pInfo);
	
		if(ret != 0 )
		{
			sleep(1);
			continue;

		}

		for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
		{
			if(pInfo[i].name != -1 && pInfo[i].plug == 0)
			{	
				//creat thread
				//KMV_USB_printf("creat dev:%s,info[%d]->name=[%d]\n",pInfo[i].handler,i,pInfo[i].name);
				pthread_create(&(pInfo[i].thrHndl),NULL,(void *)KMV_USB_GET_Proc,(void *)&pInfo[i]);
				//KMV_USB_printf("dev:%s,info[%d]->name=[%d]\n",pInfo[i].handler,i,pInfo[i].name);

			}
		}
	   sleep(1);		 
	} 

	pDev-> open  = 0;
	pDev-> close = 1;
	return NULL;

}
/************************************************************

int KMV_USB_DEV_CreatProc(KMV_USB_DEV_E *pDev,WV_U32 chn,WV_U32 ena,const WV_S8 *pPhyName,WV_U8 devType);

*************************************************************/
int KMV_USB_DEV_CreatProc(KMV_USB_DEV_E *pDev,WV_U32 chn,WV_U32 ena,const WV_S8 *pPhyName,WV_U8 devType)//devType=0 vga 1 dvi
{
	memset((WV_S8 *)pDev,0,sizeof(KMV_USB_DEV_E));
	int i;
	for(i=0;i<KMV_USB_DEV_INFO_MAX_NUM;i++)
	{
		strcpy(pDev->usbDevInfo[i].physDev,pPhyName);
		pDev->usbDevInfo[i].chn = chn;
		pDev->usbDevInfo[i].ena = ena;
		pDev->usbDevInfo[i].level = 2;
		pDev->usbDevInfo[i].devType = devType & 0x1;
		pDev->usbDevInfo[i].name = -1;
		pDev->usbDevInfo[i].plug = 0;

	}
	pthread_create(&(pDev->thrHndl),NULL,(void *)KMV_USB_DEV_Proc,(void *)pDev);

	return 0;
}

/************************************************************

int KMV_USB_DEV_DestroyProc(KMV_USB_DEV_E *pDev);

*************************************************************/
int KMV_USB_DEV_DestroyProc(KMV_USB_DEV_E *pDev)
{
	if(pDev->open == 1)
   {
   	  pDev->open = 0;

      pthread_cancel(pDev->thrHndl);
	  pthread_join(pDev->thrHndl,NULL);
   }

	return 0;
}

/************************************************************

int KMV_USB_DEV_Open();

*************************************************************/
int KMV_USB_DEV_Open()
{
	
	usb_RemoteChnVga = 0;
	usb_RemoteChnDvi = 0;

	KMV_USB_DEV_CreatProc(&gUsbDev_1,0,1,KMV_USB_HUB_PHYS1,1);//1 dvi 0 vga

	KMV_USB_DEV_CreatProc(&gUsbDev_2,0,1,KMV_USB_HUB_PHYS2,0);
	KMV_USB_DEV_CreatProc(&gUsbDev_3,1,1,KMV_USB_HUB_PHYS3,1);
	KMV_USB_DEV_CreatProc(&gUsbDev_4,1,1,KMV_USB_HUB_PHYS4,0);
	KMV_USB_DEV_CreatProc(&gUsbDev_5,2,1,KMV_USB_HUB_PHYS5,1);
	KMV_USB_DEV_CreatProc(&gUsbDev_6,2,1,KMV_USB_HUB_PHYS6,0);
	KMV_USB_DEV_CreatProc(&gUsbDev_7,3,1,KMV_USB_HUB_PHYS7,1);
	KMV_USB_DEV_CreatProc(&gUsbDev_8,3,1,KMV_USB_HUB_PHYS8,0);

/*
	//usb hub
	KMV_USB_DEV_CreatProc(&gUsbDevHub1[0],0,1,KMV_USB_HUB_PHYS1_1,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub1[1],0,1,KMV_USB_HUB_PHYS1_2,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub1[2],0,1,KMV_USB_HUB_PHYS1_3,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub1[3],0,1,KMV_USB_HUB_PHYS1_4,1);

	KMV_USB_DEV_CreatProc(&gUsbDevHub2[0],0,1,KMV_USB_HUB_PHYS2_1,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub2[1],0,1,KMV_USB_HUB_PHYS2_2,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub2[2],0,1,KMV_USB_HUB_PHYS2_3,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub2[3],0,1,KMV_USB_HUB_PHYS2_4,0);

	KMV_USB_DEV_CreatProc(&gUsbDevHub3[0],1,1,KMV_USB_HUB_PHYS3_1,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub3[1],1,1,KMV_USB_HUB_PHYS3_2,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub3[2],1,1,KMV_USB_HUB_PHYS3_3,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub3[3],1,1,KMV_USB_HUB_PHYS3_4,1);

	KMV_USB_DEV_CreatProc(&gUsbDevHub4[0],1,1,KMV_USB_HUB_PHYS4_1,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub4[1],1,1,KMV_USB_HUB_PHYS4_2,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub4[2],1,1,KMV_USB_HUB_PHYS4_3,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub4[3],1,1,KMV_USB_HUB_PHYS4_4,0);

	KMV_USB_DEV_CreatProc(&gUsbDevHub5[0],2,1,KMV_USB_HUB_PHYS5_1,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub5[1],2,1,KMV_USB_HUB_PHYS5_2,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub5[2],2,1,KMV_USB_HUB_PHYS5_3,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub5[3],2,1,KMV_USB_HUB_PHYS5_4,1);

	KMV_USB_DEV_CreatProc(&gUsbDevHub6[0],2,1,KMV_USB_HUB_PHYS6_1,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub6[1],2,1,KMV_USB_HUB_PHYS6_2,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub6[2],2,1,KMV_USB_HUB_PHYS6_3,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub6[3],2,1,KMV_USB_HUB_PHYS6_4,0);

	KMV_USB_DEV_CreatProc(&gUsbDevHub7[0],3,1,KMV_USB_HUB_PHYS7_1,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub7[1],3,1,KMV_USB_HUB_PHYS7_2,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub7[2],3,1,KMV_USB_HUB_PHYS7_3,1);
	KMV_USB_DEV_CreatProc(&gUsbDevHub7[3],3,1,KMV_USB_HUB_PHYS7_4,1);

	KMV_USB_DEV_CreatProc(&gUsbDevHub8[0],3,1,KMV_USB_HUB_PHYS8_1,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub8[1],3,1,KMV_USB_HUB_PHYS8_2,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub8[2],3,1,KMV_USB_HUB_PHYS8_3,0);
	KMV_USB_DEV_CreatProc(&gUsbDevHub8[3],3,1,KMV_USB_HUB_PHYS8_4,0);

*/
	return 0;
}

/********************************************************************

int  KMV_KEYBD_Close();

********************************************************************/
int  KMV_USB_DEV_Close()
{

	WV_S32 i;

	KMV_USB_DEV_DestroyProc(&gUsbDev_1);

	KMV_USB_DEV_DestroyProc(&gUsbDev_2);
	KMV_USB_DEV_DestroyProc(&gUsbDev_3);
	KMV_USB_DEV_DestroyProc(&gUsbDev_4);
	KMV_USB_DEV_DestroyProc(&gUsbDev_5);
	KMV_USB_DEV_DestroyProc(&gUsbDev_6);
	KMV_USB_DEV_DestroyProc(&gUsbDev_7);
	KMV_USB_DEV_DestroyProc(&gUsbDev_8);

/*
	//close usb hub
	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub1[i]);
	}

	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub2[i]);
	}

	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub3[i]);
	}

	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub4[i]);
	}


	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub5[i]);
	}

	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub6[i]);
	}

	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub7[i]);
	}


	for(i=0;i<4;i++)
	{
		KMV_USB_DEV_DestroyProc(&gUsbDevHub8[i]);
	}

*/
   return 0; 
}




