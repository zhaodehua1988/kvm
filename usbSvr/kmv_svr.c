#include "kmv_svr.h"
#include "kmv_svr_cmd.h"

#define SVR_DATA_MAX_LEN 127
#define KMV_SVR_PORT 8080
#define SVR_SYNC_WORD 0x5a5a5a5a



typedef struct SVR_SERVER_DEV_E 
{
	pthread_t thrHndl; 
	unsigned int      open;
	unsigned int      close; 
	int      mSocket;
	int      cSocket;
	unsigned int      port;
	SVR_HEAD_E  headBuf;
	unsigned short      recvCnt;
	unsigned short      sendCnt; 
	unsigned char      *pBuf;
	unsigned int conectNum;
	unsigned int      startMs;
	unsigned int      cmdMs; 
} SVR_SERVER_DEV_E; 

static SVR_SERVER_DEV_E  * pSvrDev;

/********************************************************

int SVR_Init(SVR_SERVER_DEV_E  * pDev)

********************************************************/ 
int  SVR_Init(SVR_SERVER_DEV_E  * pDev)
{
	unsigned int reuse;
	socklen_t len;
	struct sockaddr_in addr;
	int status;

	pDev -> mSocket = socket(AF_INET,SOCK_STREAM,0);
	if (pDev -> mSocket < 0) 
	{ 
		KMV_SVR_error("SERVER  socket init error.\r\n");
		return  SVR_ERR; 
	}

	//fcntl(DpDev ->mSocket,F_SETFL,O_NONBLOCK);//no block
	reuse = 1;
	len = sizeof(reuse);
	setsockopt(pDev ->mSocket, SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,len);
	//get sen buff
	int   nSendBufLen;
	socklen_t optlen = sizeof(int); 
	nSendBufLen = 0;
	getsockopt(pDev->mSocket, SOL_SOCKET, SO_SNDBUF,(char*)&nSendBufLen, &optlen); 
	// KMV_SVR_printf("SERVER socket SNDBUF  = %d.\r\n",nSendBufLen);


	addr.sin_family = AF_INET;
	addr.sin_port = htons(pDev ->port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	bzero(&(addr.sin_zero), 8);
	status  =  bind(pDev ->mSocket, (struct sockaddr *) &addr, sizeof(addr));
	if( status < 0 )
	{
		close(pDev->mSocket); 
		pDev ->mSocket = -1; 
		KMV_SVR_error("SERVER socket bind  error = %d.\r\n",status);
		return SVR_ERR;
	} 

	return SVR_OK;
}

/********************************************************

int SVR_DeInit(SVR_SERVER_DEV_E  * pDev)

********************************************************/ 

int SVR_DeInit(SVR_SERVER_DEV_E *pDev)
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
  KMV_SVR_printf("SERVER socket DeInit OK\r\n");   
  return  SVR_OK;
}



/********************************************************

int  SVR_SERVER_Recv(int socket,unsigned char * pBuf, int len)

********************************************************/ 
int SVR_SERVER_Recv(int socket,unsigned char * pBuf, int len)
{
  int recvLen,recvToal,reqLen,ret =0;
  unsigned char * pRevBuf;
  pRevBuf = pBuf;
  recvLen = len;
  recvToal = 0; 
  reqLen = len; 
   while(1)
   {

     recvLen = recv(socket,pRevBuf,reqLen,0);
      if(recvLen >0)
       {   
        	recvToal +=recvLen;
     
            if(recvToal == len)
            	{
            	  ret = SVR_OK;
         		   break; 
            	}
            else 
		        {
		          pRevBuf =  pBuf + recvToal;
		          reqLen = len - recvToal;
		        }	
         
        }
      else if(recvLen == 0)
      {
       	KMV_SVR_error("SERVER CNNECT loss !!!!!!!!!! %d",recvLen);
        ret = 1;
        break;  
      }
      else
      {
      	
      	KMV_SVR_error("SERVER recv EROOR !!!!!!!!!! %d",recvLen);
      	ret = SVR_ERR;
        break;
      }
   }

     
 return ret;
}

