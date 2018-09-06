#include "kmv_usb.h"
#include "ext_fpga.h"
#include <sys/time.h>

#define KMV_MOUSE_DATA_LEN  5
#define KMV_MOUSE_PATH "/dev/input/"
#define KMV_MOUSE_FREQUENCY  5


typedef struct KMV_MOUSE_readBuf
{
	unsigned char data[4];
}KMV_MOUSE_readBuf;



/************************************************************

int KMV_MOUSE_change data formate2(KMV_MOUSE_DEV_E *pDev);

*************************************************************/
int KMV_MOUSE_ChangeDataFormate(unsigned char *pSrcBuf,unsigned char *pDesBuf)
{

	pDesBuf[0] = 0x04;	
	pDesBuf[1] = pSrcBuf[0] & 0x07; //bit7[0] bit6[0] bit5[0] bit4[0] bit3[0] bit2[middle] bit1[right] bit0[left] 
	pDesBuf[2] = pSrcBuf[1];
	pDesBuf[3] = 0x100 - pSrcBuf[2];

	pDesBuf[4] =0x100- pSrcBuf[3];

	return 0;
}

/************************************************************

int KMV_MOUSE_SendChl(int chn ,unsigned char *pBuf,int len);

*************************************************************/
int KMV_MOUSE_SendChl(int chn ,unsigned char *pBuf,int len,unsigned char devType)
{

	if(chn<4 &&  chn >=0)
	{			
		EXT_FPGA_SendKB(chn,pBuf,len ); 
		return 0;		
	}
	else if(chn = 4)
	{
		//send to remote dev
		KMV_UDP_ClientSend(pBuf,len,2,devType);
	}
	return 0;
}

/************************************************************

int KMV_MOUSE_send(KMV_MOUSE_DEV_E *pDev);

*************************************************************/
int KMV_MOUSE_Send(char chn,unsigned char *pBuf,int len,char ena,unsigned char devType)
{


#if 0
	int i=0;
	printf("send chl[%d]:",chn);
	for(i=0;i<len;i++){
		printf("%02x ",pBuf[i]);
	} 
	printf("\n"); 
#endif
	if(ena == 0 )
	{
		return 0;
	}	
 
	KMV_MOUSE_SendChl(chn ,pBuf,len,devType);
	return 0;
}

/************************************************************

int KMV_MOUSE_Proc(KMV_MOUSE_DEV_E *pDev);

*************************************************************/
int KMV_MOUSE_Proc(KMV_USB_DEVICE_INFO *pInfo){
	
	int readLen,ret;
	int mouse_fd;
	unsigned char mouse_parm[] = {243,200,243,100,243,80};
	//unsigned char mouse_parm[] = {243,200,243,100,243,80};
	unsigned char readBuf[4];
	unsigned char sendBuf[KMV_MOUSE_DATA_LEN];
	char mouse_path[40];

	sprintf(mouse_path,"%s%s",KMV_MOUSE_PATH,pInfo->handler);
	//KMV_USB_printf("mouse path %s\n",mouse_path);

		  
	ret = access(mouse_path,F_OK);
	if(ret == 0){
		KMV_USB_printf("access ok\n");	

	}else{
		KMV_USB_printf("access error \n");
		return -1;
	}
	mouse_fd = open(mouse_path,O_RDWR);
	if(mouse_fd <= 0)
	{
		KMV_USB_printf("open mouse dev error[%s]\n",mouse_path);
		return -1;	
	}else{
		KMV_USB_printf("open mouse dev ok[%s]\n",mouse_path);
	}

	//set mouse parameter
	write(mouse_fd,mouse_parm,sizeof(mouse_parm));
	pInfo->plug = 1;
	while(1)
	{
		
		readLen=read(mouse_fd,readBuf,sizeof(readBuf));
		//KMV_USB_printf("readLen = %d,readBuf = %d \n",readLen,sizeof(readBuf));

		if(readLen == -1){
			KMV_USB_printf("readLen = -1\n");
				break;
		}		

		if( readLen == sizeof(readBuf))
		{
			KMV_MOUSE_ChangeDataFormate(readBuf,sendBuf);
			KMV_MOUSE_Send(pInfo->chn,sendBuf,KMV_MOUSE_DATA_LEN,pInfo->ena,pInfo->devType);
				
		}
							
			

	}

	close(mouse_fd);

	//memset(pInfo,0,sizeof(KMV_USB_DEVICE_INFO));
	memset(pInfo->handler,0,sizeof(pInfo->handler));	
	pInfo->name = -1;
	pInfo->plug = 0;
	
	return 0;	
}



