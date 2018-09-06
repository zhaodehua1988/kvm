#ifndef  _TSK_UART_RS232_H_
#define  _TSK_UART_RS232_H_
#include "wv_common.h" 
WV_S32  TSK_UART_Open();
WV_S32  TSK_UART_Close();
WV_S32  TSK_UART_SetPort(WV_U32 port);
WV_S32 TSK_UART_GetPort(WV_U32 * pPort);
WV_S32 TSK_UART_SetSpeed(WV_U32 baud);
WV_S32 TSK_UART_GetSpeed(WV_U32 * pBaud);
#endif
