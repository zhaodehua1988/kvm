#include "kmv_svr_udp.h"
#include "sys_env.h"
#include "kmv_usb.h"
#include "ext.h"
#define KMV_SVR_UDP_DATA_MAX_LEN 127

#define KMV_UDP_PORT 9001
#define KMV_UDP_REMOTE_IP "192.168.1.133"

#if 1


typedef struct KMV_SVR_UDP_DATA_H
{
	unsigned char sync[4];
	unsigned char cmd;//0:return val 1:sw 2:mouse 3:keyBd
	unsigned char ret;         //0: ok ,else: faile
	unsigned char len;
	unsigned char level;
	unsigned char devType; //0 vga, 1 dvi
	unsigned char keyValue[32];


}KMV_SVR_UDP_DATA_H;

typedef struct KMV_SVR_UDP_KEY_E
{
	pthread_t thrHndl; 
	unsigned int      open;
	unsigned int      close; 
	unsigned char free;
	unsigned char localChl;
	unsigned char level;//init level = 0
	struct sockaddr_in sAddr;
	struct sockaddr_in cAddr;

	int sock;
	//unsigned char buf[KMV_SVR_UDP_DATA_MAX_LEN];
	//KMV_SVR_UDP_DATA_H *pData;
	
}KMV_SVR_UDP_KEY_E;

KMV_SVR_UDP_KEY_E gUdpRecvDev;// for server recv
KMV_SVR_UDP_KEY_E gUdpSendDev;// for send to remote server
/**************************************************************

int KMV_UDP_ServerInit();

**************************************************************/
int KMV_UDP_ServerInit()
{
	printf("udp  recv init\n"); 	
	memset((char *)&gUdpRecvDev,0,sizeof(KMV_SVR_UDP_KEY_E));

	if((gUdpRecvDev.sock = socket(PF_INET, SOCK_DGRAM,0)) < 0)
	{
		KMV_SVR_error("udp server socket error\n");
		return -1;	
	}

	struct sockaddr_in addr;	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(KMV_UDP_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(gUdpRecvDev.sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_in)) < 0 )
	{	
		KMV_SVR_error("udp server socket bind error\n");
		return -1;
	}


	return 0;
}

/**************************************************************

int KMV_UDP_GetSwChl(KMV_SVR_UDP_DATA_H *pData)

**************************************************************/
int KMV_UDP_GetSwChl(KMV_SVR_UDP_DATA_H *pData)
{
	int chn;
	chn = pData->keyValue[0];
	KMV_USB_DEV_changeRemoteChn(pData->devType,chn,0);
	switch(pData->devType)
	{
		case 0:
			EXT_VGA_SetSw(4,chn,1);
			printf("vga set sw out[4] in[%d] \n",chn);
			break;
		case 1:
			EXT_DVI_SetSw(4,chn,1);
			printf("dvi set sw out[4] in[%d] \n",chn);
			break;

		default:
			break;
	}
	return 0;
}
/**************************************************************

int KMV_UDP_GetMouse(KMV_SVR_UDP_DATA_H *pData)

**************************************************************/
int KMV_UDP_GetMouse(KMV_SVR_UDP_DATA_H *pData)
{
	int chn;
	chn = KMV_USB_DEV_getRemoteChn(pData->devType);
	EXT_FPGA_SendKB(chn,pData->keyValue,pData->len);

	return 0;
}

/**************************************************************

int KMV_UDP_GetMouse(KMV_SVR_UDP_DATA_H *pData)

**************************************************************/
int KMV_UDP_GetKb(KMV_SVR_UDP_DATA_H *pData)
{
	int chn;
	
	chn = KMV_USB_DEV_getRemoteChn(pData->devType);

	EXT_FPGA_SendKB(chn,pData->keyValue,pData->len );
	return 0;
}

/**************************************************************

int KMV_UDP_GetCmd(KMV_SVR_UDP_DATA_H *pData)

**************************************************************/
int KMV_UDP_GetCmd(KMV_SVR_UDP_DATA_H *pData)
{
	
	switch(pData->cmd)
	{
		case 0:
			
			break;
		case 1:
			KMV_UDP_GetSwChl(pData);
			break;
		case 2:
			KMV_UDP_GetMouse(pData);
			break;
		case 3:
			KMV_UDP_GetKb(pData);
			break;

		default:
			break;	
			
	}
	return 0;
	
}

