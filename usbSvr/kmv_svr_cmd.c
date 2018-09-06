#include "kmv_svr_cmd.h"
#include "sys.h"
#include "tsk.h"
#include "ext.h"
#include "kmv_usb.h"
#include "tsk_sw.h"
#include "sys_ip.h"
#include "tlv320aic3268.h"
#include "tsk_uart.h"
#include "kmv_serial.h"
/************************************************************

int KMV_SVR_CMD_Ask(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_Ask(SVR_HEAD_E *pHead,unsigned char *pBuf,char ret)
{
	pHead->dataNum = 3;
	pBuf[1] = pBuf[0];
	pBuf[0] = 0x02;
	pBuf[2] = ret;
		
	return ret;
}


/************************************************************

int KMV_SVR_CMD_Reset(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_Reset(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret=0;
	KMV_SVR_printf("svr cmd reset\n");
	WV_CMD_GetReset(); 
	ret = KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	
	return ret;
}


/************************************************************

int KMV_SVR_CMD_Handel(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_Handel(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd handle\n");
	
	ret = KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	
	return SVR_OK;
}
/************************************************************

int KMV_SVR_CMD_SetLocalIp(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetLocalIp(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	WV_S8 buf[30];
	KMV_SVR_printf("svr cmd set local IP\n");
    sprintf(buf,"%d.%d.%d.%d",pBuf[1],pBuf[2],pBuf[3],pBuf[4]); 
    SYS_IP_SetIp(buf);
    sprintf(buf,"%d.%d.%d.%d",pBuf[5],pBuf[6],pBuf[7],pBuf[8]); 
    SYS_IP_SetMask(buf);
    sprintf(buf,"%d.%d.%d.%d",pBuf[9],pBuf[10],pBuf[11],pBuf[12]); 
    SYS_IP_SetGw(buf); 
    SYS_IP_SaveConf();
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_SetLocalAddr(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetLocalAddr(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd set local address\n");
	char  buf[30];
	WV_U16 port;
	sprintf(buf,"%d.%d.%d.%d",pBuf[1],pBuf[2],pBuf[3],pBuf[4]); 
    port = *((WV_U16*)(pBuf+5));	
    TSK_VENC_SetSendIp(0, buf,port);
    TSK_VENC_SetSendIp(1, buf,port);
    
    TSK_AI_SetSendIp(0, buf,port);
    TSK_AI_SetSendIp(1, buf,port);
    
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}
/************************************************************

int KMV_SVR_CMD_SetRemoteAddr(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetRemoteAddr(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	//pHead->dataNum = 1;
	int ret = 0;
	KMV_SVR_printf("svr cmd set remote address\n");
    char  buf[30];
	WV_U16 port;
	sprintf(buf,"%d.%d.%d.%d",pBuf[1],pBuf[2],pBuf[3],pBuf[4]); 
    port = *((WV_U16*)(pBuf+5));	
    TSK_SRX_SetRxIp(0,buf,port);
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_SetUartPort1(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetUartPort1(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	
	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	KMV_SVR_printf("svr cmd set uart port 1 [%d]\n",port);
	ret = SYS_ENV_SetU32("VSerialPort_1",port);
	ret = SYS_ENV_Save();
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_SetUartPort2(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetUartPort2(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	
	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	KMV_SVR_printf("svr cmd set uart port 2 [%d]\n",port);
	ret = SYS_ENV_SetU32("VSerialPort_2",port);
	ret = SYS_ENV_Save();
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_SetUartPort3(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetUartPort3(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	
	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	KMV_SVR_printf("svr cmd set uart port 3 [%d]\n",port);
	ret = SYS_ENV_SetU32("VSerialPort_3",port);
	ret = SYS_ENV_Save();
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}


/************************************************************

int KMV_SVR_CMD_SetUartPort4(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SetUartPort4(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	
	WV_U32 port;
	port = pBuf[1] * 256 +  pBuf[2];
	KMV_SVR_printf("svr cmd set uart port 4 [%d]\n",port);
	ret = SYS_ENV_SetU32("VSerialPort_4",port);
	ret |= SYS_ENV_Save();
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_ChangeVGA(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeVGA(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change VGA in[%d] to out[%d] ena[%d] ",pBuf[1],pBuf[2],pBuf[3]);
	
    ret = EXT_VGA_SetSw(pBuf[2],pBuf[1],pBuf[3]);

	if(pBuf[2] == 4)
	{
		KMV_USB_DEV_changeRemoteChn(0,pBuf[1],0);
	}
	
	KMV_USB_DEV_ChlSw(pBuf[2],pBuf[1],pBuf[3],0);
	
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_ChangeDVI(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeDVI(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change DVI in[%d] to out[%d] ena[%d] \n",pBuf[1],pBuf[2],pBuf[3]);
    ret = EXT_DVI_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	if(pBuf[2] == 4)
	{
		KMV_USB_DEV_changeRemoteChn(1,pBuf[1],0);
	}

	KMV_USB_DEV_ChlSw(pBuf[2],pBuf[1],pBuf[3],1);
	
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_ChangeLINE(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeLine(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change Line ");
     EXT_3268_SetMux(pBuf[1] ,pBuf[3]);
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_ChangeMic(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeMic(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change Mic ");
	ret = EXT_3268_SetMic(pBuf[1] ,pBuf[3]);
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_ChangeAudio(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeAudio(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change video ");
//	ret = EXT_3268_SetMic(pBuf[1] ,pBuf[3]);

	unsigned char chn;
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
			KMV_SVR_CMD_Ask(pHead,pBuf,-1);
			return 0;
	}	

	ret = EXT_3268_SetMux(chn,pBuf[3]); 
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

int KMV_SVR_CMD_ChangeUART(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
		int ret = 0;
	KMV_SVR_printf("svr cmd change UART baud ");
//	ret = EXT_3268_SetMic(pBuf[1] ,pBuf[3]);

	unsigned int num; // 0 1 2 3 
	unsigned int baudRate;
	unsigned int checkBit;
	unsigned int dataBit;
	unsigned int stopBit;

	baudRate = *(unsigned int*)&pBuf[2];
	num     = pBuf[1];
	checkBit = 0;
	dataBit = 8;
	stopBit = 1;	
	printf("change com %d baud %d \n",num,baudRate);
	ret = EXT_FPGA_ConfUart(num,baudRate,checkBit,stopBit); 
	
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_ChangeMouse(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeMouse(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change Mouse in[%d] out[%d] ena[%d]\n",pBuf[1],pBuf[2],pBuf[3]);
	//ret = KMV_USB_DEV_ChlSw(pBuf[2],pBuf[1],pBuf[3]);
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}
/************************************************************

int KMV_SVR_CMD_ChangeDecodeChn(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_ChangeDecodeChn(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0;
	KMV_SVR_printf("svr cmd change Decode chn in[%d] out[%d] ena[%d]\n",pBuf[1],pBuf[2],pBuf[3]);
	//EXT_VGA_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	//EXT_DVI_SetSw(pBuf[2],pBuf[1],pBuf[3]);
	//KMV_USB_DEV_changeRemoteChn(pBuf[2],0);
	KMV_SVR_CMD_Ask(pHead,pBuf,ret);
	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_SearchInfo(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_SearchInfo(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	int ret = 0,i;
	unsigned char vgaSw[5];
	unsigned char dviSw[5];
	unsigned char ip[10];
	unsigned short localPort=8080;
	unsigned short uartPort[4];
	unsigned char audio[4];
	KMV_SVR_printf("svr cmd search info ");
	TSK_SW_GetSW(dviSw,vgaSw);
	SYS_IP_getIpInt(ip);
	EXT_3268_GetMux(audio);
	KMV_V_SERIAL_GetPortCmd(uartPort);
	pBuf[0]=0x30;
	//set ip
	for(i=0;i<4;i++)
	{
		pBuf[i+1] = ip[i];//pBuf[4]
	}
	//set port
	pBuf[5] = localPort >> 8;
	pBuf[6] = localPort & 0xff;//pBuf[6]
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
		pBuf[20+i] = dviSw[i] | 0x80;
	}                     //pBuf[24]
	//set audio
	for(i=0;i<4;i++)
	{
		pBuf[25+i] = audio[i];
	}                    //pBuf[28]
	
	pHead->dataNum = 29;
/*
	printf("\nsearch info ");

	for(i=0;i<29;i++)
	{

		printf("buf[%d]=%02x ",i,pBuf[i]);
	}
	printf("\n");
*/
	//KMV_SVR_CMD_Ask(pHead,pBuf,ret);

	return SVR_OK;
}

