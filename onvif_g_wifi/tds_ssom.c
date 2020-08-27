#include "soapStub.h"
#include "gsoapapp.h"
#include "time.h"
#include "sys/time.h"
#include "sys/types.h"


SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse)
{
	static struct tds__Service ser;
	static struct tt__OnvifVersion over;
	static struct _tds__Service_Capabilities tscap;
	static char X_Addr[128];
    static int mnop = 10;
    static char OSDXML[]="<trt:Capabilities SnapshotUri=\"true\" Rotation=\"false\" VideoSourceMode=\"true\" OSD=\"true\" TemporaryOSDText=\"false\" EXICompression=\"false\"><trt:ProfileCapabilities MaximumNumberOfProfiles=\"10\" /><trt:StreamingCapabilities RTPMulticast=\"false\" RTP_TCP=\"true\" RTP_RTSP_TCP=\"true\" NonAggregateControl=\"false\" NoRTSPStreaming=\"false\" /></trt:Capabilities>";
	ser.Namespace = "http://www.onvif.org/ver10/media/wsdl";
	sprintf(X_Addr, "http://%s/onvif/media_service", get_local_ip("eth0"));  
	ser.XAddr = X_Addr;
	ser.Version = &over;
	over.Major = 19;
	over.Minor = 6;
	ser.Capabilities = &tscap;
	tscap.__any = OSDXML;

	tds__GetServicesResponse->Service = &ser;
	tds__GetServicesResponse->__sizeService =1;
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(struct soap* soap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(struct soap* soap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);

	tds__GetDeviceInformationResponse->Manufacturer = "qddytt";
	tds__GetDeviceInformationResponse->Model = "ipc";
	tds__GetDeviceInformationResponse->FirmwareVersion="v1.0";
	tds__GetDeviceInformationResponse->SerialNumber="sn12345678";
	tds__GetDeviceInformationResponse->HardwareId="12345";
	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(struct soap* soap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{
	struct tm _tm;
	struct timeval tv;
	time_t timep;

	_tm.tm_sec = tds__SetSystemDateAndTime->UTCDateTime->Time->Second;
	_tm.tm_min = tds__SetSystemDateAndTime->UTCDateTime->Time->Minute;
	if(tds__SetSystemDateAndTime->UTCDateTime->Time->Hour+8 >= 24)
	{
		_tm.tm_hour= tds__SetSystemDateAndTime->UTCDateTime->Time->Hour+8-24;
		_tm.tm_mday= tds__SetSystemDateAndTime->UTCDateTime->Date->Day+1;		
	}
	else
	{
		_tm.tm_hour= tds__SetSystemDateAndTime->UTCDateTime->Time->Hour+8;
		_tm.tm_mday= tds__SetSystemDateAndTime->UTCDateTime->Date->Day;		
	}
	_tm.tm_mon = tds__SetSystemDateAndTime->UTCDateTime->Date->Month-1;
	_tm.tm_year= tds__SetSystemDateAndTime->UTCDateTime->Date->Year-1900;
	timep = mktime(&_tm);
	tv.tv_sec = timep;
	tv.tv_usec = 0;
	if(settimeofday(&tv,(struct timezone *)0)<0)
	{
		printf("Set system datatime error!/n");
	}
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(struct soap* soap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{
	time_t timep;
	struct tm* pLocalTime;

	static struct tt__SystemDateTime putt;

	static struct tt__TimeZone timz;
	static struct tt__SystemDateTimeExtension ste;
	static struct tt__DateTime puttime;
	static struct tt__DateTime puttimeutc;

    static struct tt__Time Tim;	/* required element of type tt:Time */
	static struct tt__Date Dat;	/* required element of type tt:Date */
	static struct tt__Time Timutc;	/* required element of type tt:Time */
	static struct tt__Date Datutc;	/* required element of type tt:Date */
	
	time(&timep);
	pLocalTime = localtime(&timep);
    printf("today is %4d-%02d-%02d %02d:%02d:%02d\n",pLocalTime->tm_year+1900,pLocalTime->tm_mon+1,pLocalTime->tm_mday,pLocalTime->tm_hour,pLocalTime->tm_min,pLocalTime->tm_sec);
	
	Tim.Second = pLocalTime->tm_sec;
	Tim.Minute = pLocalTime->tm_min;
	Tim.Hour = pLocalTime->tm_hour;
	Dat.Day = pLocalTime->tm_mday;	
	Dat.Month = pLocalTime->tm_mon+1;	
	Dat.Year = pLocalTime->tm_year+1900;
	puttime.Time = &Tim;
	puttime.Date = &Dat; 
	putt.LocalDateTime = &puttime;

	Timutc.Second = pLocalTime->tm_sec;
	Timutc.Minute = pLocalTime->tm_min;
	if(pLocalTime->tm_hour<8)
	{
		Timutc.Hour = 24 + pLocalTime->tm_hour - 8;
		Datutc.Day = pLocalTime->tm_mday-1;
	}
	else
	{
		Timutc.Hour =  pLocalTime->tm_hour - 8;
		Datutc.Day = pLocalTime->tm_mday;		
	}	
	Datutc.Month = pLocalTime->tm_mon+1;	
	Datutc.Year = pLocalTime->tm_year+1900;
	puttimeutc.Time = &Timutc;
	puttimeutc.Date = &Datutc;
	putt.UTCDateTime = &puttimeutc;

	timz.TZ="ChinaStandardTime-8";
	putt.TimeZone = &timz;

    ste.__size = 0;
	ste.__any = "cshh-time";
	putt.Extension = &ste;

	putt.DateTimeType = tt__SetDateTimeType__Manual;
	putt.DaylightSavings = xsd__boolean__false_;			
	putt.__anyAttribute= "cshh-time";

	tds__GetSystemDateAndTimeResponse->SystemDateAndTime = &putt;
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(struct soap* soap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(struct soap* soap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(struct soap* soap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(struct soap* soap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(struct soap* soap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(struct soap* soap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    static char* WsdlUrl = "http://www.onvif.org/onvif/ver10/analytics/wsdl/analytics.wsdl \
     http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl \
     http://www.onvif.org/onvif/ver10/event/wsdl/event.wsdl \
     http://www.onvif.org/onvif/ver10/imaging/wsdl/imaging.wsdl \
     http://www.onvif.org/onvif/ver10/media/wsdl/media.wsdl \
     http://www.onvif.org/onvif/ver10/ptz/wsdl/ptz.wsdl \
     http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl \
     http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl";
     tds__GetWsdlUrlResponse->WsdlUrl = WsdlUrl;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);

    enum tt__CapabilityCategory CapabilityCategory;
    if (!tds__GetCapabilities->Category) {
        CapabilityCategory = tt__CapabilityCategory__All;
    } else {
        CapabilityCategory =  *(tds__GetCapabilities->Category);
    }
    if (CapabilityCategory != tt__CapabilityCategory__All &&
        CapabilityCategory != tt__CapabilityCategory__Analytics &&
        CapabilityCategory != tt__CapabilityCategory__Device &&
        CapabilityCategory != tt__CapabilityCategory__Events &&
        CapabilityCategory != tt__CapabilityCategory__Imaging &&
        CapabilityCategory != tt__CapabilityCategory__Media &&
        CapabilityCategory != tt__CapabilityCategory__PTZ) {
        fprintf(stderr, "The requested WSDL service category is not supported by the NVT");
        //return soap_sender_fault_subcode(soap, "env:Receiver/ter:ActionNotSupported/ter:NoSuchService", "The requested WSDL service category is not supported by the NVT", NULL); ;
        return SOAP_FAULT;
    }
    static struct tt__Capabilities tCapabilities;
    soap_default_tt__Capabilities(soap, &tCapabilities);
    tds__GetCapabilitiesResponse->Capabilities = &tCapabilities;

    /* tt:AnalyticsCapabilities */
    if ((CapabilityCategory == tt__CapabilityCategory__All) || (CapabilityCategory == tt__CapabilityCategory__Analytics)) {
        static struct tt__AnalyticsCapabilities tAnalytics;
        soap_default_tt__AnalyticsCapabilities(soap, &tAnalytics);
        tCapabilities.Analytics = &tAnalytics;
        static char Analytics_Addr[256];
        sprintf(Analytics_Addr, "%sanalytics", gApp->xAddrRoot);

        tAnalytics.XAddr = Analytics_Addr;
        tAnalytics.RuleSupport = xsd__boolean__false_;
        tAnalytics.AnalyticsModuleSupport = xsd__boolean__false_;
    }

    /* tt:DeviceCapabilities */
    if ((CapabilityCategory == tt__CapabilityCategory__All) || (CapabilityCategory == tt__CapabilityCategory__Device)) {
        static struct tt__DeviceCapabilities tDevice;
        soap_default_tt__DeviceCapabilities(soap, &tDevice);
        tCapabilities.Device = &tDevice;
        static char Device_Addr[64];
        sprintf(Device_Addr, "%sdevice_service", gApp->xAddrRoot);
        tDevice.XAddr = Device_Addr;
        /* tt:NetworkCapabilities */
        static struct tt__NetworkCapabilities tNetwork;
        soap_default_tt__NetworkCapabilities(soap, &tNetwork);
        tDevice.Network = &tNetwork;
        static enum xsd__boolean  enIPFilter         = xsd__boolean__true_;
        static enum xsd__boolean  enZeroConfiguration = xsd__boolean__false_;
        static enum xsd__boolean  enIPVersion6       = xsd__boolean__false_;
        static enum xsd__boolean  enDynDNS           = xsd__boolean__true_;
        tNetwork.IPFilter                 = &enIPFilter;
        tNetwork.ZeroConfiguration        = &enZeroConfiguration;
        tNetwork.IPVersion6               = &enIPVersion6;
        tNetwork.DynDNS                   = &enDynDNS;
        static struct tt__NetworkCapabilitiesExtension tNetworkCapabilitiesExtension;
        soap_default_tt__NetworkCapabilitiesExtension(soap, &tNetworkCapabilitiesExtension);
        tNetwork.Extension = &tNetworkCapabilitiesExtension;
        static enum xsd__boolean enDot11Configuration;
        tNetworkCapabilitiesExtension.Dot11Configuration = &enDot11Configuration;
        /* tt:SystemCapabilities */
        static struct tt__SystemCapabilities tSystemCapabilities;
        soap_default_tt__SystemCapabilities(soap, &tSystemCapabilities);
        tDevice.System = &tSystemCapabilities;
        tSystemCapabilities.DiscoveryResolve = xsd__boolean__false_;
        tSystemCapabilities.DiscoveryBye = xsd__boolean__true_;
        tSystemCapabilities.RemoteDiscovery = xsd__boolean__true_;
        tSystemCapabilities.SystemBackup = xsd__boolean__false_;
        tSystemCapabilities.SystemLogging = xsd__boolean__false_;
        tSystemCapabilities.FirmwareUpgrade = xsd__boolean__true_;
        tSystemCapabilities.__sizeSupportedVersions = 3;
        static struct tt__OnvifVersion tSupportedVersions[3];
        tSupportedVersions[0].Major                    = 2;
        tSupportedVersions[0].Minor                    = 2;
        tSupportedVersions[1].Major                    = 2;
        tSupportedVersions[1].Minor                    = 1;
        tSupportedVersions[2].Major                    = 2;
        tSupportedVersions[2].Minor                    = 0;
        tSystemCapabilities.SupportedVersions = tSupportedVersions;
        static struct tt__SystemCapabilitiesExtension tSystemCapabilitiesExtension;
        soap_default_tt__SystemCapabilitiesExtension(soap, &tSystemCapabilitiesExtension);
        tSystemCapabilities.Extension = &tSystemCapabilitiesExtension;
        static enum xsd__boolean enHttpFirmwareUpgrade = xsd__boolean__true_;
        static enum xsd__boolean enHttpSystemBackup = xsd__boolean__false_;
        static enum xsd__boolean enHttpSystemLogging = xsd__boolean__false_;
        static enum xsd__boolean enHttpSupportInformation = xsd__boolean__false_;
        tSystemCapabilitiesExtension.HttpFirmwareUpgrade = &enHttpFirmwareUpgrade;
        tSystemCapabilitiesExtension.HttpSystemBackup = &enHttpSystemBackup;
        tSystemCapabilitiesExtension.HttpSystemLogging = &enHttpSystemLogging;
        tSystemCapabilitiesExtension.HttpSupportInformation = &enHttpSupportInformation;
        /* tt:IOCapabilities */
        static struct tt__IOCapabilities tIO;
        soap_default_tt__IOCapabilities(soap, &tIO);
        tDevice.IO = &tIO;

        static int iInputConnectors = 1;
        tIO.InputConnectors = &iInputConnectors;
        static int iRelayOutputs = 1;
        tIO.RelayOutputs = &iRelayOutputs;
        static struct tt__IOCapabilitiesExtension tIOCapabilitiesExtension;
        soap_default_tt__IOCapabilitiesExtension(soap, &tIOCapabilitiesExtension);
        tIO.Extension = &tIOCapabilitiesExtension;
        static enum xsd__boolean enAuxiliary = xsd__boolean__false_;
        tIOCapabilitiesExtension.Auxiliary = &enAuxiliary;
        tIOCapabilitiesExtension.__sizeAuxiliaryCommands = 1;
        static char chAuxiliaryCommands[] = "nothing";
        static char* p = chAuxiliaryCommands;
        tIOCapabilitiesExtension.AuxiliaryCommands = &p;//&chAuxiliaryCommands;
        static struct tt__IOCapabilitiesExtension2 tIOCapabilitiesExtension2;
        soap_default_tt__IOCapabilitiesExtension2(soap, &tIOCapabilitiesExtension2);
        tIOCapabilitiesExtension.Extension = &tIOCapabilitiesExtension2;
		tDevice.Security = NULL;
	}
		tCapabilities.Events = NULL;
	/* tt:ImagingCapabilities */
		 tCapabilities.Imaging = NULL;

    if ((CapabilityCategory == tt__CapabilityCategory__All) || (CapabilityCategory == tt__CapabilityCategory__Media)) {
        static struct  tt__MediaCapabilities tMedia;
        soap_default_tt__MediaCapabilities(soap, &tMedia);
        tCapabilities.Media = &tMedia;
        static char Media_Addr[64];
        sprintf(Media_Addr, "%sMedia", gApp->xAddrRoot);
        tMedia.XAddr = Media_Addr;
        static struct tt__RealTimeStreamingCapabilities tRealTimeStreamingCapabilities;
        soap_default_tt__RealTimeStreamingCapabilities(soap, &tRealTimeStreamingCapabilities);
        tMedia.StreamingCapabilities = &tRealTimeStreamingCapabilities;
        static enum xsd__boolean  enRTPMulticast         = xsd__boolean__false_;
        static enum xsd__boolean  enRTP_USCORETCP = xsd__boolean__true_;
        static enum xsd__boolean  enRTP_USCORERTSP_USCORETCP       = xsd__boolean__true_;
        tRealTimeStreamingCapabilities.RTPMulticast = &enRTPMulticast;
        tRealTimeStreamingCapabilities.RTP_USCORETCP = &enRTP_USCORETCP;
        tRealTimeStreamingCapabilities.RTP_USCORERTSP_USCORETCP = &enRTP_USCORERTSP_USCORETCP;
        static struct tt__MediaCapabilitiesExtension tMediaCapabilitiesExtension;
        soap_default_tt__MediaCapabilitiesExtension(soap, &tMediaCapabilitiesExtension);
        tMedia.Extension = &tMediaCapabilitiesExtension;
        static struct tt__ProfileCapabilities tProfileCapabilities;
        soap_default_tt__ProfileCapabilities(soap, &tProfileCapabilities);
        tMediaCapabilitiesExtension.ProfileCapabilities = &tProfileCapabilities;
        tProfileCapabilities.MaximumNumberOfProfiles = 3;
    }

    tCapabilities.PTZ = NULL;
	tCapabilities.Extension = NULL;

	fprintf(stderr, "GetCapabilities ok....\n");
    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(struct soap* soap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{
#if 0
10966 struct tt__NetworkInterface
10967 {
10968     char *token;    /* required attribute of type tt:ReferenceToken */
10969     enum xsd__boolean Enabled;  /* required element of type xsd:boolean */
10970     struct tt__NetworkInterfaceInfo *Info;  /* optional element of type tt:NetworkInterfaceInfo */
10971     struct tt__NetworkInterfaceLink *Link;  /* optional element of type tt:NetworkInterfaceLink */
10972     struct tt__IPv4NetworkInterface *IPv4;  /* optional element of type tt:IPv4NetworkInterface */
10973     struct tt__IPv6NetworkInterface *IPv6;  /* optional element of type tt:IPv6NetworkInterface */
10974     struct tt__NetworkInterfaceExtension *Extension;    /* optional element of type tt:NetworkInterfaceExtension */
10975     char *__anyAttribute;   /* optional attribute of type xsd:anyType */
10976 };


 1943 struct _tds__GetNetworkInterfacesResponse
 1944 {
 1945     int __sizeNetworkInterfaces;    /* SOAP 1.2 RPC return element (when namespace qualified) */    /* sequence of elements <NetworkInterfaces> */
 1946     struct tt__NetworkInterface *NetworkInterfaces; /* required element of type tt:NetworkInterface */
 1947 };

#endif
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(struct soap* soap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(struct soap* soap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(struct soap* soap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(struct soap* soap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(struct soap* soap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(struct soap* soap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(struct soap* soap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(struct soap* soap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(struct soap* soap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(struct soap* soap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(struct soap* soap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(struct soap* soap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(struct soap* soap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(struct soap* soap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(struct soap* soap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(struct soap* soap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(struct soap* soap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(struct soap* soap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(struct soap* soap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(struct soap* soap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(struct soap* soap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(struct soap* soap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(struct soap* soap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(struct soap* soap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(struct soap* soap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(struct soap* soap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(struct soap* soap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(struct soap* soap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(struct soap* soap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(struct soap* soap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfigurations(struct soap* soap, struct _tds__GetStorageConfigurations *tds__GetStorageConfigurations, struct _tds__GetStorageConfigurationsResponse *tds__GetStorageConfigurationsResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateStorageConfiguration(struct soap* soap, struct _tds__CreateStorageConfiguration *tds__CreateStorageConfiguration, struct _tds__CreateStorageConfigurationResponse *tds__CreateStorageConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetStorageConfiguration(struct soap* soap, struct _tds__GetStorageConfiguration *tds__GetStorageConfiguration, struct _tds__GetStorageConfigurationResponse *tds__GetStorageConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetStorageConfiguration(struct soap* soap, struct _tds__SetStorageConfiguration *tds__SetStorageConfiguration, struct _tds__SetStorageConfigurationResponse *tds__SetStorageConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteStorageConfiguration(struct soap* soap, struct _tds__DeleteStorageConfiguration *tds__DeleteStorageConfiguration, struct _tds__DeleteStorageConfigurationResponse *tds__DeleteStorageConfigurationResponse)
{
	printf("%s:%d\n",__FUNCTION__,__LINE__);
	return SOAP_OK;
}
