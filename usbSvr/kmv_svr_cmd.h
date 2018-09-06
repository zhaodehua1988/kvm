#ifndef _KMV_SVR_CMD_H_H_
#define _KMV_SVR_CMD_H_H_
#include "kmv_svr.h"

#define KMV_SVR_CMD_RESET 0x01
#define KMV_SVR_CMD_ASK   0x02
#define KMV_SVR_CMD_HANDLE 0x03
#define KMV_SVR_CMD_SET_LOCAL_IP  0x04
#define KMV_SVR_CMD_SET_LOCAL_ADDR 0x05
#define KMV_SVR_CMD_SET_REMOTE_ADDR 0x06
#define KMV_SVR_CMD_UART_PORT1 0x07
#define KMV_SVR_CMD_UART_PORT2 0x08
#define KMV_SVR_CMD_UART_PORT3 0x09
#define KMV_SVR_CMD_UART_PORT4 0x0a
#define KMV_SVR_CMD_VGA_CHANGE 0x10
#define KMV_SVR_CMD_DVI_CHANGE 0x11
#define KMV_SVR_CMD_LINE_CHANGE 0x12
#define KMV_SVR_CMD_MIC_CHANGE 0x13
#define KMV_SVR_CMD_MOUSE_CHANGE 0x14
#define KMV_SVR_CMD_REMOTE_DECODE 0x15
#define KMV_SVR_CMD_AUDIO_CHANGE 0x16

#define KMV_SVR_CMD_SEARCH_INFO 0x30

int KMV_SVR_CMD_Proc(SVR_HEAD_E *pHead,unsigned char *pBuf);
#endif

