#include <stdio.h>
#include "soapH.h"
#include "soapStub.h"
#include "wsddapi.h"

int __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse)
{
printf("%s,%d\n",__FUNCTION__, __LINE__);
	return 0;
}

int __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);
	return 0;
}

int __tdn__Probe(struct soap *soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);
	return 0;
}
