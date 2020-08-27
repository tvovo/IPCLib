#include <stdio.h>
#include "utils.h"
#include "soapH.h"
#include "wsddapi.h"

void wsdd_event_Hello(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const     char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);

}

void wsdd_event_Bye(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);

}

soap_wsdd_mode wsdd_event_Probe(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *Types, const char *Scopes, const char *MatchBy, struct wsdd__ProbeMatchesType *matches)
{

	static char xAddr[128] = {0};
    printf("%s,%d\n",__FUNCTION__, __LINE__);
    printf("MessageID:%s\n", MessageID);
    printf("ReplyTo:%s\n", ReplyTo);
    printf("Types:%s\n", Types);
    printf("Scopes:%s\n", Scopes);
    printf("MatchBy:%s\n", MatchBy);

	sprintf(xAddr, "http://%s:80/onvif/device_service", get_local_ip("eth0"));//eth0
	printf("xAddr = %s\n", xAddr);
	soap_wsdd_init_ProbeMatches(soap, matches);
	soap_wsdd_add_ProbeMatch(soap, matches,
	"urn:uuid:464A4854-4656-5242-4530-313035394100",
	"tdn:NetworkVideoTransmitter",
	"onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/type/audio_encoder onvif://www.onvif.org/type/ptz onvif://www.onvif.org/type/video_analytics onvif://www.onvif.org/hardware/HD-IPCAM 		 onvif://www.onvif.org/location/country/china onvif://www.onvif.org/name/IPCAM",
	NULL,
	xAddr,
    10);
    return SOAP_WSDD_MANAGED;

}

void wsdd_event_ProbeMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType     *matches)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);
}


soap_wsdd_mode wsdd_event_Resolve(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *EndpointReference, struct wsdd__ResolveMatchType *match)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);

    return 0;
}

void wsdd_event_ResolveMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ResolveMatchType *match)
{
	printf("%s,%d\n",__FUNCTION__, __LINE__);

}






