#ifndef GSOAPAPP_H
#define GSOAPAPP_H
#include  <pthread.h>
#include "stdsoap2.h"


struct GsoapApp
{
    int running;
    int bindPort;
    int forked;
    int masterSocket;
    int slaveSocket;
    char hostAddress[128];
    char xAddr[128];
    char xAddrRoot[128];
    struct soap soap;
    pthread_t threadId;
};

extern struct GsoapApp* gApp;

int gsoapAppInit(struct GsoapApp* app);
void gsoapAppFini(struct GsoapApp* app);
int gsoapAppRun(struct GsoapApp* app, int forked);

#endif
