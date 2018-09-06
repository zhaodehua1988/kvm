#include "kmv_serial.h"



#define KMV_V_SERIAL_PORT1  8001
#define KMV_V_SERIAL_PORT2  8002
#define KMV_V_SERIAL_PORT3  8003
#define KMV_V_SERIAL_PORT4  8004

#define KMV_V_SERIAL_DATA_MAX_LEN 1204



typedef struct KMV_V_SERIAL_DEV_E 
{
	pthread_t thrHndl; 
	unsigned int open;
	unsigned int close;
	unsigned int port;
	unsigned int chl;
	int mSocket;
	int cSocket;
	unsigned int recvEroNum;
	unsigned int timeoutNum;
	unsigned char *pBuf;
	unsigned char *pBufSend;
	unsigned int dataNum;
	struct sockaddr_in client_addr;
	socklen_t client_addr_length;
}KMV_V_SERIAL_DEV_E;

static KMV_V_SERIAL_DEV_E  *pSerialDev_1;
static KMV_V_SERIAL_DEV_E  *pSerialDev_2;
static KMV_V_SERIAL_DEV_E  *pSerialDev_3;
static KMV_V_SERIAL_DEV_E  *pSerialDev_4;

/********************************************************************

int  KMV_V_SERIAL_GetPort(KMV_V_SERIAL_DEV_E *pDev,int serialNum);

********************************************************************/
int KMV_V_SERIAL_GetPort(KMV_V_SERIAL_DEV_E *pDev,int serialNum)
{
	WV_U32 port;
	
	int ret = 0;
	switch(serialNum){
		case 1:
			SYS_ENV_GetU32("VSerialPort_1" ,&port);
			pDev->port = port;
			pDev->chl = 0;
			break;
		case 2:
			SYS_ENV_GetU32("VSerialPort_2" ,&port);
			pDev->port = port;
			pDev->chl = 1;
			break;
		case 3:
			SYS_ENV_GetU32("VSerialPort_3" ,&port);
			pDev->port = port;
			pDev->chl = 2;
			break;
		case 4:
			SYS_ENV_GetU32("VSerialPort_4" ,&port);
			pDev->port = port;
			pDev->chl = 3;
			break;
		default:
			ret = -1;
			break;
	}
	 
	return ret;
}


/****************************************************************************************

WV_S32 KMV_V_SERIAL_GetPortCmd(WV_U16 *pPort);

****************************************************************************************/
WV_S32 KMV_V_SERIAL_GetPortCmd(WV_U16 *pPort)
{
	WV_U32 port,w;
	SYS_ENV_GetU32("VSerialPort_1",&port);
	pPort[0] = port;
	SYS_ENV_GetU32("VSerialPort_2",&port);
	pPort[1] = port;
	SYS_ENV_GetU32("VSerialPort_3",&port);
	pPort[2] = port;
	SYS_ENV_GetU32("VSerialPort_4",&port);
	pPort[3] = port;
				
	return WV_SOK;
}

/********************************************************

int  KMV_V_SERIAL_ChangeBaudRate(unsigned int baudRate,int port)

********************************************************/ 
int  KMV_V_SERIAL_ChangeBaudRate(unsigned int baudRate,int chl)
{

	EXT_FPGA_ConfUart(chl,baudRate,0,1);
	//EXT_FPGA_ConfUart(WV_U8 chn,WV_U32 burd,WV_U32 check,WV_U32 stop)
	return 0;
}

