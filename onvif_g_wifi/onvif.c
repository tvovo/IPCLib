#include "soapH.h"  
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "gsoapapp.h"
#include "discovery.h"

struct GsoapApp* gApp = NULL;
struct DiscoveryApp* disApp = NULL;
int myOSD_Init(void);

int main(int argc, char* argv[])
{
	gApp = (struct GsoapApp*)calloc(1, sizeof(*gApp));
	disApp = (struct DiscoveryApp*)calloc(1, sizeof(*disApp));
	
	myOSD_Init();
	
	discoveryInit(disApp);
	discoveryRun(disApp);

	gsoapAppInit(gApp);
	gsoapAppRun(gApp, 1);

	while(1) {
		sleep(1);
	}

	discoveryFini(disApp);
	gsoapAppFini(gApp);
	
	free(gApp);
	free(disApp);

	return 0;
} 



int SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
	printf("get error code\n");
	return 0;
}

int SOAP_ENV__Fault_ex(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
	printf("get error code_ex\n");
	return 0;
}




