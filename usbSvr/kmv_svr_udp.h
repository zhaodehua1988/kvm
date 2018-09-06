#ifndef KMV_SVR_UDP_H_H_
#define KVM_SVR_UDP_H_H_
#include "kmv_svr.h"

int KMV_UDP_ClientGetAddr(char *ip,int port);
int KMV_UDP_ClientSend(char *buf,int len,unsigned char cmd,unsigned char devType);
int KMV_UDP_ClientSetRemoteChl(int chl,int level);

#endif

