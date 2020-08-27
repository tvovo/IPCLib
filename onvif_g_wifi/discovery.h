#ifndef DISCOVERY_H
#define DISCOVERY_H
#include  <pthread.h>
#include "stdsoap2.h"

struct DiscoveryApp
{
	struct soap soap;
    pthread_t threadId;
	int 	running;
};


int discoveryInit(struct DiscoveryApp *disApp);
void discoveryFini(struct DiscoveryApp *disApp);
int discoveryRun(struct DiscoveryApp *disApp);

#endif