/********************************************************

int  KMV_V_SERIAL_PcToFpga(int socket,unsigned char * pBuf)

********************************************************/ 
int KMV_V_SERIAL_PcToFpga(KMV_V_SERIAL_DEV_E *pDev)
{
	int recvLen;
	unsigned char * pRecvBuf;
	pRecvBuf = pDev->pBuf;
	unsigned int len;
	unsigned int baudRate=0;
	len = KMV_V_SERIAL_DATA_MAX_LEN;
	//recv don't wait 
	recvLen = recvfrom(pDev->mSocket,pRecvBuf,len,0,(struct sockaddr*)&pDev->client_addr, &pDev->client_addr_length);
	if(recvLen == 0)
	{
		return SERIAL_ERR;
	}
	//if recvLen > 0 ,send data to fpga	
/*
	if(recvLen == 8){

		if(pRecvBuf[0] == 0x55 && pRecvBuf[1] == 0xAA && pRecvBuf[2] == 0x55)
		{
			baudRate |= pRecvBuf[3] << 16;
			baudRate |= pRecvBuf[4] << 8;
			baudRate |= pRecvBuf[5];
			printf("recv baudRate change to [%d]\n",baudRate);
			KMV_V_SERIAL_ChangeBaudRate(baudRate,pDev->chl);
		}
	}
	else if(recvLen == 16)
	{
		if(pRecvBuf[8] == 0x55 && pRecvBuf[9] == 0xAA && pRecvBuf[10] == 0x55)
		{
			baudRate |= pRecvBuf[11] << 16;
			baudRate |= pRecvBuf[12] << 8;
			baudRate |= pRecvBuf[13];
			KMV_V_SERIAL_ChangeBaudRate(baudRate,pDev->chl);
			printf("recv chn[%d] baudRate change to [%d]\n",pDev->chl,baudRate);
		}
		
	}
*/
	if(recvLen > 0){
/*		int i;
		printf("\r\nrecv data:");	
		for(i=0;i<recvLen;i++)
		{
			printf("%02x ",pDev->pBuf[i]);	
		}
		printf("\r\n");	
		//send to fpga
		printf("send to fpga\n");
*/
		EXT_FPGA_SendUart(pDev->chl,pDev->pBuf, recvLen);
	}
  
	return SERIAL_OK;
}

/********************************************************

int  KMV_V_SERIAL_FpgaToPc(int socket,unsigned char * pBuf)

********************************************************/ 
int KMV_V_SERIAL_FpgaToPc(KMV_V_SERIAL_DEV_E *pDev)
{
	int ret = -1;
	int len,sendLen;
	memset(pDev->pBufSend,0,sizeof( KMV_V_SERIAL_DATA_MAX_LEN ));
	ret = 	EXT_FPGA_GetUart(pDev->chl, pDev->pBufSend, &len );
	if(ret != 0)
	{
		return 0;
	}
	if(pDev->mSocket < 0)
	{
		return 0;
	}
	if(	pDev->client_addr.sin_addr.s_addr == 0)
	{
		return 0;
	}
	
	sendLen = sendto(pDev->mSocket,pDev->pBufSend,len,0,(struct sockaddr*)&pDev->client_addr, sizeof(pDev->client_addr));
	/*	
	int i;
	printf("fpga to pc len= %d \n",sendLen);
	for(i=0;i<len;i++)
	{
		printf("%02x ",pDev->pBufSend[i]);
	}		 
	printf("\r\n");
	*/
	return SERIAL_OK;
}