/**************************************************************

int KMV_UDP_ServerRecv();

**************************************************************/
void* KMV_UDP_ServerRecv()
{

	int recvLen;
	socklen_t peerlen = sizeof(struct sockaddr_in);
	gUdpRecvDev.open =1;
	gUdpRecvDev.close =0;
	KMV_SVR_UDP_DATA_H data;
	while(gUdpRecvDev.open == 1)
	{
		memset(&data,0,sizeof(KMV_SVR_UDP_DATA_H));
		recvLen=recvfrom(gUdpRecvDev.sock,(char *)&data,sizeof(KMV_SVR_UDP_DATA_H),0,(struct sockaddr *)&gUdpRecvDev.cAddr,&peerlen);
		
		if(recvLen <=0)
		{
			KMV_SVR_error("server recv error,recvLen=[%d]\n",recvLen);
			continue;
		}
		
		KMV_UDP_GetCmd((KMV_SVR_UDP_DATA_H *)(&data));
		
		
	}
	gUdpRecvDev.close = 1;
	
	return NULL;
	
}



/*******************************client send************************/

	                	
/**************************************************************

int KMV_UDP_ClientInit();

**************************************************************/
int KMV_UDP_ClientInit()
{
	

	memset((char *)&gUdpSendDev,0,sizeof(KMV_SVR_UDP_KEY_E));
	
	if((gUdpSendDev.sock = socket(AF_INET, SOCK_DGRAM,0)) < 0)
	{
		printf("udp server socket error\n");
		return -1;	
	}
	
	char ip[20];
	SYS_ENV_Get("Venc_0_ip",ip);
	KMV_UDP_ClientGetAddr(ip,9001);
	gUdpSendDev.open = 1;
	return 0;
}

/**************************************************************

int KMV_UDP_ClientGetAddr(char *ip,int port);

**************************************************************/
int KMV_UDP_ClientGetAddr(char *ip,int port)
{
	gUdpSendDev.sAddr.sin_family = AF_INET;
	gUdpSendDev.sAddr.sin_port = htons(port);
	gUdpSendDev.sAddr.sin_addr.s_addr = inet_addr(ip);
	gUdpSendDev.open = 1;
	return 0;
}

/**************************************************************

int KMV_UDP_ClientSetRemoteChl(int chl,int level)

**************************************************************/
/*
int KMV_UDP_ClientSetRemoteChl(int chl,int level)
{

	

	if((level >= gUdpSendDev.level) ) 
		return 0;

	gUdpSendDev.level = level;
	//set remote chl
	
	return 0;
}
*/
/**************************************************************

int KMV_UDP_ClientSend(char *buf,int len,unsigned char cmd,unsigned char devType);

**************************************************************/
int KMV_UDP_ClientSend(char *buf,int len,unsigned char cmd,unsigned char devType)
{
	if(	gUdpSendDev.open != 1)
	{
		printf("udp client  is not open  \n");
		return 0;
	}
		
	int i;
	KMV_SVR_UDP_DATA_H data;
	memset(&data,0,sizeof(data));
	data.cmd = cmd;
	for(i=0;i<4;i++)
	{
		data.sync[i] = 0x5a;
	}
	data.len = len;
	memcpy(data.keyValue,buf,len);
	data.devType = devType;
	//printf("client send key\n");
	sendto(gUdpSendDev.sock, (char *)&data,sizeof(KMV_SVR_UDP_DATA_H),0,(struct sockaddr *)&gUdpSendDev.sAddr,sizeof(gUdpSendDev.sAddr));
	return 0;
}

/**************************************************************

int KMV_UDP_ClientDeInit();

**************************************************************/
int KMV_UDP_ClientDeInit()
{
	gUdpSendDev.open = 0;
	return 0;
}


/**************************************************************

int KMV_UDP_Server_Open();

**************************************************************/
int KMV_UDP_Server_Open()
{
	KMV_UDP_ServerInit();
	
	KMV_UDP_ClientInit();

	pthread_create(&(gUdpRecvDev.thrHndl),NULL,(void *)KMV_UDP_ServerRecv,NULL);

	return 0;
}

/**************************************************************

int KMV_UDP_Server_Close();

**************************************************************/
int KMV_UDP_Server_Close()
{
	if(gUdpRecvDev.open == 1)
	{
		gUdpRecvDev.open = 0;
		pthread_cancel(gUdpRecvDev.thrHndl);
		pthread_join(gUdpRecvDev.thrHndl,NULL);   
	}

	KMV_UDP_ClientDeInit();
	return 0;
	
}

#endif











