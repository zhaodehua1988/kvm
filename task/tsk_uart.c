#include "tsk_uart.h"
#include "his_gpio.h"
#include "sys.h"
#include "tsk.h"
#include "ext.h"
#include "kmv_usb.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include "kmv_serial.h"
#define TSK_UART_DEV_NAME  "/dev/ttyAMA1"  
#define TASK_UART_MAX_LEN  127
  
#define TSK_UART_DEBUG_MODE 
#ifdef  TSK_UART_DEBUG_MODE 
  #define TSK_UART_printf(...) \
  				 do {\
  				  printf("\n\rUART:" __VA_ARGS__);\
  				  fflush(stdout); \
  				  } while(0)
#else 
  #define TSK_UART_printf(...) 
#endif   
 
typedef struct TSK_UART_DEV_E 
{
 WV_THR_HNDL_T thrHndl; 
 WV_U32      open;
 WV_U32      close;
 WV_S32      fd; 
 WV_U32      port;
 WV_U32      baudRate;
 WV_U32      rcvEroNum;
 WV_U32      checkEroNum;
 WV_U32      procEroNum;
 WV_U32      timeoutNum; 
 WV_U32      runNum;
 WV_U32      runTime;
 WV_U8      recvBuf[TASK_UART_MAX_LEN];
 WV_U8      sendBuf[TASK_UART_MAX_LEN];

    
}TSK_UART_DEV_E;

WV_U16 const gcausCrcTable[256] =
{
	0, 4489, 8978, 12955, 17956, 22445, 25910, 29887,
	35912, 40385, 44890, 48851, 51820, 56293, 59774, 63735,
	4225, 264, 13203, 8730, 22181, 18220, 30135, 25662,
	40137, 36160, 49115, 44626, 56045, 52068, 63999, 59510,
	8450, 12427, 528, 5017, 26406, 30383, 17460, 21949,
	44362, 48323, 36440, 40913, 60270, 64231, 51324, 55797,
	12675, 8202, 4753, 792, 30631, 26158, 21685, 17724,
	48587, 44098, 40665, 36688, 64495, 60006, 55549, 51572,
	16900, 21389, 24854, 28831, 1056, 5545, 10034, 14011,
	52812, 57285, 60766, 64727, 34920, 39393, 43898, 47859,
	21125, 17164, 29079, 24606, 5281, 1320, 14259, 9786,
	57037, 53060, 64991, 60502, 39145, 35168, 48123, 43634,
	25350, 29327, 16404, 20893, 9506, 13483, 1584, 6073,
	61262, 65223, 52316, 56789, 43370, 47331, 35448, 39921,
	29575, 25102, 20629, 16668, 13731, 9258, 5809, 1848,
	65487, 60998, 56541, 52564, 47595, 43106, 39673, 35696,
	33800, 38273, 42778, 46739, 49708, 54181, 57662, 61623,
	2112, 6601, 11090, 15067, 20068, 24557, 28022, 31999,
	38025, 34048, 47003, 42514, 53933, 49956, 61887, 57398,
	6337, 2376, 15315, 10842, 24293, 20332, 32247, 27774,
	42250, 46211, 34328, 38801, 58158, 62119, 49212, 53685,
	10562, 14539, 2640, 7129, 28518, 32495, 19572, 24061,
	46475, 41986, 38553, 34576, 62383, 57894, 53437, 49460,
	14787, 10314, 6865, 2904, 32743, 28270, 23797, 19836,
	50700, 55173, 58654, 62615, 32808, 37281, 41786, 45747,
	19012, 23501, 26966, 30943, 3168, 7657, 12146, 16123,
	54925, 50948, 62879, 58390, 37033, 33056, 46011, 41522,
	23237, 19276, 31191, 26718, 7393, 3432, 16371, 11898,
	59150, 63111, 50204, 54677, 41258, 45219, 33336, 37809,
	27462, 31439, 18516, 23005, 11618, 15595, 3696, 8185,
	63375, 58886, 54429, 50452, 45483, 40994, 37561, 33584,
	31687, 27214, 22741, 18780, 15843, 11370, 7921, 3960
};