/*********************************************************
int SVR_HEAD_Analyze(SVR_HEAD_E *pHead );

*********************************************************/
int SVR_HEAD_Analyze(SVR_HEAD_E *pHead)
{
	if(pHead->sync != SVR_SYNC_WORD)
	{
		KMV_SVR_error("sync word error!!!!");
		return SVR_ERR;
	}
	
	if((pHead->dataNum <1 )||(pHead->dataNum > 127)){
		KMV_SVR_error("data len is [1,%d],err:%d",SVR_DATA_MAX_LEN,pHead->dataNum);
		return SVR_ERR;
	}
	return SVR_OK;
}
/*********************************************************

void * SVR_SERVER_GetCmd( );

*********************************************************/

int SVR_SERVER_GetCmd(SVR_SERVER_DEV_E * pDev)
{
 
    int len,ret; 	
	len =5;
	//KMV_SVR_printf("head len = %d\n",len);
	 ret = SVR_SERVER_Recv (pDev ->cSocket,(unsigned char *)&(pDev ->headBuf),len);
	if(ret != SVR_OK)
	{
		KMV_SVR_error("server receve error!!");
		return SVR_ERR;
	}
	//
	 
    int i;

//	KMV_SVR_printf("recv cmd head:");
/*
	for(i=0;i<5;i++)
	{
		printf("%02X",((unsigned char *)&(pDev ->headBuf))[i]);
	}

		printf("dataNum = [%d]",pDev->headBuf.dataNum);   
		printf("\r\n");
*/

	//
	ret = SVR_HEAD_Analyze(&(pDev->headBuf));
	if(ret != SVR_OK)
	{
		KMV_SVR_error("head Analyze error!!");
		return SVR_ERR;
	}
	
	//recv load
	if(pDev->headBuf.dataNum > 0)
	{
		SVR_SERVER_Recv(pDev ->cSocket,pDev-> pBuf, pDev->headBuf.dataNum);
	}	

	return SVR_OK;

}
/*********************************************************

void * SVR_SERVER_SendCmd( );

*********************************************************/
int SVR_SERVER_SendCmd(SVR_SERVER_DEV_E * pDev)
{
 
    int len,sendLen; 	
    //send frame head
    char buf[1024*4];
    int i;
     
	KMV_SVR_printf("ack cmd head:");
  	/*
  	for(i=0;i< 5;i++)
  		{
  	 	  	printf("%02X",((unsigned char *)&(pDev ->headBuf))[i]);
  		}
  	 printf("\r\n");
    */
	len = 5+pDev->headBuf.dataNum; 
	memset(buf,0,sizeof(buf));
	memcpy(buf,(char *)&pDev->headBuf,5);

	if(pDev->headBuf.dataNum > 0 )
	{
		memcpy(&buf[5],pDev->pBuf,pDev->headBuf.dataNum);
	}

    sendLen = send(pDev->cSocket,buf,len,0); 
    if(sendLen != len)
    { 
    
      	KMV_SVR_error("SERVER send  Head EROOR ! %d[%d]\r\n",sendLen,len);
     	return SVR_ERR;  
    }
    //send load
/*
    len = pDev->headBuf.dataNum; 
	if(pDev -> headBuf.dataNum  > 0 )
	{
		sendLen = send(pDev->cSocket,pDev -> pBuf,pDev -> headBuf.dataNum,0);  
		if(sendLen != len)
		{
		 	KMV_SVR_error("SERVER send  load EROOR ! %d[%d]\r\n",sendLen,pDev -> headBuf.dataNum);
		 	return SVR_ERR;  
		}
	} 
	*/
	return SVR_OK;
}

