#ifndef _KMV_USB_H_H_
#define _KMV_USB_H_H_


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "ext_dvi.h"
#include "kmv_svr_udp.h"
#include "tsk_sw.h"

#define KMV_USB_DEBUG_MODE

#ifdef  KMV_USB_DEBUG_MODE
#define KMV_USB_printf(...) \
  				 do {\
  				  printf("\n\rUSB:" __VA_ARGS__);\
  				  fflush(stdout); \
  				  } while(0)
#else 
  #define KMV_USB_printf(...) 
#endif

#define KMV_USB_DEVICE_DATAMAX_LEN 32


#define KMV_USB_HUB_PHYS1    "usb-hiusb-ehci-1.1.1"
#define KMV_USB_HUB_PHYS2 	 "usb-hiusb-ehci-1.1.2"
#define KMV_USB_HUB_PHYS3    "usb-hiusb-ehci-1.1.3"
#define KMV_USB_HUB_PHYS4    "usb-hiusb-ehci-1.1.4"
#define KMV_USB_HUB_PHYS5    "usb-hiusb-ehci-1.4.1"
#define KMV_USB_HUB_PHYS6    "usb-hiusb-ehci-1.4.2"
#define KMV_USB_HUB_PHYS7    "usb-hiusb-ehci-1.4.3"
#define KMV_USB_HUB_PHYS8    "usb-hiusb-ehci-1.4.4"

typedef struct KMV_USB_DEVICE_INFO{
	pthread_t thrHndl;
	int plug;  //1:plug in 0:plug out
	int name;//1:Keyboard 2:mouse -1:no dev
	char chn;
	char ena;
	char level;// priority  level
	char physDev[KMV_USB_DEVICE_DATAMAX_LEN];
	char devType;//0 vga 1 dvi
	char handler[KMV_USB_DEVICE_DATAMAX_LEN];
	unsigned char keyState[121];
	unsigned char specialKey;

}KMV_USB_DEVICE_INFO;

int KMV_MOUSE_Proc(KMV_USB_DEVICE_INFO *pInfo);
int KMV_KEYBD_Proc(KMV_USB_DEVICE_INFO *pInfo);
int KMV_USB_DEV_SetChl(KMV_USB_DEVICE_INFO *pInfo,unsigned char *pBuf,int len);
int KMV_USB_DEV_Open();
int KMV_USB_DEV_Close();

int KMV_USB_DEV_ChlSw(unsigned char outChn,unsigned char inChn,unsigned char ena,unsigned char devType);
int KMV_MOUSE_ChangeMouseFrequency(int freq);
int KMV_USB_DEV_ChangeLevel(int chl,int level);

int KMV_USB_DEV_getRemoteChn(int devType);
int KMV_USB_DEV_changeRemoteChn(int devType,int chl,int level);


int KMV_KEYBD_PopAllKey(KMV_USB_DEVICE_INFO *pInfo);
#endif