static TSK_UART_DEV_E gUartDev;
/*******************************************************************

WV_U16 TSK_UART_CRC16(const WV_U8 *pChar,WV_S32 len,WV_U16 crc);

*******************************************************************/
WV_U16 TSK_UART_CRC16(const WV_U8 *pChar,WV_S32 len,WV_U16 crc)
{
	WV_S32 i = 0;
	WV_U16 usTmp;
	for(i = 0;i<len;i++)
	{
		usTmp = gcausCrcTable[((*(pChar + i)) & 0x00ff) ^ (crc & 0x00ff)];
		crc = ((usTmp & 0xff00) | ((usTmp & 0x00ff) ^ ((crc & 0xff00) >> 8)));
	}

	return crc;
}

/*******************************************************************

WV_U16 TSK_UART_PackData(const WV_U8 *pSrc,WV_U32 len,WV_U8 *pDes);

*******************************************************************/
WV_U16 TSK_UART_PackData(const WV_S8 *pSrc,WV_U32 len,WV_S8 *pDes)
{
	WV_U16 crc;
	WV_U16 desLen = 0;
	WV_U8 *p;
	WV_S32 i;
	p = pDes;
	p[0] = 0x7e;
	p++;
	desLen++;
	for(i=0;i<len;i++){
		if(pSrc[i]==0x7e){
			p[0] = 0x7d;
			p[1] = 0x5e;
			p+=2;
			desLen+=2;
		}else if(pSrc[i] == 0x7d){
			p[0] = 0x7d;
			p[1] = 0x5d;
			p+=2;
			desLen+=2;
		}else{
			p[0] = pSrc[i];
			p++;
			desLen++;
		}
	}

	crc = TSK_UART_CRC16(pSrc,desLen-1,0xffff);
	//crc^=0xffff;

	p[0] = (crc & 0xff00) >> 8;//((crc & 0xff00) >> 8
	if(p[0] == 0x7e){
		p[0] = 0x7c;
		p[1] = 0x8e;
		p+=2;
		desLen+=2;
	}else if(p[0] == 0x7c){
		p[0] = 0x7c;
		p[1] = 0x8e;
		p+=2;
		desLen+=2;
	}else{
		p++;
		desLen++;
	}
	
	p[0] = crc & 0x00ff;
	if(p[0] == 0x7e){
		p[0] = 0x7c;
		p[1] = 0x8e;
		p+=2;
		desLen+=2;
	}else if(p[0] == 0x7c){
		p[0] = 0x7c;
		p[1] = 0x8c;
		p+=2;
		desLen+=2;
	}else{
		p++;
		desLen++;
	}
	p[0] = 0x7e;
	desLen++;
	//TSK_UART_printf("desLen [%d]\n",desLen);
	return desLen;
}
/****************************************************************************************

WV_U16 TSK_UART_DePackData(WV_U8 *pBuf,WV_S32 *pLen)

****************************************************************************************/
WV_U16 TSK_UART_DePackData(WV_U8 *pBuf,WV_S32 *pLen)
{
	
	WV_S32 i,len=0,j=0;
	WV_U8 data[TASK_UART_MAX_LEN+64];
	for(i=0;i<*pLen;i++)
	{
		//0x7c0x8e == 0x7e;  0x7c0x8c == 0x7e;
		if(pBuf[i] == 0x7c)
		{
			
			if(pBuf[i+1] == 0x8e)
			{
				data[j] = 0x7e;
			}else if(pBuf[i+1] == 0x8c)
			{
				data[j] = 0x7c;
			}

			i++;
		}else{

			data[j] = pBuf[i];
		}

		len++;
		j++;
	}

		memcpy(pBuf,data,len);
		*pLen = len;

	return 0;
}