/********************************************************************

int  KMV_V_SERIAL_Init(KMV_V_SERIAL_DEV_E *pDev);

********************************************************************/
int  KMV_V_SERIAL_Init(KMV_V_SERIAL_DEV_E *pDev)
{
	unsigned int reuse;
	socklen_t len;
	struct sockaddr_in addr;
	int status;

	//pDev -> mSocket = socket(AF_INET,SOCK_STREAM,0);
	pDev -> mSocket = socket(AF_INET,SOCK_DGRAM,0);//udp
	if (pDev -> mSocket < 0) 
	{ 
		KMV_SERIAL_error(" socket init error.\r\n");
		return  SERIAL_ERR; 
	}

	fcntl(pDev ->mSocket,F_SETFL,O_NONBLOCK);//no block
	reuse = 1;
	len = sizeof(reuse);
	setsockopt(pDev ->mSocket, SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,len);
	//get sen buff
	int   nSendBufLen;
	socklen_t optlen = sizeof(int); 
	nSendBufLen = 0;
	getsockopt(pDev->mSocket, SOL_SOCKET, SO_SNDBUF,(char*)&nSendBufLen, &optlen); 
	//KMV_SERIAL_printf("SERVER socket SNDBUF  = %d.\r\n",nSendBufLen);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(pDev ->port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	bzero(&(addr.sin_zero), 8);
	status  =  bind(pDev ->mSocket, (struct sockaddr *) &addr, sizeof(addr));
	if( status < 0 )
	{
		close(pDev->mSocket); 
		pDev ->mSocket = -1; 
		KMV_SERIAL_error(" socket bind  error = %d.\r\n",status);
		return SERIAL_ERR;
	} 
	KMV_SERIAL_printf("socket bind ok\n");
	return SERIAL_OK;
}

/********************************************************************

int  KMV_SERIAL_DeInit(KMV_V_SERIAL_DEV_E *pDev);

********************************************************************/
int KMV_SERIAL_DeInit(KMV_V_SERIAL_DEV_E *pDev)
{
	if(pDev -> cSocket >= 0)
	{
		close(pDev->cSocket);
		pDev->cSocket = -1; 
	}

	if(pDev -> mSocket >= 0)
	{
		close(pDev->mSocket);
		pDev->mSocket = -1; 
	}
	KMV_SERIAL_printf(" socket DeInit OK\r\n");   
	return  SERIAL_OK;

}

/********************************************************************

int  KMV_V_SERIAL_Proc(void *PDev);

********************************************************************/
void*  KMV_V_SERIAL_Proc(void *prm)
{
	KMV_V_SERIAL_DEV_E *pDev;
	int ret; 
	int   nSendBufLen;
	int flags;
	pDev = (KMV_V_SERIAL_DEV_E *) prm; 

	//
	if( KMV_V_SERIAL_Init(pDev) == SERIAL_ERR)
	{
		return NULL;
	} 

	pDev -> open   = 1;
	pDev -> close  = 0; 

	while(pDev -> open  == 1)
	{
		//data form  PC to DEV
		ret = KMV_V_SERIAL_PcToFpga(pDev);
		//data form DEV to PC
		ret = KMV_V_SERIAL_FpgaToPc(pDev);
		//usleep(1000); 	  
	}
	 
	KMV_SERIAL_DeInit(pDev); 
	pDev -> close  = 1;  	

	return SERIAL_OK;
}