/************************************************************

int KMV_SVR_CMD_Proc(SVR_HEAD_E *pHead,unsigned char *pBuf);

*************************************************************/
int KMV_SVR_CMD_Proc(SVR_HEAD_E *pHead,unsigned char *pBuf)
{
	unsigned char data;
	data = pBuf[0];
	int ret = 0;
	switch(data){
		case KMV_SVR_CMD_RESET :
			//reset 
			ret = KMV_SVR_CMD_Reset(pHead,pBuf);
			break;

		case KMV_SVR_CMD_ASK :
			//svr ask		
			KMV_SVR_CMD_Ask(pHead,pBuf,ret);
			break;

		case KMV_SVR_CMD_HANDLE:
			//handle test 
			ret = KMV_SVR_CMD_Handel(pHead,pBuf);		
			break;

		case KMV_SVR_CMD_SET_LOCAL_IP:
			//set local machine IP address
			ret = KMV_SVR_CMD_SetLocalIp(pHead,pBuf);
			//KMV_SVR_CMD_Ask(pHead,pBuf,ret);
			break;

		case KMV_SVR_CMD_SET_LOCAL_ADDR:
			//set local send address
			KMV_SVR_CMD_SetLocalAddr(pHead,pBuf);
			break;

		case KMV_SVR_CMD_SET_REMOTE_ADDR:
			//set remote recv address
			KMV_SVR_CMD_SetRemoteAddr(pHead,pBuf);
			break;

		case KMV_SVR_CMD_UART_PORT1:
			//set uart port 1
			KMV_SVR_CMD_SetUartPort1(pHead,pBuf);
			break;

		case KMV_SVR_CMD_UART_PORT2:
			//set uart port 2
			KMV_SVR_CMD_SetUartPort2(pHead,pBuf);
			break;
		case KMV_SVR_CMD_UART_PORT3:
			//set uart port 3
			KMV_SVR_CMD_SetUartPort3(pHead,pBuf);
			break;

		case KMV_SVR_CMD_UART_PORT4:
			//set uart port 4
			KMV_SVR_CMD_SetUartPort4(pHead,pBuf);
			break;

		case KMV_SVR_CMD_VGA_CHANGE:
			//change VGA
			KMV_SVR_CMD_ChangeVGA(pHead,pBuf);
			break;

		case KMV_SVR_CMD_DVI_CHANGE:
			//change DVI
			KMV_SVR_CMD_ChangeDVI(pHead,pBuf);
			break;

		case KMV_SVR_CMD_LINE_CHANGE:
			//change Line
			KMV_SVR_CMD_ChangeLine(pHead,pBuf);
			break;

		case KMV_SVR_CMD_MIC_CHANGE:
			//change Mic
			KMV_SVR_CMD_ChangeMic(pHead,pBuf);
			break;

		case KMV_SVR_CMD_MOUSE_CHANGE:
			//change mouse
			KMV_SVR_CMD_ChangeMouse(pHead,pBuf);
			break;
		case KMV_SVR_CMD_REMOTE_DECODE:
			//change Decode channel
			KMV_SVR_CMD_ChangeDecodeChn(pHead,pBuf);
			break;
		case KMV_SVR_CMD_AUDIO_CHANGE:
			KMV_SVR_CMD_ChangeAudio(pHead,pBuf);
			break;

		case KMV_SVR_CMD_UART_SET:
			KMV_SVR_CMD_ChangeUART(pHead,pBuf);
			break;
		case KMV_SVR_CMD_SEARCH_INFO:
			//search info
			KMV_SVR_CMD_SearchInfo(pHead,pBuf);
			break;

		default :
			ret = -1;
			KMV_SVR_CMD_Ask(pHead,pBuf,ret);
			break;
		
	}
	return SVR_OK;
}