/****************************************************************************************

WV_S32 TSK_UART_GetSpeed();

****************************************************************************************/

WV_S32 TSK_UART_GetSpeed(WV_U32 * pBaud)
{
	*pBaud  = gUartDev.baudRate;
	return WV_SOK;    
}

/****************************************************************************************

WV_S32 TSK_UART_SetSpeed();

****************************************************************************************/
WV_S32 TSK_UART_SetSpeed(WV_U32 baud)
{ 
 
	WV_S32  ret;
	struct  termios opt;
	if(gUartDev.fd < 0 )
		{
		TSK_UART_printf("no UART open !!!"); 
		return WV_EFAIL;
		}
 
	tcgetattr(gUartDev.fd, &opt);
	tcflush(gUartDev.fd, TCIOFLUSH);
	switch( baud)
    {
    case 2400:
        cfsetispeed(&opt, B2400);
        cfsetospeed(&opt, B2400);
         gUartDev.baudRate = 2400;
        break;
    case 4800:
        cfsetispeed(&opt, B4800);
        cfsetospeed(&opt, B4800);
         gUartDev.baudRate = 4800;
        break;
    case 9600:
        cfsetispeed(&opt, B9600);
        cfsetospeed(&opt, B9600);
         gUartDev.baudRate = 9600;
        break;
    case 19200:
		cfsetispeed(&opt, B19200);
		cfsetospeed(&opt, B19200);
		gUartDev.baudRate = 19200;
		break;
    case 38400:
		cfsetispeed(&opt, B38400);
		cfsetospeed(&opt, B38400);
		gUartDev.baudRate = 38400;
		break;   
    case 57600:
        cfsetispeed(&opt, B57600);
        cfsetospeed(&opt, B57600);
        gUartDev.baudRate = 57600;
        break;
	case 115200:
		cfsetispeed(&opt, B115200);
		cfsetospeed(&opt, B115200);
		gUartDev.baudRate = 115200;
		break;
    default:
        cfsetispeed(&opt, B4800);
        cfsetospeed(&opt, B4800);
        gUartDev.baudRate = 115200; 
        break;
    } 
	ret = tcsetattr(gUartDev.fd,TCSANOW,&opt);
	if(ret < 0) 
	{
		 TSK_UART_printf(" UART SetSpeed  error  !!!"); 
	}
	tcflush(gUartDev.fd,TCIOFLUSH);
	return ret; 
}

/****************************************************************************************

WV_S32 TSK_UART_Setmode(TSK_UART_DEV_E * pDev)

****************************************************************************************/
WV_S32 TSK_UART_Setmode(TSK_UART_DEV_E * pDev)
{ 
	printf("set uart mode \n");
 
	WV_S32  ret;
	struct  termios opt;
	if(pDev-> fd < 0 )
	{
		TSK_UART_printf("no UART open !!!"); 
		return WV_EFAIL;
	}

	tcgetattr(pDev->fd, &opt);
	tcflush(pDev->fd, TCIOFLUSH);
    // opt.c_cflag  |=  CLOCAL | CREAD;
	//opt.c_cflag &= ~CSIZE;
	//opt.c_cflag |= CS8;     //8 bit
	//opt.c_cflag &= ~PARENB; // no pare
	opt.c_cflag &= ~CSTOPB; // 1 stop bid
	
	//opt.c_cflag |= CSTOPB; // 2 stop [[ opt.c_cflga |=CSTOPB 2 bit stop]
	//opt.c_oflag  &= ~OPOST;	

	cfmakeraw(&opt);
	//opt.c_lflag |= ECHO;
   	tcflush(pDev->fd,TCIFLUSH); 
	opt.c_cc[VTIME] = 0;
	opt.c_cc[VMIN] = 1;
    
	ret = tcsetattr(pDev->fd,TCSANOW,&opt);
	if(ret < 0)
	{
		TSK_UART_printf(" UART SetMode  error  !!!"); 
	}
   tcgetattr(pDev->fd, &opt);
   
   TSK_UART_printf(" UART SetMode  VTIME[%d]VMIN[%d]",opt.c_cc[VTIME],opt.c_cc[VMIN]); 

	return ret; 
}

