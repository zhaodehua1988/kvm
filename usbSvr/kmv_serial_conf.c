#include "kmv_serial.h"

#define KMV_SERIAL_CONF_PORT 6100
#define KMV_SERIAL_CONF_INFO_MAX_LEN 128

typedef struct KMV_SERIAL_CONF_E 
{
	pthread_t thrHndl; 
	unsigned int open;
	unsigned int close;
	int mSocket;
	int cSocket;
	unsigned int recvEroNum;
	unsigned int timeoutNum;
	char *pBuf;
	unsigned int dataNum;
}KMV_SERIAL_CONF_E;



static KMV_SERIAL_CONF_INFO vSerialInfo[4];
static KMV_SERIAL_CONF_E gSerialConfDev;

/********************************************************

int  KMV_SERIAL_CONF_atoi(char *pRecv,unsigned int ** data)

********************************************************/ 
int  KMV_SERIAL_CONF_atoi(char *pRecv,unsigned int recvLen,unsigned int * data,unsigned int *pDataLen)
{

	int i=0,j=0;
	unsigned int temp=0;
	for(i=0; i<recvLen; i++)
	{
		if(pRecv[i] >= '0' && pRecv[i] <= '9'){

			temp=temp*10 + (pRecv[i] - '0');

		}else if(pRecv[i] == ';'){

			data[j] = temp;
			temp =0;
			j++;
		}else if(pRecv[i] == '\0'){

			KMV_SERIAL_printf("recv data change to int end \n");
			break;		
		}
	}
	*pDataLen = j;
	//printf("\nrecv char : %s \n",pRecv);
/*	
	KMV_SERIAL_printf("recv atoi :");	
	for(i=0;i<j;i++)
	{
		printf("[%d] ",data[i]);
	}
	printf("\n");
*/
	return 0;	
}


/********************************************************

int  KMV_SERIAL_CONF_SetBaudRate(unsigned int baud)

********************************************************/ 
int  KMV_SERIAL_CONF_SetBaudRate(int *update,int num,unsigned int data)//only support 9600 19200 38400 57600 115200
{
	switch(data)
	{
		case 0:
			//9600
			if(vSerialInfo[num].baudRate != 9600)
			{
				vSerialInfo[num].baudRate = 9600;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d] baudrate=[9600]\n",vSerialInfo[num].port);
			break;			
		case 1:
			//none
			KMV_SERIAL_printf("change port[%d] baudrate=[no change]\n",vSerialInfo[num].port);
			break;
		case 2:
			//19200
			if(vSerialInfo[num].baudRate != 19200)
			{
				vSerialInfo[num].baudRate = 19200;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d] baudrate=[19200]\n",vSerialInfo[num].port);
			break;
		case 3:
			//38400
			if(vSerialInfo[num].baudRate != 38400)
			{
				vSerialInfo[num].baudRate = 38400;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d] baudrate=[38400]\n",vSerialInfo[num].port);
			break;
		case 4:
			//57600
			if(vSerialInfo[num].baudRate != 57600)
			{
				vSerialInfo[num].baudRate = 57600;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d] baudrate=[57600]\n",vSerialInfo[num].port);
			break;
		case 5:
			//115200
			if(vSerialInfo[num].baudRate != 115200)
			{
				vSerialInfo[num].baudRate = 115200;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d] baudrate=[115200]\n",vSerialInfo[num].port);
			break;
		case 7:
			//start or stop
			KMV_SERIAL_printf("start or stop serial dev,port[%d]\n",vSerialInfo[num].port);
			return -1;
		default:
			//9600
			if(vSerialInfo[num].baudRate != 9600)
			{
				vSerialInfo[num].baudRate = 9600;
				*update+=1;
			}
			KMV_SERIAL_printf("change port [%d] default baudrate=[9600]\n",vSerialInfo[num].port);
			break;
	}
	return 0;	
}

