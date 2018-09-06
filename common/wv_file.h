#ifndef _WV_COMM_FILE_H_
#define _WV_COMM_FILE_H_
#include "wv_common.h" 
WV_S32   WV_FILE_Access(WV_S8 *filename);
WV_S32   WV_FILE_GetLen(WV_S8 *filenameAll,WV_S32 *pLen);
WV_S32   WV_FILE_Remove(WV_S8 *filenameAll);
WV_S32   WV_FILE_Cpy(WV_S8 *filenameScr,WV_S8 *filenameDst);
WV_S32   WV_FILE_GetAvail(WV_S8 *filenameAll,WV_S32 * pLenM);
#endif