/****************************************************************************************

WV_S32 TSK_UART_RecvCheck();

****************************************************************************************/
WV_S32 TSK_UART_RecvCheck(WV_U8 *pBuf,WV_U32 len)
{
	WV_S32 i;
	WV_U16 crc=0;
	WV_S32 dataLen;
	
	//printf("recv:\n");
/*
	for(i=0;i<len;i++){
		TSK_UART_printf("Buf[%d]=[%02x]\n",i,pBuf[i]);
	}
*/
	if(pBuf[0] != 0x7E){
		return WV_EFAIL;
	}

	if(pBuf[len-1] != 0x7E){
			return WV_EFAIL;
	}



	TSK_UART_DePackData(pBuf,&len);	
/*
	for(i=0;i<len;i++)
	{
		printf("[%d]%02x ",i,pBuf[i]);
	}
	printf("\n");
*/
#if 0
	if((pBuf[len-2] == 0) && (pBuf[len-3] == 0))
	{
		return WV_SOK;
	}

	crc = TSK_UART_CRC16(&(pBuf[1]),len-4,0xffff);

	if((((crc & 0xff00) >> 8) != pBuf[len -2] ) || ((crc & 0x00ff) != pBuf[len-1]) )
	{
		printf("crc error! crc=[%02x][%02x],pBuf[len-2]=[%02x],pBuf[len-1]=[%02x]\n",(crc & 0xff00) >> 8,(crc & 0x00ff),pBuf[len-2],pBuf[len-1]);
		return WV_EFAIL;
	}
#endif
	return WV_SOK;
}


  
/****************************************************************************************

WV_S32 TSK_UART_Send();

****************************************************************************************/

WV_S32 TSK_UART_Send(TSK_UART_DEV_E * pDev,WV_U32  len)
{ 
 
	 WV_S32  ret;
	 if(pDev-> fd < 0 )
		 {
	        TSK_UART_printf("no UART open !!!"); 
			return WV_EFAIL;
		 }
	 ret  = write(pDev->fd,pDev->sendBuf,len); 
	 if(ret == -1)
	   {
        TSK_UART_printf("TSK_UART_Send errro !!!");  
	   }
	return ret; 
}  