/********************************************************************

int  KMV_V_UART_Open();

********************************************************************/
int  KMV_V_SERIAL_PortRegister()
{
	SYS_ENV_Register(1 ,"VSerialPort_1","8001","V serial port 1");
	SYS_ENV_Register(1 ,"VSerialPort_2","8002","V serial port 2");
	SYS_ENV_Register(1 ,"VSerialPort_3","8003","V serial port 3");
	SYS_ENV_Register(1 ,"VSerialPort_4","8004","V serial port 4");

	return SERIAL_OK;
}
/********************************************************************

int  KMV_V_UART_Open();

********************************************************************/
int  KMV_V_SERIAL_Open()
{

	KMV_V_SERIAL_PortRegister();
	pSerialDev_1 = (KMV_V_SERIAL_DEV_E *)malloc(sizeof(KMV_V_SERIAL_DEV_E)); 
	memset(pSerialDev_1,0,sizeof(KMV_V_SERIAL_DEV_E));
	pSerialDev_1->pBuf = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32);
	pSerialDev_1->pBufSend = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32);
	//get port
	KMV_V_SERIAL_GetPort(pSerialDev_1,1);
	pthread_create(&(pSerialDev_1->thrHndl),NULL,(void *)KMV_V_SERIAL_Proc,(void *)pSerialDev_1);


	pSerialDev_2 = (KMV_V_SERIAL_DEV_E *)malloc(sizeof(KMV_V_SERIAL_DEV_E)); 
	memset(pSerialDev_2,0,sizeof(KMV_V_SERIAL_DEV_E));
	pSerialDev_2->pBuf = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32);
	pSerialDev_2->pBufSend = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32);
	//get port
	KMV_V_SERIAL_GetPort(pSerialDev_2,2);
	pthread_create(&(pSerialDev_2->thrHndl),NULL,(void *)KMV_V_SERIAL_Proc,(void *)pSerialDev_2);


	pSerialDev_3 = (KMV_V_SERIAL_DEV_E *)malloc(sizeof(KMV_V_SERIAL_DEV_E)); 
	memset(pSerialDev_3,0,sizeof(KMV_V_SERIAL_DEV_E));
	pSerialDev_3->pBuf = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32); 
	pSerialDev_3->pBufSend = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32); 
	//get port
	KMV_V_SERIAL_GetPort(pSerialDev_3,3);
	pthread_create(&(pSerialDev_3->thrHndl),NULL,(void *)KMV_V_SERIAL_Proc,(void *)pSerialDev_3);


	pSerialDev_4 = (KMV_V_SERIAL_DEV_E *)malloc(sizeof(KMV_V_SERIAL_DEV_E)); 
	memset(pSerialDev_4,0,sizeof(KMV_V_SERIAL_DEV_E));
	pSerialDev_4->pBuf = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32);
	pSerialDev_4->pBufSend = (unsigned char *)malloc( KMV_V_SERIAL_DATA_MAX_LEN +32); 
	//get port
	KMV_V_SERIAL_GetPort(pSerialDev_4,4);
	pthread_create(&(pSerialDev_4->thrHndl),NULL,(void *)KMV_V_SERIAL_Proc,(void *)pSerialDev_4);



	return SERIAL_OK;
}

/********************************************************************

int  KMV_V_SERIAL_Close();

********************************************************************/
int  KMV_V_SERIAL_Close()
{
	
	if(pSerialDev_1->open == 1)
	{
		
		pSerialDev_1->open = 0;
		while(pSerialDev_1->close == 1) ;
		pthread_cancel(pSerialDev_1->thrHndl);
		pthread_join(pSerialDev_1->thrHndl,NULL);
		
	}
	free (pSerialDev_1 -> pBuf);
	free (pSerialDev_1 -> pBufSend);
	free (pSerialDev_1);
/******************************************/

	if(pSerialDev_2->open == 1)
	{
		
		pSerialDev_2->open = 0;
		while(pSerialDev_2->close == 1) ;
		pthread_cancel(pSerialDev_2->thrHndl);
		pthread_join(pSerialDev_2->thrHndl,NULL);
		
	}
	free (pSerialDev_2 -> pBuf);
	free (pSerialDev_2 -> pBufSend);
	free (pSerialDev_2);

/******************************************/
	if(pSerialDev_3->open == 1)
	{
		pSerialDev_3->open = 0;
		while(pSerialDev_3->close == 1) ;
		pthread_cancel(pSerialDev_3->thrHndl);
		pthread_join(pSerialDev_3->thrHndl,NULL);
		
	}
	free (pSerialDev_3 -> pBuf);
	free (pSerialDev_3 -> pBufSend);
	free (pSerialDev_3);
		
/******************************************/
	if(pSerialDev_4->open == 1)
	{
		pSerialDev_4->open = 0;
		while(pSerialDev_4->close == 1) ;
		pthread_cancel(pSerialDev_4->thrHndl);
		pthread_join(pSerialDev_4->thrHndl,NULL);
		
	}
	free (pSerialDev_4 -> pBufSend);
	free (pSerialDev_4 -> pBuf);
	free (pSerialDev_4);
				
	return SERIAL_OK;
}
