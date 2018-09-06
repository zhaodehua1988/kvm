#include"stream_send.h"
#include"tsk_streamRx.h"   
#include"his.h" 
/*******************************************************************************************

WV_S32  STRM_Send(WV_ETH_UDP_TX_E *pDev,WV_U8  *pBuf, WV_U32 len,WV_U8 pid,WV_U32 *pCnt)

*******************************************************************************************/

WV_S32  STRM_SendEs(WV_ETH_UDP_TX_E *pDev,WV_U8 *pBuf, WV_U32 len,WV_U8 pid,WV_U32 *pCnt)
{

	WV_U8 sockBuf[2048];
	WV_U8 *esBuf,*pCur;
	WV_U32 esLen,fillLen;
	WV_S32 ret;
		esLen = len;
		esBuf = pBuf;
		while(esLen >0)
    	{
			sockBuf[0] = 0x47;  
			if(esLen > STRM_PAG_LOAD_LEN)
			{
				sockBuf[1] = 0x40;
				fillLen = STRM_PAG_LOAD_LEN;
			}
			else  //end of ES
			{
				sockBuf[1] = 0x80; 
				fillLen = esLen;
			}
		 	
		 	sockBuf[2] = pid;
			sockBuf[3]  = *pCnt & 0xff;
			
			
			HIS_SYS_GetCurPts( (WV_U64 *)(sockBuf+4) );
			//*((WV_U64 *)(sockBuf+4)) = ptsUs;
			*(WV_U16 *)(sockBuf+12) = fillLen;
			pCur = sockBuf + STRM_PAG_HEAD_LEN;
			memcpy(pCur,esBuf, fillLen);
			esLen    = esLen -fillLen;
			esBuf    = esBuf +fillLen;
		    (*pCnt)++;
		    //ret=TSK_SRX_TestTx(sockBuf,fillLen + STRM_PAG_HEAD_LEN);//loop for stream rx
			ret = WV_UDP_Tx(pDev,sockBuf, fillLen + STRM_PAG_HEAD_LEN);

			if(ret != WV_SOK )			
			{ 
			  return  WV_EFAIL; 
			}     	

		} 

	return  WV_SOK;  
}