/********************************************************

int  KMV_SERIAL_CONF_SetCheckBit(int *update,unsigned int data)

********************************************************/ 
int  KMV_SERIAL_CONF_SetCheckBit(int *update,int num,unsigned int data)
{
	switch(data){
		
		case 0:
			//check Bit :none
			if(vSerialInfo[num].checkBit != 0 )
			{
				vSerialInfo[num].checkBit = 0;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  checkBit=[none]\n",vSerialInfo[num].port); 
			break;
		case 1:
			//check Bit :odd
			if(vSerialInfo[num].checkBit != 1 )
			{
				vSerialInfo[num].checkBit = 1;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  checkBit=[odd]\n",vSerialInfo[num].port); 
			break;
		case 2:
			//check Bit :even
			if(vSerialInfo[num].checkBit != 2 )
			{
				vSerialInfo[num].checkBit = 2;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  checkBit=[even]\n",vSerialInfo[num].port); 
			break;
		case 3:
			//check Bit :space	
			if(vSerialInfo[num].checkBit != 3 )
			{
				vSerialInfo[num].checkBit = 3;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  checkBit=[space]\n",vSerialInfo[num].port); 
			break;
		case 4:
			//check Bit :make
			if(vSerialInfo[num].checkBit != 4 )
			{
				vSerialInfo[num].checkBit = 4;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  checkBit=[make]\n",vSerialInfo[num].port); 
			break;
		default:
			if(vSerialInfo[num].checkBit != 0 )
			{
				vSerialInfo[num].checkBit = 0;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  checkBit error ,not sport\n",vSerialInfo[num].port); 
			break;
	}
	return 0;	
}
/********************************************************

int  KMV_SERIAL_CONF_SetDataBit(unsigned int data)

********************************************************/ 
int  KMV_SERIAL_CONF_SetDataBit(int *update,int num,unsigned int data)
{
	switch(data){
		case 0:
			//data bit:8
			if(vSerialInfo[num].dataBit != 8 )
			{
				vSerialInfo[num].dataBit = 8;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  dataBit=[8]\n",vSerialInfo[num].port);
			break;
		case 1:
			//data bit:7
			if(vSerialInfo[num].dataBit != 7 )
			{
				vSerialInfo[num].dataBit = 7;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  dataBit=[7]\n",vSerialInfo[num].port);
			break;
		case 2:
			//data bit:6
			if(vSerialInfo[num].dataBit != 6 )
			{
				vSerialInfo[num].dataBit = 6;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  dataBit=[6]\n",vSerialInfo[num].port);
			break;			
		default:
			//data bit:default 8
			if(vSerialInfo[num].dataBit != 8 )
			{
				vSerialInfo[num].dataBit = 8;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  dataBit change default=[8]\n",vSerialInfo[num].port);
			break;
	}
	return 0;	
}
/********************************************************

int  KMV_SERIAL_CONF_SetStopBit(int *update,unsigned int data)

********************************************************/ 
int  KMV_SERIAL_CONF_SetStopBit(int *update,int num,unsigned int data)
{
	switch(data)
	{
		case 0:
			//stop bit : 1
			if(vSerialInfo[num].stopBit != 1 )
			{
				vSerialInfo[num].stopBit = 1;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  stopBit=[1]\n",vSerialInfo[num].port);
			break;
		case 1:
			//stop bit : 2
			if(vSerialInfo[num].stopBit != 2 )
			{
				vSerialInfo[num].stopBit = 2;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  stopBit=[2]\n",vSerialInfo[num].port);	
			break;
		default:
			//stop bit : default 1
			if(vSerialInfo[num].stopBit != 1 )
			{
				vSerialInfo[num].stopBit = 1;
				*update+=1;
			}
			KMV_SERIAL_printf("change port[%d]  stopBit default =[1]\n",vSerialInfo[num].port);
			break;	
	}
	return 0;	
}


/********************************************************

int  KMV_SERIAL_CONF_GetInfo(KMV_SERIAL_CONF_INFO *pInfo);

********************************************************/ 
int  KMV_SERIAL_CONF_GetInfo(KMV_SERIAL_CONF_INFO **pInfo)
{

	int i;
	for(i=0;i<4;i++)
	{
		pInfo[i]->port = vSerialInfo[i].port;
		pInfo[i]->baudRate = vSerialInfo[i].baudRate;
		pInfo[i]->checkBit = vSerialInfo[i].checkBit;
		pInfo[i]->dataBit = vSerialInfo[i].dataBit;
		pInfo[i]->stopBit = vSerialInfo[i].stopBit;
	}
	
	return 0;
}

/********************************************************

int  KMV_SERIAL_CONF_Info_Init()

********************************************************/ 
int  KMV_SERIAL_CONF_Info_init()
{

	int i;

	vSerialInfo[0].port = 8001;
	vSerialInfo[1].port = 8002;
	vSerialInfo[2].port = 8003;
	vSerialInfo[3].port = 8004;

	for(i=0;i<4;i++)
	{

		vSerialInfo[i].baudRate = 9600;
		vSerialInfo[i].checkBit = 0;
		vSerialInfo[i].dataBit = 8;
		vSerialInfo[i].stopBit = 1;
	}

	return 0;
}


/********************************************************

int  KMV_SERIAL_CONF_SetInfo(char *pBuf)

********************************************************/ 
int  KMV_SERIAL_CONF_SetInfo(char *pRecv,unsigned int recvLen)
{
	int i,num;
	int update=0;
	unsigned int dataBuf[32];
	unsigned int dataLen=0;
	KMV_SERIAL_CONF_atoi(pRecv,recvLen,dataBuf,&dataLen);
	if(dataLen != 7)
	{
		KMV_SERIAL_error("error get conf info ,dataLen is %d\n",dataLen);
		return -1;
	}
	if(dataBuf[0] != 0 || dataBuf[1] != 10)
	{
		KMV_SERIAL_error("error get conf info ,buf[0]=%d ,buf[1]=%d\n",dataBuf[0],dataBuf[1]);
	}
	//get port 	
	for(i=0;i<4;i++)
	{
		if(dataBuf[2] == vSerialInfo[i].port)
		{
			num = i;
			break;
		}
	}

	if(i==4){

		KMV_SERIAL_printf("\nerror get client port,clientPort=%d, \n",dataBuf[2]);		
		return -1;
	}

	KMV_SERIAL_CONF_SetBaudRate(&update, num, dataBuf[3]);
	KMV_SERIAL_CONF_SetCheckBit(&update,num, dataBuf[4]);
	KMV_SERIAL_CONF_SetDataBit(&update, num,dataBuf[5]);
	KMV_SERIAL_CONF_SetStopBit(&update, num,dataBuf[6]);

	KMV_SERIAL_printf("update is %d\n",update);
	if(update >0)
	{
		EXT_FPGA_ConfUart(num,vSerialInfo[num].baudRate,vSerialInfo[num].checkBit,vSerialInfo[i].stopBit);  
		KMV_SERIAL_printf("update serial conf info\n");
	}	
	return 0;
}

/********************************************************

int  KMV_SERIAL_CONF_Recv(KMV_SERIAL_CONF_E *pDev)

********************************************************/ 
int  KMV_SERIAL_CONF_Recv(KMV_SERIAL_CONF_E *pDev)
{

	int recvLen,ret;
	char * pRecvBuf;
	pRecvBuf = pDev->pBuf;
	
	memset(pRecvBuf,0,KMV_SERIAL_CONF_INFO_MAX_LEN);

	unsigned int len;
	len = KMV_SERIAL_CONF_INFO_MAX_LEN;
	
	recvLen = recv(pDev->cSocket,pRecvBuf,len,0);
	KMV_SERIAL_printf("recvLen = [%d]\n",recvLen);	
	if(recvLen > 0)
	{
		KMV_SERIAL_printf("recv serial conf info : %s\n",pDev->pBuf);
		KMV_SERIAL_CONF_SetInfo(pDev->pBuf,recvLen);
		ret = SERIAL_OK;
					
	}else if(recvLen ==0){
		
		KMV_SERIAL_printf("serial conf cnnect loss!!!!!!\n");
		ret = SERIAL_ERR;
	}else{
		KMV_SERIAL_error("serial conf recv error !!!!!!\n");
		ret = SERIAL_ERR;
	}
   
	return ret;
}

/********************************************************************

int  KMV_SERIAL_CONF_Init(KMV_SERIAL_CONF_E *pDev)

********************************************************************/
int  KMV_SERIAL_CONF_Init(KMV_SERIAL_CONF_E *pDev)
{
	unsigned int reuse;
	socklen_t len;
	struct sockaddr_in addr;
	int status;

	pDev -> mSocket = socket(AF_INET,SOCK_STREAM,0);
	if (pDev -> mSocket < 0) 
	{ 
		KMV_SERIAL_error(" socket init error.\r\n");
		return  SERIAL_ERR; 
	}

	reuse = 1;
	len = sizeof(reuse);
	setsockopt(pDev ->mSocket, SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,len);
	
	//get sen buff
	int   nSendBufLen;
	socklen_t optlen = sizeof(int); 
	nSendBufLen = 0;
	getsockopt(pDev->mSocket, SOL_SOCKET, SO_SNDBUF,(char*)&nSendBufLen, &optlen); 
	// KMV_SERIAL_printf("SERVER socket SNDBUF  = %d.\r\n",nSendBufLen);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(KMV_SERIAL_CONF_PORT);
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
	KMV_SERIAL_printf("socket get serial conf proc  bind ok\n");
	return SERIAL_OK;
}
/********************************************************************

int KMV_SERIAL_CONF_DeInit(KMV_SERIAL_CONF_E *pDev);

********************************************************************/
int KMV_SERIAL_CONF_DeInit(KMV_SERIAL_CONF_E *pDev)
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
	KMV_SERIAL_printf(" serial conf  DeInit OK\r\n");   
	return  SERIAL_OK;
}
/********************************************************************

void*  KMV_SERIAL_CONF_Proc(void *prm)

********************************************************************/
void*  KMV_SERIAL_CONF_Proc(void *prm)
{
	KMV_SERIAL_CONF_E *pDev;
	int ret; 
	int   nSendBufLen;

	pDev = (KMV_SERIAL_CONF_E *) prm; 

	//
	if( KMV_SERIAL_CONF_Init(pDev) == SERIAL_ERR)
	{
		return NULL;
	} 
	ret  =  listen(pDev -> mSocket,1);
	if ( ret  < 0 ) 
	{
		close(pDev ->mSocket); 
		pDev ->mSocket = -1; 
		KMV_SERIAL_error(" socket listen  error = %d.\r\n",ret);
		return NULL;
	}
	KMV_SERIAL_printf(" socket listen OK\r\n");  
	  
	pDev -> open   = 1;
	pDev -> close  = 0; 
	while(pDev -> open  == 1)
	{

	   pDev ->cSocket = accept(  pDev ->mSocket,NULL, NULL); 
	   if( pDev -> cSocket < 0)
	   {
	     KMV_SERIAL_error("SERVER  SOCKET  ACCEPT   error .\r\n");

	     continue;
	   } 
		//get send buff

		socklen_t optlen = sizeof(int); 
		nSendBufLen = 0;
		getsockopt(pDev->cSocket, SOL_SOCKET, SO_SNDBUF,(char*)&nSendBufLen, &optlen); 
		KMV_SERIAL_printf(" cSocket SNDBUF  = %d.\r\n",nSendBufLen);
		KMV_SERIAL_printf(" SOCKET  ACCEPT   OK\r\n");
		while( pDev ->cSocket >=  0 ) 
		{ 

			 ret =KMV_SERIAL_CONF_Recv(pDev);
				if(ret != SERIAL_OK)
				{
					KMV_SERIAL_error("  Disconet  \r\n"); 
					close(pDev->cSocket);
					pDev->cSocket = -1; 
					break;
				} 
		} 

	}
	 
	KMV_SERIAL_CONF_DeInit(pDev); 
	pDev -> close  = 1;  	

	return SERIAL_OK;
}

/********************************************************************

int  KMV_SERIAL_CONF_Open();

********************************************************************/
int  KMV_SERIAL_CONF_Open()
{
	static KMV_SERIAL_CONF_E gSerialConfDev;
	

	memset(&gSerialConfDev,0,sizeof(KMV_SERIAL_CONF_E));

	gSerialConfDev.pBuf = (char *)malloc( KMV_SERIAL_CONF_INFO_MAX_LEN +32); 
	//serial conf info init
	KMV_SERIAL_CONF_Info_init();

	pthread_create(&(gSerialConfDev.thrHndl),NULL,(void *)KMV_SERIAL_CONF_Proc,(void *)&gSerialConfDev);


	return SERIAL_OK;
}
/********************************************************************

int  KMV_SERIAL_CONF_Close()

********************************************************************/
int  KMV_SERIAL_CONF_Close()
{

	if(gSerialConfDev.open == 1)
	{
		gSerialConfDev.open = 0;
		
		while(gSerialConfDev.close == 1) ;
		pthread_cancel(gSerialConfDev.thrHndl);
	    pthread_join(gSerialConfDev.thrHndl,NULL);
		
	}
	free (gSerialConfDev.pBuf);

	return SERIAL_OK;

}



