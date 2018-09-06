#ifndef _WV_TELNET_CMD_H_
#define _WV_TELNET_CMD_H_
#include "wv_common.h" 
WV_S32   WV_TEL_Open(); 
WV_S32   WV_TEL_Close();
WV_S32  WV_TELNET_printf(WV_S8 * pBuf);
#endif
