#include"stream_pkg.h"

 /***********************************************

WV_S32  STRM_PackAnlyse(WV_U8 *pBuf,STRM_PACK_INFO_S *pInfo); 

************************************************/
WV_S32  STRM_PackAnlyse(WV_U8 *pBuf,STRM_PACK_INFO_S *pInfo)
{
 //clr sync flag
    pInfo->sync = 0;  
    
 // check sync is 0x47 
 if(pBuf[0] != 0x47 )
  {
    pInfo->sync = 0;  
    return WV_EFAIL;
  } 
  
// get flag end
  pInfo->endFlag = 0;   
 if( (pBuf[1] &  0XF0)== 0X80)
 {
   pInfo->endFlag = 1;  
 } 
  // get payload start flag
  
 pInfo->pid  =  pBuf[2];
 
 //get cc 
 pInfo->cc  = pBuf[3] & 0XFF;
 
 pInfo->loadOffset   =  STRM_PAG_HEAD_LEN ;
 pInfo->loadLen      =  *((WV_U16*)(pBuf + 12) ); 
 	if(pInfo->loadLen > STRM_PAG_LOAD_LEN)
 	{ 
 	  return WV_EFAIL;   
 	}
 
 pInfo->pLoad        =  pBuf +  STRM_PAG_HEAD_LEN;
 pInfo->txPts        = *((WV_U64 *)(pBuf+4));
 pInfo->sync = 1;
 return WV_SOK;
   
}

