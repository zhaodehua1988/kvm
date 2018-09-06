#include "stream_pes.h"

#define  BIT0	0x01
#define  BIT1	0x02
#define  BIT2	0x04
#define  BIT3	0x08
#define  BIT4	0x10
#define  BIT5	0x20
#define  BIT6	0x40
#define  BIT7	0x80



/***********************************************

WV_S32  STRM_EsAnlyse(WV_U8 *pBuf,STRM_ES_INFO_S *pInfo,WV_U32 len); 

************************************************/
WV_S32  STRM_EsAnlyse(WV_U8 *pBuf,STRM_ES_INFO_S *pInfo,WV_U32 len)
{
	 WV_U32  start;
	 WV_U64  pts;
	  // set all 0
	 memset(pInfo,0,sizeof(struct STRM_ES_INFO_S));
	 
	 if(len <10)
	 {  
	    WV_ERROR("PES len erro :  len = %d \r\n",len);
	   return -1;
	 }
	 start =0;
	 while(pInfo-> sync == 0)
	 {
		   if((pBuf[start]==0x00) && (pBuf[start+1]==0x00) && (pBuf[start+2]==0x01))
		   {
			pInfo-> sync = 1;
			break;
		   }
		   start++;
		   if(start == len-9)
		   {
		    WV_ERROR("PES sync erro :  start = %d len = %d \r\n",start,len);
			return WV_EFAIL;
		   } 
	 }
	
	pInfo -> streamId = pBuf[start+3];
	
	pInfo -> lenPack = ((pBuf[start+4]& 0XFF)<<8) | (pBuf[start+5] & 0XFF);
	
	if(((pBuf[start+6]>>6) & 0X03) != 0x02)
	{
	 pInfo-> sync = 0;
	   WV_ERROR("PES deagexterro:0x%x \r\n",pBuf[start+6]);
	  return WV_EFAIL;
	}
	
	
	pInfo -> lenHead = pBuf[start+8]; 
	
         if(start > len-pBuf[start+8])
		   {
		     WV_ERROR("PES len erro 2:  start = %d len = %d  lenHead = %d \r\n",start,len,pBuf[start+8]);
		     pInfo-> sync = 0;
			return -1;
		   } 
	 
	if( (pBuf[start+7] & BIT7) == BIT7)
	{
	 pInfo -> ptsFlag = 1;
	 pts =  (pBuf[start+9]>>1)&0x07;
 	 pts  =  (pts<<8) |  (pBuf[start+10] & 0xFF);
 	 pts  =  (pts<<7) |  ((pBuf[start+11]>>1) & 0x7F);
 	 pts  =  (pts<<8) |  (pBuf[start+12] & 0xFF);
 	 pts  =  (pts<<7) |  ((pBuf[start+13]>>1) & 0x7F);
     pInfo -> pts =  pts;
	}
	 
     pInfo ->loadLen = len - 9 - pInfo->lenHead;
	 if( pInfo ->loadLen < 0 ) 
	 { 
	   WV_ERROR("PES loadLen <0\r\n");
	   pInfo-> sync = 0;
	   return WV_EFAIL;
	 }
	 pInfo->pLoad  = pBuf + 9 + pInfo->lenHead;
	 
	return WV_SOK;
    
}

