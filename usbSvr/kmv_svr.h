#ifndef _KMV_SVR_TOP_H_
#define _KMV_SVR_TOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#define SVR_ERR -1
#define SVR_OK  0

//#define KMV_SVR_DEBUG_MODE

#ifdef  KMV_SVR_DEBUG_MODE
#define KMV_SVR_printf(...) \
  				 do {\
  				  printf("\n\rSVR:" __VA_ARGS__);\
  				  fflush(stdout); \
  				  } while(0)
#else 
  #define KMV_SVR_printf(...) 
#endif

#ifdef  KMV_SVR_DEBUG_MODE
#define KMV_SVR_error(...) \
  				 do {\
				  printf("\r\n<fun:%s line:%d>",__FUNCTION__,__LINE__);\
  				  printf("SVR err:" __VA_ARGS__);\
  				  fflush(stdout); \
  				  } while(0)
#else 
  #define KMV_SVR_error(...) 
#endif


typedef struct SVR_HEAD_E 
{
	unsigned int sync;
	unsigned char dataNum; 
} SVR_HEAD_E; 


int   SVR_Open();
int   SVR_Close();

#endif