/****************************************************************************************

WV_S32 TSK_UART_AckCmd();

****************************************************************************************/
WV_S32 TSK_UART_AckCmd(TSK_UART_DEV_E * pDev,WV_S32 ret)
{ 
	WV_S32  len;
	WV_S8 src[20];

	memset(pDev->sendBuf,0,TASK_UART_MAX_LEN);
	src[0] = 0x02;
	src[1] = pDev->recvBuf[1];
	if(ret == 0)
	{
		src[2] = 0;
	}else{
		src[2] = 1;
	}
	len = TSK_UART_PackData(src,3,pDev->sendBuf);
	ret = TSK_UART_Send(pDev,len);

	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_Reset();

****************************************************************************************/
WV_S32 TSK_UART_Reset(TSK_UART_DEV_E * pDev)
{ 
 
	WV_S32 ret=0;
	TSK_UART_AckCmd(pDev,ret);	
	sleep(1); 
	ret = WV_CMD_GetReset(); 
	
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_Handle(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_Handle(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	if(pDev->recvBuf[1]!=0 && pDev->recvBuf[5]!=0x7e)
	{
		ret = -1;
	}
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SetIP(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SetIP(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;

	WV_S8 buf[30];
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];
    sprintf(buf,"%d.%d.%d.%d",pBuf[1],pBuf[2],pBuf[3],pBuf[4]); 
    SYS_IP_SetIp(buf);
    sprintf(buf,"%d.%d.%d.%d",pBuf[5],pBuf[6],pBuf[7],pBuf[8]); 
    SYS_IP_SetMask(buf);
    sprintf(buf,"%d.%d.%d.%d",pBuf[9],pBuf[10],pBuf[11],pBuf[12]); 
    SYS_IP_SetGw(buf); 
    SYS_IP_SaveConf();

	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SetLocalAddr(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SetLocalAddr(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;

	WV_S8 buf[30];
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	WV_U16 port;
	sprintf(buf,"%d.%d.%d.%d",pBuf[1],pBuf[2],pBuf[3],pBuf[4]); 
    port = *((WV_U16*)(pBuf+5));	
    TSK_VENC_SetSendIp(0, buf,port);
    TSK_VENC_SetSendIp(1, buf,port);
    
    TSK_AI_SetSendIp(0, buf,port);
    TSK_AI_SetSendIp(1, buf,port);

	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SetRemoteAddr(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_SetRemoteAddr(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;

	WV_S8 buf[30];
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	WV_U16 port;
	sprintf(buf,"%d.%d.%d.%d",pBuf[1],pBuf[2],pBuf[3],pBuf[4]); 
    port = *((WV_U16*)(pBuf+5));	
    TSK_SRX_SetRxIp(0,buf,port);
	TSK_UART_AckCmd(pDev,ret);
	return ret; 
 
}

/****************************************************************************************

WV_S32 TSK_UART_SetUartPort_1(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SetUartPort_1(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;

	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	ret = SYS_ENV_SetU32("VSerialPort_1",port);
	ret = SYS_ENV_Save();
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SetUartPort_2(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SetUartPort_2(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	ret = SYS_ENV_SetU32("VSerialPort_2",port);
	ret = SYS_ENV_Save();
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SetUartPort_3(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SetUartPort_3(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	ret = SYS_ENV_SetU32("VSerialPort_3",port);
	ret = SYS_ENV_Save();
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SetUartPort_4(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SetUartPort_4(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	ret = SYS_ENV_SetU32("VSerialPort_4",port);
	ret = SYS_ENV_Save();
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SwVGA(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SwVGA(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];
    ret = EXT_VGA_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	 KMV_USB_DEV_ChlSw(pBuf[2],pBuf[1],pBuf[3],0);
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}
/****************************************************************************************

WV_S32 TSK_UART_SwDVI(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SwDVI(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];
	//printf("in[%02x]out[%02x],ena= %d \n",pBuf[1],pBuf[2],pBuf[3]);
    ret = EXT_DVI_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	 KMV_USB_DEV_ChlSw(pBuf[2],pBuf[1],pBuf[3],1);
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}
/****************************************************************************************

WV_S32 TSK_UART_SwLine(TSK_UART_DEV_E * pDev);

****************************************************************************************/

WV_S32 TSK_UART_SwLine(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

    EXT_3268_SetMux(pBuf[1] ,pBuf[3]);

	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SwMic(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_SwMic(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	ret = EXT_3268_SetMic(pBuf[1] ,pBuf[3]);
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}
/****************************************************************************************

WV_S32 TSK_UART_SwMouse(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_SwMouse(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	//ret = KMV_USB_DEV_ChlSw(pBuf[2],pBuf[1],pBuf[3]);
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}

/****************************************************************************************

WV_S32 TSK_UART_SwAudio(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_SwAudio(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];
	WV_U8 chn;
	switch(pBuf[1])
	{
		case 0:
			chn = 1;
			break;
		case 1:
			chn = 2;
			break;
		case 2:
			chn = 0;
			break;
		case 3:
			chn = 3;
			break;
		default:
			return 0;
	}	
	
	ret =   EXT_3268_SetMux(chn ,pBuf[3]);
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}
/****************************************************************************************

WV_S32 TSK_UART_SwDecodeChn(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_SwDecodeChn(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0;
	WV_U8 *pBuf;
	pBuf = &pDev->recvBuf[1];

	//EXT_VGA_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	//EXT_DVI_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	//KMV_USB_DEV_changeRemoteChn(pBuf[2],0);
	//printf("sw out[%d]in[%d]\n",pBuf[2],pBuf[1]);
//	ret = KMV_USB_DEV_ChlSw(pBuf[1],pBuf[2],pBuf[3]);
	TSK_UART_AckCmd(pDev,ret);
	return ret;  
}
/****************************************************************************************

WV_S32 TSK_UART_SearchInfo(TSK_UART_DEV_E * pDev);

****************************************************************************************/
WV_S32 TSK_UART_SearchInfo(TSK_UART_DEV_E * pDev)
{

	WV_S32  len;
	WV_U8 pBuf[30];
	memset(pDev->sendBuf,0,TASK_UART_MAX_LEN);


	WV_S32 ret = 0,i;
	WV_U8 vgaSw[5];
	WV_U8 dviSw[5];
	WV_U8 ip[10];
	WV_U16 localPort=8080;
	WV_U16 uartPort[4];
	WV_U8 audio[4];
	KMV_SVR_printf("svr cmd search info ");
	TSK_SW_GetSW(dviSw,vgaSw);//get sw 
	SYS_IP_getIpInt(ip);//get ip
	EXT_3268_GetMux(audio);//get audio
	KMV_V_SERIAL_GetPortCmd(uartPort);//get v_serial port
	
	pBuf[0]=0x30;
	//set ip
	for(i=0;i<4;i++)
	{
		pBuf[i+1] = ip[i];//pBuf[4]
	}
	//set port
	pBuf[5] = localPort >> 8;
	pBuf[6] = localPort & 0xff;//pBuf[6]
	//printf("localPort=0x%02x[%d],pBuf[5]=0x%02x[%d],localPort >> 8=[%02x],pBuf[6]=0x%02x[%d],localPort &0xff =[0x%02x]",\
			localPort,localPort,pBuf[5],pBuf[5],localPort >> 8,pBuf[6],pBuf[6],localPort & 0xff);
	//set uart port
	for(i=0;i<4;i++)
	{
		pBuf[7+2*i] = uartPort[i] >> 8;
		pBuf[8+2*i] = uartPort[i] & 0xff;		 
	}                       //pBuf[14]
	//set vgaSw
	for(i=0;i<5;i++)
	{
		pBuf[15+i] = vgaSw[i] | 0x80;
	}                      //pBuf[19]
	//set dviSw
	for(i=0;i<5;i++)
	{
		pBuf[20+i] = dviSw[i] |= 0x80;
	}                     //pBuf[24]
	//set audio
	for(i=0;i<4;i++)
	{
		pBuf[25+i] = audio[i];
	}                    //pBuf[28]
	
	printf("\nsearch info ");


	for(i=0;i<29;i++)
	{

		printf("buf[%d]=%d[0x%02x] ",i,pBuf[i],pBuf[i]);
	}
	printf("\n");

	len = TSK_UART_PackData(pBuf,29,pDev->sendBuf);
	
	ret = TSK_UART_Send(pDev,len);
		
}



/****************************************************************************************
WV_S32 TSK_UART_RecvCmd(TSK_UART_DEV_E *pDev);
****************************************************************************************/
WV_S32 TSK_UART_RecvCmd(TSK_UART_DEV_E *pDev)
{


#if 1
	WV_S32  recvLen,len,ret;
	struct  termios opt;
	WV_U8 *p;
	ret = -1;
	p=pDev->recvBuf;	
	len = 0;
//	printf("\nin tsk uart recv cmd\n ");
	if(pDev-> fd < 0 )
	{
		TSK_UART_printf("no UART open !!!"); 
		return WV_EFAIL;
	} 
       /*************************/
	tcgetattr(pDev->fd, &opt); 
	opt.c_cc[VTIME] = 0;
	opt.c_cc[VMIN] = 1; 
	tcsetattr(pDev->fd,TCSANOW,&opt);
	   /*************************/

	recvLen  = read(pDev->fd, p,1); 
//	TSK_UART_printf("uart read 1 [%02x]\n",p[0]);
	if((recvLen != 1 ) || (p[0] != 0x7e))
	{
		TSK_UART_printf("read uart recv error\n");
		return WV_EFAIL;
	}
	len += recvLen;
	
	p++;

     /**************************************/
	tcgetattr(pDev->fd, &opt); 
	opt.c_cc[VTIME] = 1;
	opt.c_cc[VMIN] = 0; 
	tcsetattr(pDev->fd,TCSANOW,&opt);
	   /***********************************/

	/****************7e 7e *********/
	recvLen  = read(pDev->fd, p,1); 
	if(recvLen != 1 ) 
	{
		return WV_EFAIL;
	}else if((p[0] == 0x7e )){
		p--;
		len--;
	}
	len += recvLen;
	p++;
	/********************************* * **/
	while(1){
		recvLen  = read(pDev->fd, p,1);
		len+=recvLen;
		if((recvLen != 1 ) || (p[0] == 0x7e))
		{
			break;
		}
		p++;
		
		if(len>127 || len < 1){
			TSK_UART_printf("len= %d\n",len);
			return WV_EFAIL;
		}		
	}
/*
	int i;
	for(i=0;i<len;i++)
	{
		printf("%02x ",pDev->recvBuf[i]);	
		
	}
	printf("\n");
*/
	ret = TSK_UART_RecvCheck(pDev->recvBuf,len);
	if(ret != WV_SOK){
		pDev->checkEroNum++;
		return WV_EFAIL;
	}

	return WV_SOK;
#endif
}

/***************************************************************

WV_S32 TSK_UART_Cmd(TSK_UART_DEV_E  *pDev,WV_U8 *pBuf);

***************************************************************/
WV_S32 TSK_UART_RunCmd(TSK_UART_DEV_E  *pDev)
{

		WV_S32 ret = 0;
		switch(pDev->recvBuf[1])
		{
			case  0x01://reset 
			   TSK_UART_printf("recev CMD  reset\n");
			   TSK_UART_Reset(pDev);
			    break;
			case 0x02:
				TSK_UART_printf("recev CMD  ack\n");
				TSK_UART_AckCmd(pDev,ret);
				break;
			case 0x03:
				TSK_UART_printf("recev CMD  handle\n");
				TSK_UART_Handle(pDev);
				break;
			case 0x04:
				TSK_UART_printf("recev CMD  set local ip\n");
				TSK_UART_SetIP(pDev);
				break;
			case 0x05:
				TSK_UART_printf("recev CMD  set local addr\n");
				TSK_UART_SetLocalAddr(pDev);
				break;
			case 0x06:
				TSK_UART_printf("recev CMD  set remote addr\n");
				TSK_UART_SetRemoteAddr(pDev);
				break;
			case 0x07:
				TSK_UART_printf("recev CMD  set uart port 1 \n");
				TSK_UART_SetUartPort_1(pDev);
				break;
			case 0x08:
				TSK_UART_printf("recev CMD  set uart port 2\n");
				TSK_UART_SetUartPort_2(pDev);
				break;
			case 0x09:
				TSK_UART_printf("recev CMD  set uart port 3\n");
				TSK_UART_SetUartPort_3(pDev);
				break;
			case 0x0a:
				TSK_UART_printf("recev CMD  set uart port 4\n");
				TSK_UART_SetUartPort_4(pDev);
				break;
			case 0x10:
				TSK_UART_printf("recev CMD  SW VGA \n");
				TSK_UART_SwVGA(pDev);
				break;
			case 0x11:
				TSK_UART_printf("recev CMD  SW DVI \n");
				TSK_UART_SwDVI(pDev);
				break;
			case 0x12:
				TSK_UART_printf("recev CMD  SW Line \n");
				TSK_UART_SwLine(pDev);
				break;
			case 0x13:
				TSK_UART_printf("recev CMD  SW Mic \n");
				TSK_UART_SwMic(pDev);
				break;
			case 0x14:
				TSK_UART_printf("recev CMD  SW Mouse \n");
				TSK_UART_SwMouse(pDev);
				break;
			case 0x15:
				TSK_UART_printf("recev CMD  SW deCodeChn \n");
				TSK_UART_SwDecodeChn(pDev);
				break;
			case 0x16:
				TSK_UART_printf("recev CMD  SW Audio \n");
				TSK_UART_SwAudio(pDev);
				break;
			case 0x30:
				TSK_UART_printf("recev CMD  search Info \n");
				TSK_UART_SearchInfo(pDev);
				break;
			default:
			   pDev -> procEroNum ++; 
			  TSK_UART_printf("recev CMD   not list\n"); 
		} 

	return ret;
}

/****************************************************************************************

WV_S32 TSK_UART_Init();

****************************************************************************************/

WV_S32 TSK_UART_Init(TSK_UART_DEV_E * pDev)
{ 
	WV_S32 ret = 0; 
	pDev->fd =  open(TSK_UART_DEV_NAME, O_RDWR);
	if(pDev-> fd < 0 )
	{
		TSK_UART_printf("UART open erro!!!"); 
		return WV_EFAIL;
	}
	//TSK_UART_SetSpeed(pDev->baudRate);
	TSK_UART_SetSpeed(9600);
	TSK_UART_Setmode(pDev);
return ret;  
}


/****************************************************************************************

WV_S32 TSK_UART_DeInit();

****************************************************************************************/

WV_S32 TSK_UART_DeInit(TSK_UART_DEV_E * pDev)
{ 
WV_S32 ret = 0; 
 
 ret = 	close(pDev -> fd);
		 
return ret;  
}


/******************************************************************************

void *TSK_UART_Proc(Void * prm);

******************************************************************************/
void * TSK_UART_Proc ( void * prm)
{
	TSK_UART_DEV_E  * pDev; 
	WV_S32 ret;

	pDev = (TSK_UART_DEV_E  *) prm;
	pDev-> open  = 1;
	pDev-> close  = 0;  
	TSK_UART_printf("TSK_UART_begin"); 		
	while(pDev -> open == 1)
	{  
		struct  timeval    tv;
		struct  timezone   tz;
		gettimeofday(&tv,&tz);
		pDev ->runTime = tv.tv_sec; 
		ret  = TSK_UART_RecvCmd(pDev);
	
		pDev ->runNum ++;  
		if(ret != WV_SOK)
		{ 
			pDev -> rcvEroNum ++;
			continue; 	 
		} 
		ret = TSK_UART_RunCmd(pDev);
    }  
 pDev-> open  = 0;
 pDev-> close = 1;
 return NULL;  
} 

  
/****************************************************************************************

WV_S32 TSK_UART_Open();

****************************************************************************************/
WV_S32 TSK_UART_Open()
{ 
	WV_S32 ret = 0;  
	TSK_UART_printf("TSK_UART_Open()");  

	memset(&gUartDev,0,sizeof(gUartDev));

	TSK_UART_Init(&gUartDev); 

	WV_RET_ADD( WV_THR_Create(&gUartDev.thrHndl, TSK_UART_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gUartDev),ret);
		 	
	return ret;
}
 
/****************************************************************************************

WV_S32  TSK_UART_Close();

****************************************************************************************/
WV_S32  TSK_UART_Close()
{ 

	WV_S32 ret = 0;
   if(gUartDev.open == 1)
   {
   	  gUartDev.open = 0;
   	  while(gUartDev.close == 1) ;
      WV_CHECK_RET( WV_THR_Destroy(&gUartDev.thrHndl));	
   }
   TSK_UART_DeInit(&gUartDev);  
 return ret;  
}