/*********************************************************

void * SVR_SERVER_Proc(void * prm);

*********************************************************/
void * SVR_SERVER_Proc(void * prm)
{
	SVR_SERVER_DEV_E  * pDev;
	int ret; 
	pDev = (SVR_SERVER_DEV_E *) prm; 
	//
	if( SVR_Init(pDev) == SVR_ERR)
	  {
	    return NULL;
	  } 
       ret  =  listen(pDev -> mSocket,1);
	   if ( ret  < 0 ) 
	     {
			close(pDev ->mSocket); 
			pDev ->mSocket = -1; 
				KMV_SVR_error("SERVER socket listen  error = %d.\r\n",ret);
				return NULL;
		  }
	KMV_SVR_printf("SERVER socket listen OK\r\n");  
	  
	pDev -> open   = 1;
	pDev -> close  = 0; 
	while(pDev -> open  == 1)
	{
		  
		   pDev ->cSocket = accept(  pDev ->mSocket,NULL, NULL); 
		   if( pDev -> cSocket < 0)
		   {
		     KMV_SVR_error("SERVER  SOCKET  ACCEPT   error =.\r\n");
		     continue;
		   } 
			pDev -> recvCnt = 0; 
			pDev -> sendCnt = 0; 
			//get sen buff
			int   nSendBufLen;
			socklen_t optlen = sizeof(int); 
			nSendBufLen = 0;
			getsockopt(pDev->cSocket, SOL_SOCKET, SO_SNDBUF,(char*)&nSendBufLen, &optlen); 
			KMV_SVR_printf("SERVER cSocket SNDBUF  = %d.\r\n",nSendBufLen);
			KMV_SVR_printf("SERVER  SOCKET  ACCEPT   OK\r\n");
 
			pDev -> conectNum++;
	      while( pDev ->cSocket >=  0 ) 
		  { 
		     //get cmd frame 
				ret = SVR_SERVER_GetCmd(pDev); 
				
				if(ret != SVR_OK)
				{
					KMV_SVR_error("SERVER  SOCKET  DISconet  GetCmd \r\n"); 
					close(pDev->cSocket);
					pDev->cSocket = -1; 
					break;
				} 
		     
				// proc   
				ret = KMV_SVR_CMD_Proc(&(pDev ->headBuf),pDev -> pBuf);  
				//ack  cmd 
				//printf("svr cmd proc\n");
				ret = SVR_SERVER_SendCmd(pDev);
				if(ret != SVR_OK)
				{
					KMV_SVR_printf("SERVER  SOCKET  DISconet  SendCmd \r\n"); 
					close(pDev->cSocket);
					pDev->cSocket = -1; 
					break;
				} 
               // HI_SYS_GetTimeStampMs  ( &(pDev ->cmdMs)); 
		  } 
		  
	}
	 
	SVR_DeInit(pDev); 
	pDev -> close  = 1;  
   
	return NULL; 
}

/********************************************************************

int  SVR_Open();

********************************************************************/
int  SVR_Open()
{

	pSvrDev = (SVR_SERVER_DEV_E *)malloc(sizeof(SVR_SERVER_DEV_E)); 
	memset(pSvrDev,0,sizeof(SVR_SERVER_DEV_E));

	pSvrDev -> pBuf = (unsigned char *)malloc( SVR_DATA_MAX_LEN +32); 

	//SVR_CMD_Init();
	pSvrDev-> port = KMV_SVR_PORT;
	pthread_create(&(pSvrDev->thrHndl),NULL,(void *)SVR_SERVER_Proc,(void *)pSvrDev);

	return SVR_OK;
}

/********************************************************************

int  SVR_Close();

********************************************************************/
int  SVR_Close()
{
  
	if(pSvrDev->open == 1)
	{
		pSvrDev->open = 0;
		while(pSvrDev->close == 1) ;

		pthread_cancel(pSvrDev->thrHndl);
		pthread_join(pSvrDev->thrHndl,NULL);   
	}
   free (pSvrDev -> pBuf);
   free (pSvrDev);
   //SVR_CMD_DeInit();
   return SVR_OK; 
}
