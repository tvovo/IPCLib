#include "soapStub.h"
#include "utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define UNIX_DOMAIN "/tmp/UNIX.domain"
#include <pthread.h>
int SOAP_ENV__Fault_Ex(struct soap* soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
    return 0;
}

static int GetProfile(struct soap* soap, char* token, void* prof)
{

    static struct tt__VideoSourceConfiguration            vsrc[2];
    static struct tt__AudioSourceConfiguration            asrc;
    static struct tt__IntRectangle                        vrect;
    static struct tt__MulticastConfiguration              mcast;
    static struct tt__IPAddress                           mcast_addr;
    static enum xsd__boolean enfixed = xsd__boolean__false_;
    static char* mcast_saddr = "0.0.0.0";
    static int mcast_port  = 6601;
    int sizeProfiles = 2, i=0;

    // Allocate memory for user list.
    int profiles_size = sizeof(struct tt__Profile) * sizeProfiles;
    int vrcl = sizeof(struct tt__VideoRateControl) * sizeProfiles;
    int vrlo = sizeof(struct tt__VideoResolution)  * sizeProfiles;
    int vrln = sizeof(struct tt__H264Configuration)  * sizeProfiles;
    int ecfg = sizeof(struct tt__VideoEncoderConfiguration) * sizeProfiles;
    int aerc = sizeof(struct tt__AudioEncoderConfiguration) * sizeProfiles;
    int aopc = sizeof(struct tt__AudioOutputConfiguration) * sizeProfiles;
    int extn = sizeof(struct tt__ProfileExtension) * sizeProfiles;
    struct tt__Profile* tProfiles = (struct tt__Profile*)soap_malloc(soap, profiles_size);
    struct tt__VideoResolution* vres = (struct tt__VideoResolution*)soap_malloc(soap, vrlo);
    struct tt__H264Configuration* vren = (struct tt__H264Configuration*)soap_malloc(soap, vrln);
    struct tt__VideoRateControl*  vrctl = (struct tt__VideoRateControl*)soap_malloc(soap, vrcl);
    struct tt__VideoEncoderConfiguration* venc = (struct tt__VideoEncoderConfiguration*)soap_malloc(soap, ecfg);
    struct tt__AudioEncoderConfiguration* aevc = (struct tt__AudioEncoderConfiguration*)soap_malloc(soap, aerc);
    struct tt__AudioOutputConfiguration * aoce = (struct tt__AudioOutputConfiguration*)soap_malloc(soap, aopc);
    struct tt__ProfileExtension *pfex = (struct tt__ProfileExtension*)soap_malloc(soap, extn);
    memset(tProfiles, 0, profiles_size);

    memset(vres, 0, vrlo);
    memset(vren, 0, vrln);
    memset(venc, 0, ecfg);
    memset(vrctl, 0, vrcl);
    memset(aevc, 0, aerc);
    memset(aoce, 0, aopc);
    memset(pfex, 0, extn);

    if (!tProfiles) {
        return SOAP_FAULT;
    }
    //check profiles is exist
   // if (token) {
   //     if(strcmp(token, "media_profile1")!=0 && strcmp(token, "media_profile2")!=0)
   //         return soap_sender_fault_subcode(soap, "ter:InvalidArgVal/ter:NoProfile", "The requested profile token ProfileToken does not exist.", NULL);
   // }
    // Fill each user information.
    for (i=0; i < sizeProfiles; i++) {
        // Profile
        
        if(i==1){
        tProfiles[i].Name   = "media_profile2";
        tProfiles[i].token  = "media_profile2";
        venc[i].token       = "video_encoder_config2";
        venc[i].Name        = "video_encoder_config2";

        venc[i].UseCount    = 2;
        venc[i].Encoding    = tt__VideoEncoding__H264;
        venc[i].Quality     = 2;
        aevc[i].token       = "Audio_encoder_config2";
        aevc[i].Name        = "Audio_encoder_config2";

        aevc[i].UseCount    = 2;

        aevc[i].Encoding = 0;
        aevc[i].Bitrate = 64;
        aevc[i].SampleRate = 8;
        vrect.height            = 576;
        vrect.width             = 720;//1080 720 *蹇呴』涓€鏍凤紝鍚﹀垯娴嬭瘯涓嶉€氳�?
        vrect.y                 = 1;
        vrect.x                 = 1;
        vsrc[i].Name           = "video_source_config2";
        vsrc[i].token          = "video_source_config2";
        vsrc[i].SourceToken    = "video_source2";
        vsrc[i].UseCount       = 2;//vsctg.UseCount;
        vsrc[i].Bounds         = &vrect;
        //dbc_get_audio_source_configurations(gWsApp->fDBC, &ascts);
        asrc.Name           = "AudioSourceConfig";
        asrc.UseCount       = 2;
        asrc.token          = "AudioSourceConfigToken";
        asrc.SourceToken    = "AudioSourceChannel1";
        vrctl[i].FrameRateLimit            = 30;
        vrctl[i].BitrateLimit              = 4000;
        vres[i].Height                     = 720;
        vres[i].Width                      = 1280;
        mcast_addr.Type          = tt__IPType__IPv4;
        mcast_addr.IPv4Address   = mcast_saddr;
        mcast.Address                = &mcast_addr;
        mcast.Port                   = mcast_port;
        mcast.TTL                = 64;
        mcast.AutoStart          = xsd__boolean__false_;
        venc[i].Multicast   = &mcast;
        aevc[i].Multicast   = &mcast;
        venc[i].H264        = &vren[i];    
        venc[i].RateControl = &vrctl[i];
        venc[i].Resolution  = &vres[i];
        static LONG64 SmTimeout;
        soap_s2xsd__duration(soap,"PT100S",&SmTimeout);
        venc[i].SessionTimeout = SmTimeout;
        aevc[i].SessionTimeout = SmTimeout;
        aoce[i].Name = "AudioOutputConfigName";
        aoce[i].UseCount = 2;
        aoce[i].token = "AudioOutputConfigToken";
        aoce[i].OutputToken = "AudioOutputToken";
        aoce[i].SendPrimacy = "www.onvif.org/ver20/HalfDuplex/Server";
        aoce[i].OutputLevel = 10;
        pfex[i].AudioOutputConfiguration = &aoce[i];
        tProfiles[i].PTZConfiguration   =  NULL;

        tProfiles[i].VideoEncoderConfiguration = &venc[i];
        tProfiles[i].VideoSourceConfiguration  = &vsrc[i];

        tProfiles[i].AudioEncoderConfiguration = &aevc[i];
        tProfiles[i].AudioSourceConfiguration  = &asrc;
        tProfiles[i].Extension  = &pfex[i];
        tProfiles[i].fixed    = &enfixed;
        if (token) {
            if (strcmp(token, tProfiles[i].Name) == 0) {
                ((struct _trt__GetProfileResponse*)prof)->Profile = &tProfiles[i];
                return SOAP_OK;
            }
        }
            
        }
        if(i==0){
        tProfiles[i].Name   = "media_profile1";
        tProfiles[i].token  = "media_profile1";
        venc[i].token       = "video_encoder_config1";
        venc[i].Name        = "video_encoder_config1";

        venc[i].UseCount    = 2;
        venc[i].Encoding    = tt__VideoEncoding__H264;
        venc[i].Quality     = 2;
        aevc[i].token       = "Audio_encoder_config1";
        aevc[i].Name        = "Audio_encoder_config1";

        aevc[i].UseCount    = 2;

        aevc[i].Encoding = 0;
        aevc[i].Bitrate = 64;
        aevc[i].SampleRate = 8;
        vrect.height            = 1080;
        vrect.width             = 1920;//1080 720 *蹇呴』涓€鏍凤紝鍚﹀垯娴嬭瘯涓嶉€氳�?
        vrect.y                 = 1;
        vrect.x                 = 1;
        vsrc[i].Name           = "video_source_config1";
        vsrc[i].token          = "video_source_config1";
        vsrc[i].SourceToken    = "video_source1";
        vsrc[i].UseCount       = 2;//vsctg.UseCount;
        vsrc[i].Bounds         = &vrect;
        //dbc_get_audio_source_configurations(gWsApp->fDBC, &ascts);
        asrc.Name           = "AudioSourceConfig";
        asrc.UseCount       = 2;
        asrc.token          = "AudioSourceConfigToken";
        asrc.SourceToken    = "AudioSourceChannel1";
        vrctl[i].FrameRateLimit            = 30;
        vrctl[i].BitrateLimit              = 4000;
        vres[i].Height                     = 720;
        vres[i].Width                      = 1280;
        mcast_addr.Type          = tt__IPType__IPv4;
        mcast_addr.IPv4Address   = mcast_saddr;
        mcast.Address                = &mcast_addr;
        mcast.Port                   = mcast_port;
        mcast.TTL                = 64;
        mcast.AutoStart          = xsd__boolean__false_;
        venc[i].Multicast   = &mcast;
        aevc[i].Multicast   = &mcast;
        venc[i].H264        = &vren[i];    
        venc[i].RateControl = &vrctl[i];
        venc[i].Resolution  = &vres[i];
        static LONG64 SmTimeout;
        soap_s2xsd__duration(soap,"PT100S",&SmTimeout);
        venc[i].SessionTimeout = SmTimeout;
        aevc[i].SessionTimeout = SmTimeout;
        aoce[i].Name = "AudioOutputConfigName";
        aoce[i].UseCount = 2;
        aoce[i].token = "AudioOutputConfigToken";
        aoce[i].OutputToken = "AudioOutputToken";
        aoce[i].SendPrimacy = "www.onvif.org/ver20/HalfDuplex/Server";
        aoce[i].OutputLevel = 10;
        pfex[i].AudioOutputConfiguration = &aoce[i];
        tProfiles[i].PTZConfiguration   =  NULL;

        tProfiles[i].VideoEncoderConfiguration = &venc[i];
        tProfiles[i].VideoSourceConfiguration  = &vsrc[i];

        tProfiles[i].AudioEncoderConfiguration = &aevc[i];
        tProfiles[i].AudioSourceConfiguration  = &asrc;
        tProfiles[i].Extension  = &pfex[i];
        tProfiles[i].fixed    = &enfixed;
        if (token) {
            if (strcmp(token, tProfiles[i].Name) == 0) {
                ((struct _trt__GetProfileResponse*)prof)->Profile = &tProfiles[i];
                return SOAP_OK;
            }
        }
        }

    }
    printf("name0-%s name1-%s\n",tProfiles[0].Name,tProfiles[1].Name);
    ((struct _trt__GetProfilesResponse*)prof)->__sizeProfiles = sizeProfiles;
    ((struct _trt__GetProfilesResponse*)prof)->Profiles       = tProfiles;
    return SOAP_OK;
}


int __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoSources(struct soap* soap, struct _trt__GetVideoSources *trt__GetVideoSources, struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    static struct tt__VideoSource tVideoSources[2];
    static struct tt__VideoResolution tResolution[2];
    tVideoSources[0].token      = "video_source1";
    tVideoSources[0].Framerate  = 25;
    tVideoSources[0].Resolution = &tResolution[0];
    tResolution[0].Height       = 1080;//tvs.ResolutionHeight;
    tResolution[0].Width        = 1920;//tvs.ResolutionWidth;
    tVideoSources[1].token      = "video_source2";
    tVideoSources[1].Framerate  = 25;
    tVideoSources[1].Resolution = &tResolution[1];
    tResolution[1].Height       = 576;//tvs.ResolutionHeight;
    tResolution[1].Width        = 720;//tvs.ResolutionWidth;
    trt__GetVideoSourcesResponse->__sizeVideoSources = 2;
    trt__GetVideoSourcesResponse->VideoSources     = &tVideoSources;
    return SOAP_OK;
}

int __trt__GetAudioSources(struct soap* soap, struct _trt__GetAudioSources *trt__GetAudioSources, struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioOutputs(struct soap* soap, struct _trt__GetAudioOutputs *trt__GetAudioOutputs, struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__CreateProfile(struct soap* soap, struct _trt__CreateProfile *trt__CreateProfile, struct _trt__CreateProfileResponse *trt__CreateProfileResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetProfile(struct soap* soap, struct _trt__GetProfile *trt__GetProfile, struct _trt__GetProfileResponse *trt__GetProfileResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    return GetProfile(soap, trt__GetProfile->ProfileToken, trt__GetProfileResponse);
}

int __trt__GetProfiles(struct soap* soap, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{

    printf("%s:%d\n",__FUNCTION__,__LINE__);
    return GetProfile(soap, NULL, trt__GetProfilesResponse);
}

int __trt__AddVideoEncoderConfiguration(struct soap* soap, struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddVideoSourceConfiguration(struct soap* soap, struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddAudioEncoderConfiguration(struct soap* soap, struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddAudioSourceConfiguration(struct soap* soap, struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddPTZConfiguration(struct soap* soap, struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddVideoAnalyticsConfiguration(struct soap* soap, struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddMetadataConfiguration(struct soap* soap, struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddAudioOutputConfiguration(struct soap* soap, struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__AddAudioDecoderConfiguration(struct soap* soap, struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveVideoEncoderConfiguration(struct soap* soap, struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveVideoSourceConfiguration(struct soap* soap, struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveAudioEncoderConfiguration(struct soap* soap, struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveAudioSourceConfiguration(struct soap* soap, struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemovePTZConfiguration(struct soap* soap, struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveVideoAnalyticsConfiguration(struct soap* soap, struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveMetadataConfiguration(struct soap* soap, struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveAudioOutputConfiguration(struct soap* soap, struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__RemoveAudioDecoderConfiguration(struct soap* soap, struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__DeleteProfile(struct soap* soap, struct _trt__DeleteProfile *trt__DeleteProfile, struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoSourceConfigurations(struct soap* soap, struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{
    static struct tt__VideoSourceConfiguration videosourceft[2];
    static struct tt__IntRectangle tBounds[2]; /* required element of type tt:IntRectangle */
    trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations    = 2;
    videosourceft[0].token             = "video_source_config1";
    videosourceft[0].Name              = "video_source_config1";
    videosourceft[0].UseCount          = 2;
    videosourceft[0].SourceToken       = "video_source1";
    tBounds[0].height                  = 1080;
    tBounds[0].width                   = 1920; //涓巊etprofile閲屼竴锟�??
    tBounds[0].y                       = 1;
    tBounds[0].x                       = 1;
    videosourceft[0].Bounds    = &tBounds[0];
    videosourceft[1].token             = "video_source_config2";
    videosourceft[1].Name              = "video_source_config2";
    videosourceft[1].UseCount          = 2;
    videosourceft[1].SourceToken       = "video_source2";
    tBounds[1].height                  = 576;
    tBounds[1].width                   = 720; //涓巊etprofile閲屼竴锟�??
    tBounds[1].y                       = 1;
    tBounds[1].x                       = 1;
    videosourceft[0].Bounds    = &tBounds[1];

    trt__GetVideoSourceConfigurationsResponse->Configurations  = &videosourceft;
    return SOAP_OK;
}

int __trt__GetVideoEncoderConfigurations(struct soap* soap, struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioSourceConfigurations(struct soap* soap, struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioEncoderConfigurations(struct soap* soap, struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetMetadataConfigurations(struct soap* soap, struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioOutputConfigurations(struct soap* soap, struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioDecoderConfigurations(struct soap* soap, struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoSourceConfiguration(struct soap* soap, struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    static struct tt__VideoSourceConfiguration tdConfiguration;
    tdConfiguration.Name        = "video_source_config1";
    tdConfiguration.UseCount    = 2;
    tdConfiguration.token       = "video_source_config1";
    tdConfiguration.SourceToken = "video_source1";
    static struct tt__IntRectangle tBounds;
    tBounds.height              = 1080;
    tBounds.width               = 1920;   // should same as getprofile
    tBounds.y                   = 1;
    tBounds.x                   = 1;
    tdConfiguration.Bounds  = &tBounds;
    trt__GetVideoSourceConfigurationResponse->Configuration   = &tdConfiguration;
    return SOAP_OK;
}

int __trt__GetVideoEncoderConfiguration(struct soap* soap, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioSourceConfiguration(struct soap* soap, struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioEncoderConfiguration(struct soap* soap, struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetMetadataConfiguration(struct soap* soap, struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioOutputConfiguration(struct soap* soap, struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioDecoderConfiguration(struct soap* soap, struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleVideoEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleVideoSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleAudioEncoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleAudioSourceConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap* soap, struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleMetadataConfigurations(struct soap* soap, struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleAudioOutputConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetCompatibleAudioDecoderConfigurations(struct soap* soap, struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetVideoSourceConfiguration(struct soap* soap, struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetVideoEncoderConfiguration(struct soap* soap, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetAudioSourceConfiguration(struct soap* soap, struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetAudioEncoderConfiguration(struct soap* soap, struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetVideoAnalyticsConfiguration(struct soap* soap, struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetMetadataConfiguration(struct soap* soap, struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetAudioOutputConfiguration(struct soap* soap, struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetAudioDecoderConfiguration(struct soap* soap, struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoSourceConfigurationOptions(struct soap* soap, struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetVideoEncoderConfigurationOptions(struct soap* soap, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    static struct tt__VideoEncoderConfigurationOptions sVideoEncodeConfigureationOptions;
    static struct tt__IntRange sIntqualittRange;
    static struct tt__JpegOptions sJpegop;
    static struct tt__VideoResolution stResolutionsAvailables[4];
    static struct tt__IntRange stFrameRateRanges;
    static struct tt__H264Options stH264s;
    static struct tt__VideoResolution stdResolutionsAvailables[4];
    static struct tt__IntRange stdFrameRateRange;
    static enum tt__H264Profile stdH264ProfilesSupport[4];
    static struct tt__VideoEncoderOptionsExtension stdvideoeencoderextension;
    static struct tt__JpegOptions2 stdjpeg;
    static struct tt__VideoResolution stJResolutionsAvailables[4];
    static struct tt__IntRange stJFrameRateRanges;
    static struct tt__IntRange stJdBitrateRange;
    static struct tt__H264Options2 stdh264;
    static struct tt__VideoResolution stdhResolutionsAvailables[5];
    static struct tt__IntRange stdhFrameRateRange;
    static enum tt__H264Profile stdhH264ProfilesSupport[4];
    static struct tt__IntRange stsdBitrateRange;

    char chConfigurationToken[128];
    if ((!trt__GetVideoEncoderConfigurationOptions->ConfigurationToken && !trt__GetVideoEncoderConfigurationOptions->ProfileToken) || !trt__GetVideoEncoderConfigurationOptions->ConfigurationToken ) {
        sprintf(chConfigurationToken, "%s", "video_encoder_config1");
    } else {
        sprintf(chConfigurationToken, "%s", trt__GetVideoEncoderConfigurationOptions->ConfigurationToken);
    }
    //static struct VideoEncoderConfigurationOptionsTable tpc;
    if (strcmp(chConfigurationToken, "video_encoder_config1") == 0) {
        sIntqualittRange.Min = 0;
        sIntqualittRange.Max = 5;

        sJpegop.__sizeResolutionsAvailable = 4;
        stResolutionsAvailables[0].Width      = 1920;
        stResolutionsAvailables[0].Height     = 1080;
        stResolutionsAvailables[1].Width      = 1280;
        stResolutionsAvailables[1].Height     = 720;
        stResolutionsAvailables[2].Width      = 720;
        stResolutionsAvailables[2].Height     = 576;
        stResolutionsAvailables[3].Width      = 720;
        stResolutionsAvailables[3].Height     = 480;

        stFrameRateRanges.Min              = 1;
        stFrameRateRanges.Max              = 30;

        stH264s.__sizeResolutionsAvailable     = 4;
        stdResolutionsAvailables[0].Width      = 1920;
        stdResolutionsAvailables[0].Height     = 1080;
        stdResolutionsAvailables[1].Width      = 1280;
        stdResolutionsAvailables[1].Height     = 720;
        stdResolutionsAvailables[2].Width      = 720;
        stdResolutionsAvailables[2].Height     = 576;
        stdResolutionsAvailables[3].Width      = 720;
        stdResolutionsAvailables[3].Height     = 480;
        
        stdFrameRateRange.Min         = 1;
        stdFrameRateRange.Max         = 30;

        stdH264ProfilesSupport[0] = tt__H264Profile__Baseline;
        stdH264ProfilesSupport[1] = tt__H264Profile__Main;
        stdH264ProfilesSupport[2] = tt__H264Profile__Extended;
        stdH264ProfilesSupport[3] = tt__H264Profile__High;
        stH264s.__sizeH264ProfilesSupported            = 4;

        stdjpeg.__sizeResolutionsAvailable = 4;
        stJResolutionsAvailables[0].Width      = 1920;
        stJResolutionsAvailables[0].Height     = 1080;
        stJResolutionsAvailables[1].Width      = 1280;
        stJResolutionsAvailables[1].Height     = 720;
        stJResolutionsAvailables[2].Width      = 720;
        stJResolutionsAvailables[2].Height     = 576;
        stJResolutionsAvailables[3].Width      = 720;
        stJResolutionsAvailables[3].Height     = 480;
        
        stJFrameRateRanges.Min              = 1;
        stJFrameRateRanges.Max              = 30;

       
        stJdBitrateRange.Min  = 512;
        stJdBitrateRange.Max  = 8192;

        stdh264.__sizeResolutionsAvailable     = 5;
        stdhResolutionsAvailables[0].Width      = 1920;
        stdhResolutionsAvailables[0].Height     = 1080;
        stdhResolutionsAvailables[1].Width      = 1280;
        stdhResolutionsAvailables[1].Height     = 720;
        stdhResolutionsAvailables[2].Width      = 720;
        stdhResolutionsAvailables[2].Height     = 576;
        stdhResolutionsAvailables[3].Width      = 720;
        stdhResolutionsAvailables[3].Height     = 480;


        stdhFrameRateRange.Min         = 1;
        stdhFrameRateRange.Max         = 30;

        stdhH264ProfilesSupport[0] = tt__H264Profile__Baseline;
        stdhH264ProfilesSupport[1] = tt__H264Profile__Main;
        stdhH264ProfilesSupport[2] = tt__H264Profile__Extended;
        stdhH264ProfilesSupport[3] = tt__H264Profile__High;
        stdh264.__sizeH264ProfilesSupported            = 4;

        stsdBitrateRange.Min                            = 512;//
        stsdBitrateRange.Max                            = 8192;//
 
        stdh264.BitrateRange                           = &stsdBitrateRange;
        stdh264.H264ProfilesSupported                  = stdhH264ProfilesSupport;

        stdh264.FrameRateRange                         = &stdhFrameRateRange;
        stdh264.ResolutionsAvailable                   = stdhResolutionsAvailables;
        stdvideoeencoderextension.H264                 = &stdh264;
        sVideoEncodeConfigureationOptions.Extension    = &stdvideoeencoderextension;
        stdjpeg.FrameRateRange                         = &stJFrameRateRanges;

        stdjpeg.BitrateRange                           = &stJdBitrateRange;
        stdjpeg.ResolutionsAvailable                   = stJResolutionsAvailables;
        stdvideoeencoderextension.JPEG                 = &stdjpeg;
        sVideoEncodeConfigureationOptions.Extension    = &stdvideoeencoderextension;
        stH264s.H264ProfilesSupported                  = stdH264ProfilesSupport;

        stH264s.FrameRateRange                         = &stdFrameRateRange;
        stH264s.ResolutionsAvailable                   = stdResolutionsAvailables;
        sJpegop.FrameRateRange                         = &stFrameRateRanges;
        sJpegop.ResolutionsAvailable                   = stResolutionsAvailables;
        sVideoEncodeConfigureationOptions.H264         = &stH264s;
        sVideoEncodeConfigureationOptions.JPEG         = &sJpegop;
        sVideoEncodeConfigureationOptions.QualityRange = &sIntqualittRange;
        trt__GetVideoEncoderConfigurationOptionsResponse->Options = &sVideoEncodeConfigureationOptions;
    } else if (strcmp(chConfigurationToken, "video_encoder_config2") ==  0) {
               
        static struct tt__VideoEncoderConfigurationOptions VideoEncodeConfigureationOptions;
        static struct tt__IntRange IntqualittRange;
        static struct tt__JpegOptions Jpegop;
        static struct tt__VideoResolution tResolutionsAvailables[2];
        static struct tt__IntRange tFrameRateRanges;
        static struct tt__H264Options tH264s;
        static struct tt__VideoResolution tdResolutionsAvailables[2];
        static struct tt__IntRange tdFrameRateRange;
        static enum tt__H264Profile tdH264ProfilesSupport[4];
        static struct tt__VideoEncoderOptionsExtension tdvideoeencoderextension;
        static struct tt__JpegOptions2 tdjpeg;
        static struct tt__VideoResolution tJResolutionsAvailables[2];
        static struct tt__IntRange tdBitrateRange;    
        static struct tt__IntRange tJFrameRateRanges;
        static struct tt__H264Options2 tdhh264;
        static struct tt__VideoResolution thResolutionsAvailables[2];
        static enum tt__H264Profile tdhH264ProfilesSupport[4];
        static struct tt__IntRange sthFrameRateRange;
        static struct tt__IntRange tsdBitrateRange;

        IntqualittRange.Min = 1;
        IntqualittRange.Max = 5;
        Jpegop.__sizeResolutionsAvailable = 2;
        tResolutionsAvailables[0].Width   = 720;
        tResolutionsAvailables[0].Height  = 576;
        tResolutionsAvailables[1].Width   = 720;
        tResolutionsAvailables[1].Height  = 480;

        tFrameRateRanges.Min              = 1;
        tFrameRateRanges.Max              = 30;

        tH264s.__sizeResolutionsAvailable  = 2;
        tdResolutionsAvailables[0].Width   = 720;
        tdResolutionsAvailables[0].Height  = 576;
        tdResolutionsAvailables[1].Width   = 720;
        tdResolutionsAvailables[1].Height  = 480;

        tdFrameRateRange.Min         = 1;
        tdFrameRateRange.Max         = 30;
        tdH264ProfilesSupport[0] = tt__H264Profile__Baseline;
        tdH264ProfilesSupport[1] = tt__H264Profile__Main;
        tdH264ProfilesSupport[2] = tt__H264Profile__Extended;
        tdH264ProfilesSupport[3] = tt__H264Profile__High;
        tH264s.__sizeH264ProfilesSupported  = 4;

        tdjpeg.__sizeResolutionsAvailable = 2;
        tJResolutionsAvailables[0].Width   = 720;
        tJResolutionsAvailables[0].Height  = 576;
        tJResolutionsAvailables[1].Width   = 720;
        tJResolutionsAvailables[1].Height  = 480;

        tdBitrateRange.Min                            =  1000;//tpc.extension.JpegBitrateRange.Min
        tdBitrateRange.Max                            = 4000;//tpc.extension.JpegBitrateRange.Max

        tJFrameRateRanges.Min              = 1;
        tJFrameRateRanges.Max              = 30;

        tdhh264.__sizeResolutionsAvailable  = 2;
        thResolutionsAvailables[0].Width   = 720;
        thResolutionsAvailables[0].Height  = 576;
        thResolutionsAvailables[1].Width   = 720;
        thResolutionsAvailables[1].Height  = 480;

        
        tdhH264ProfilesSupport[0] = tt__H264Profile__Baseline;
        tdhH264ProfilesSupport[1] = tt__H264Profile__Main;
        tdhH264ProfilesSupport[2] = tt__H264Profile__Extended;
        tdhH264ProfilesSupport[3] = tt__H264Profile__High;
        tdhh264.__sizeH264ProfilesSupported            = 4;

        sthFrameRateRange.Min         = 1;
        sthFrameRateRange.Max         = 30;
        
        tsdBitrateRange.Min                            = 1000;
        tsdBitrateRange.Max                            = 4000;
        tdhh264.BitrateRange                           = &tsdBitrateRange;
        tdhh264.H264ProfilesSupported                  = tdhH264ProfilesSupport;
        tdhh264.FrameRateRange                         = &sthFrameRateRange;
        tdhh264.ResolutionsAvailable                   = thResolutionsAvailables;
        tdvideoeencoderextension.H264                 = &tdhh264;
        tdjpeg.FrameRateRange                         = &tJFrameRateRanges;
        tdjpeg.BitrateRange                           = &tdBitrateRange;
        tdjpeg.ResolutionsAvailable                   = tJResolutionsAvailables;
        tdvideoeencoderextension.JPEG                 = &tdjpeg;
        VideoEncodeConfigureationOptions.Extension    = &tdvideoeencoderextension;
        tH264s.H264ProfilesSupported                  = tdH264ProfilesSupport;
        tH264s.FrameRateRange                         = &tdFrameRateRange;
        tH264s.ResolutionsAvailable                   = tdResolutionsAvailables;
        Jpegop.FrameRateRange                         = &tFrameRateRanges;
        Jpegop.ResolutionsAvailable                   = tResolutionsAvailables;
        VideoEncodeConfigureationOptions.H264         = &tH264s;
        VideoEncodeConfigureationOptions.JPEG         = &Jpegop;
        VideoEncodeConfigureationOptions.QualityRange = &IntqualittRange;
        trt__GetVideoEncoderConfigurationOptionsResponse->Options   = &VideoEncodeConfigureationOptions;
    } else {
        return SOAP_FAULT;
    }
    return SOAP_OK;
}

int __trt__GetAudioSourceConfigurationOptions(struct soap* soap, struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioEncoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetMetadataConfigurationOptions(struct soap* soap, struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioOutputConfigurationOptions(struct soap* soap, struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetAudioDecoderConfigurationOptions(struct soap* soap, struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap* soap, struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetStreamUri(struct soap* soap, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
        if (trt__GetStreamUri->StreamSetup) {
        if (trt__GetStreamUri->StreamSetup->Stream == 1) {
            return soap_sender_fault_subcode(soap, "ter:InvalidArgVal/ter:InvalidStreamSetup", "Specification of StreamType or Transport part in StreamSetup is not supported.", NULL);
        }
        if (trt__GetStreamUri->StreamSetup->Transport->Protocol== 3) {
            return soap_sender_fault_subcode(soap, "ter:InvalidArgVal/ter:InvalidStreamSetup", "The HTTP is not supported.", NULL);
        }
    } else {
        return soap_sender_fault_subcode(soap, "ter:InvalidArgVal/ter:GetStreamUri", "Invalid GetStreamUri.", NULL);
    }
    static struct tt__MediaUri tMediaUri;
    static char Dev_Addr[64];
    
    if (strcmp(trt__GetStreamUri->ProfileToken, "media_profile1") ==  0) {
        sprintf(Dev_Addr, "rtsp://%s/test.264", get_local_ip("eth0"));  // 娴佸湴鍧�?
    } else    if (strcmp(trt__GetStreamUri->ProfileToken, "media_profile2") ==  0) {
        sprintf(Dev_Addr, "rtsp://%s/test2.264",  get_local_ip("eth0"));
    } else {
        return SOAP_ERR;
    }
    tMediaUri.Uri                 = Dev_Addr;
    tMediaUri.InvalidAfterConnect = xsd__boolean__false_;
    tMediaUri.InvalidAfterReboot  = xsd__boolean__false_;
    static LONG64 SmTimeout;
    soap_s2xsd__duration(soap,"PT100S",&SmTimeout);
    tMediaUri.Timeout             = SmTimeout;//"PT100S";
    trt__GetStreamUriResponse->MediaUri = &tMediaUri;
    return SOAP_OK;
}

int __trt__StartMulticastStreaming(struct soap* soap, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__StopMulticastStreaming(struct soap* soap, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetSynchronizationPoint(struct soap* soap, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__GetSnapshotUri(struct soap* soap, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    static struct tt__MediaUri tMediaUri;
    static char Dev_Addr[64];

    sprintf(Dev_Addr, "http://%s/images/snapshot.jpeg",  get_local_ip("eth0")); // //eth0 onvif鍦板�?
    tMediaUri.Uri                   = Dev_Addr;//"http://192.168.10.128/images/snapshot.png";
    tMediaUri.InvalidAfterConnect   = xsd__boolean__false_;
    tMediaUri.InvalidAfterReboot    = xsd__boolean__false_;
    static LONG64 SmTimeout;
    soap_s2xsd__duration(soap,"PT100S",&SmTimeout);
    tMediaUri.Timeout               = SmTimeout;//"PT100S";
    trt__GetSnapshotUriResponse->MediaUri                = &tMediaUri;
    return SOAP_OK;
}

int __trt__GetVideoSourceModes(struct soap* soap, struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}

int __trt__SetVideoSourceMode(struct soap* soap, struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse)
{
	
	printf("%s:%d\n",__FUNCTION__,__LINE__);
}
static int connect_fd;
static int InitOsdProSync = 0;
struct myosd_text
{
    char  Head;
    char  token[64];
    char  VideoSourceConfigurationToken[64];	
    char  PosType[12];
    float PosVector_x;
    float PosVector_y;
    char  PlainText[128];
    int   fontsize;
    uint32_t  Check;
};
static struct myosd_text myosd[3];

static int CHECK_calc(struct myosd_text *params)
{
    uint32_t i,len,chk=0;
    uint8_t *p;

    if(NULL == params)return 0;
    if(0x5a != params->Head && 0xa5 != params->Head)return 0;

    p = (uint8_t *)params;
    chk=0;
    len = sizeof(struct myosd_text) - 4;
    for(i=0;i<len;i++)
    {
        chk += *p;
        p++;
    }
    params->Check = chk;
    return 1;
}
static int CHECK_ok(struct myosd_text *params)
{
    uint32_t i,len,chk=0;
    uint8_t *p;

    if(NULL == params)return 0;
    if(0x5a != params->Head)return 0;

    p = (uint8_t *)params;

    len = sizeof(struct myosd_text) - 4;
    for(i=0;i<len;i++)
    {
        chk += *p;
        p++;
    }
    if( chk == params->Check) return 1;

    return 0;
}
static int paramsSave(void)
{
    FILE *fp;
    int wr_ret;
    unsigned long file_size;
    struct myosd_text rmyosd[3];

    fp = fopen( "a.bin", "rb+" );
    if(fp == NULL)
    {
        fp = fopen( "a.bin", "wb+" );
        if(fp == NULL)
        return 0;
    }

    rewind(fp);
    fread((char *)rmyosd, sizeof(rmyosd), 1, fp);
    if(myosd[0].Head != 0xa5)
    {   
        memcpy(&rmyosd[0],&myosd[0],sizeof(myosd[0]));
    }
    if(myosd[1].Head != 0xa5)
    {   
        memcpy(&rmyosd[1],&myosd[1],sizeof(myosd[1]));
    }
    if(myosd[2].Head != 0xa5)
    {
        memcpy(&rmyosd[2],&myosd[2],sizeof(myosd[2]));
    }
     rmyosd[0].Head = 0x5a;
     rmyosd[1].Head = 0x5a;
     rmyosd[2].Head = 0x5a;

    if(CHECK_calc(&rmyosd[0]) != 0 && CHECK_calc(&rmyosd[1]) != 0 && CHECK_calc(&rmyosd[2]) != 0)
    {
        rewind(fp);
        wr_ret = fwrite( (char *)rmyosd, sizeof(rmyosd), 1, fp);
        printf("wr_ret = %d\n", wr_ret);        
    }
    else return 0;

    fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	printf("file_size = %lu\n", file_size);

    fclose(fp);
 
 	return 1;
}
// int myOSD_Init(void)
// {
//     int wr_ret;
//     int rd_ret;
//     FILE *fp;
//     unsigned long file_size;
//     struct myosd_text rmyosd[3];
//     char checks;

//     myosd[0].Head = 0x5a;
//     myosd[1].Head = 0x5a;
//     myosd[2].Head = 0x5a;
//     sprintf(myosd[0].token,"video_source1\0");
//     sprintf(myosd[1].token,"video_source2\0");
//     sprintf(myosd[2].token,"video_source3\0");
//     if (CHECK_calc(&myosd[0]) == 0)return -1;
//     if (CHECK_calc(&myosd[1]) == 0)return -1;
//     if (CHECK_calc(&myosd[2]) == 0)return -1;

//     fp = fopen( "a.bin", "rb+" );//鏂囦欢杩藉姞锛屽彲璇诲彲鍐欙紝鏂囦欢涓嶅瓨鍦ㄥ垯鍒涘�?
//     if(fp == NULL)
//     {
//         fp = fopen( "a.bin", "wb+" );
//         if(fp == NULL)
//         return -1;
//     }
//     rewind(fp);
//     wr_ret = fwrite( (char *)myosd, sizeof(myosd), 1, fp);
// 	printf("wr_ret = %d\n", wr_ret);
 
// 	rewind(fp);//涓婇潰鐨勫啓鎿嶄綔锛屾枃浠朵綅缃亸绉婚噺涔熶細鐩稿簲鐨勭Щ鍔紝姝ゅ灏嗘枃浠跺亸绉诲埌鏂囦欢寮€濮嬩綅缃紝鐒跺悗鎵嶈兘璇诲彇鍒氬垰杈撳叆鐨勫唴瀹�
// 	rd_ret = fread((char *)rmyosd, sizeof(rmyosd), 1, fp);
// 	printf("rd_ret = %d\n",rd_ret);
//     if(strcmp(myosd[0].token,rmyosd[0].token) != 0 || strcmp(myosd[1].token,rmyosd[1].token) != 0 || strcmp(myosd[2].token,rmyosd[2].token) != 0)
//     {
//         printf("error wj\n");
//     }
//     else
//     {
//         if (CHECK_ok(&rmyosd[0]) != 0 && CHECK_ok(&rmyosd[1]) != 0 && CHECK_ok(&rmyosd[2]) != 0)
//         printf("ok wj\n");
//     }
// 	fseek(fp, 0, SEEK_END);
// 	file_size = ftell(fp);
// 	printf("file_size = %lu\n", file_size);
 
// 	fclose(fp);//鍏抽棴鏂囦欢
 
// 	return 0;
// }
static pthread_t OSDProSyncPid;
static int osdsend;
static void* OSDProSync_Process(void* arg)
{
    struct sockaddr_un srv_addr;
    int ret,flags;

    printf("ONVIF����...\n");
    do
    { 
        srv_addr.sun_family=AF_UNIX;
        strcpy(srv_addr.sun_path,UNIX_DOMAIN);
        ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
        /* code */
    } while (ret == -1);
    printf("ONVIF���ӵ�RTSP\n"); 
    write(connect_fd,(char *)myosd,sizeof(myosd));
    flags = fcntl(connect_fd, F_GETFL, 0);
    fcntl(connect_fd, F_SETFL, flags | O_NONBLOCK);  
    while(1)
    {
        if(recv(connect_fd,&ret,sizeof(ret),MSG_DONTWAIT) == 0)
        {
            printf("ONVIF���ӶϿ�\n");
            close(connect_fd);
            while(1)
            {
                sleep(3600);                
            }
        }
        else if(osdsend == 1)
        {
            printf("msg parm to RTSP\n");
            write(connect_fd,(char *)myosd,sizeof(myosd));
            osdsend = 0;
        }

        sleep(1);
    }
}
int myOSD_Init(void)
{
    int rd_ret;
    int kk=0;
    FILE *fp;
    unsigned long file_size;
    struct myosd_text rmyosd[3];

    fp = fopen( "a.bin", "rb+" );
    if(fp == NULL)
    {
        kk=1;
        fp = fopen( "a.bin", "wb+" );
        if(fp == NULL)
        return -1;
    }
 
	rewind(fp);
	rd_ret = fread((char *)rmyosd, sizeof(rmyosd), 1, fp);
	printf("rd_ret = %d\n",rd_ret);
    if (CHECK_ok(&rmyosd[0]) != 0 && CHECK_ok(&rmyosd[1]) != 0 && CHECK_ok(&rmyosd[2]) != 0)
    {  
        memcpy(myosd,rmyosd,sizeof(myosd));
        printf("ok wj %s - %s - %s\n",myosd[0].token,myosd[1].token,myosd[2].token);
    }
    else printf("error wj\n");
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	printf("file_size = %lu\n", file_size);
 
	fclose(fp);
    if(kk==1)
    {   
        myosd[0].Head = 0x5a;
        myosd[1].Head = 0x5a;
        myosd[2].Head = 0x5a;
        myosd[0].token[0] = 0;
        myosd[1].token[0] = 0;
        myosd[2].token[0] = 0;
        kk=0;
        CHECK_calc(&myosd[0]);
        CHECK_calc(&myosd[1]);
        CHECK_calc(&myosd[2]);
        paramsSave();
    }

    //***进程通讯开�?
    connect_fd=socket(PF_UNIX,SOCK_STREAM,0);
    if(connect_fd<0)
    {
        perror("cannot create communication socket");
        return 1;
    } 
    osdsend = 0;
    pthread_create(&OSDProSyncPid,0,OSDProSync_Process,NULL);
    InitOsdProSync = 1;    
	return 0;
}

int __trt__GetOSDs(struct soap* soap, struct _trt__GetOSDs *trt__GetOSDs, struct _trt__GetOSDsResponse *trt__GetOSDsResponse)
{
    static struct tt__OSDConfiguration osdcfg[3];
    static struct tt__OSDReference osdref[3];
    static struct tt__OSDPosConfiguration osdpos[3];
    static struct tt__Vector osdvec[3];
    static struct tt__OSDTextConfiguration osdtextcfg[3];

    int i,j;

    for(i=j=0;i<3;i++)
    {
        if(myosd[i].token[0] != 0)
        {
            j++;
            osdcfg[i].token = myosd[i].token;
            osdcfg[i].VideoSourceConfigurationToken = &osdref[i];
            osdcfg[i].Type = tt__OSDType__Text;
            osdcfg[i].Position = &osdpos[i];
            osdcfg[i].TextString = &osdtextcfg[i];
            osdcfg[i].Image = NULL;
            osdcfg[i].Extension = NULL;
            osdcfg[i].__anyAttribute = NULL;

            osdref[i].__item = myosd[i].VideoSourceConfigurationToken;
            osdref[i].__anyAttribute = NULL;
        
            osdpos[i].Type = myosd[i].PosType;
            osdpos[i].Pos = &osdvec[i];
            osdpos[i].Extension = NULL;
            osdpos[i].__anyAttribute = NULL;
            
            osdvec[i].x = &myosd[i].PosVector_x;
            osdvec[i].y = &myosd[i].PosVector_y;

            osdtextcfg[i].Type = "Plain";
            osdtextcfg[i].DateFormat = NULL;
            osdtextcfg[i].TimeFormat = NULL;
            osdtextcfg[i].FontSize = &myosd[i].fontsize;
            osdtextcfg[i].FontColor = NULL;
            osdtextcfg[i].BackgroundColor = NULL;
            osdtextcfg[i].PlainText = myosd[i].PlainText;
            osdtextcfg[i].__anyAttribute = NULL;            
        }
    }

    trt__GetOSDsResponse->OSDs = &osdcfg;
    trt__GetOSDsResponse->__sizeOSDs = j;
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    return SOAP_OK;
}

int __trt__GetOSD(struct soap* soap, struct _trt__GetOSD *trt__GetOSD, struct _trt__GetOSDResponse *trt__GetOSDResponse)
{
    static struct tt__OSDConfiguration osdcfg;
    static struct tt__OSDReference osdref;
    static struct tt__OSDPosConfiguration osdpos;
    static struct tt__Vector osdvec;
    static struct tt__OSDTextConfiguration osdtextcfg;
    int i;

    for(i=0;i<3;i++)
    {
        if (strcmp(myosd[i].token, trt__GetOSD->OSDToken) ==  0)
        {
            osdcfg.token = myosd[i].token;
            osdcfg.VideoSourceConfigurationToken = &osdref;
            osdcfg.Type = tt__OSDType__Text;
            osdcfg.Position = &osdpos;
            osdcfg.TextString = &osdtextcfg;
            osdcfg.Image = NULL;
            osdcfg.Extension = NULL;
            osdcfg.__anyAttribute = NULL;

            osdref.__item = myosd[i].VideoSourceConfigurationToken;
            osdref.__anyAttribute = NULL;
        
            osdpos.Type = myosd[i].PosType;
            osdpos.Pos = &osdvec;
            osdpos.Extension = NULL;
            osdpos.__anyAttribute = NULL;
            
            osdvec.x = &myosd[i].PosVector_x;
            osdvec.y = &myosd[i].PosVector_y;
            
            osdtextcfg.Type = "Plain";
            osdtextcfg.DateFormat = NULL;
            osdtextcfg.TimeFormat = NULL;
            osdtextcfg.FontSize = &myosd[i].fontsize;
            osdtextcfg.FontColor = NULL;
            osdtextcfg.BackgroundColor = NULL;
            osdtextcfg.PlainText = myosd[i].PlainText;
            osdtextcfg.__anyAttribute = NULL;

            trt__GetOSDResponse->OSD = &osdcfg;
            trt__GetOSDResponse->__any = NULL;
            trt__GetOSDResponse->__size= NULL;

        }       
    }

	printf("%s:%d\n",__FUNCTION__,__LINE__);
    return SOAP_OK;
}

int __trt__GetOSDOptions(struct soap* soap, struct _trt__GetOSDOptions *trt__GetOSDOptions, struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse)
{
    static struct tt__OSDConfigurationOptions osdco;
    static struct tt__MaximumNumberOfOSDs mnoosd;
    static enum tt__OSDType tp;
    static char *poption[]={"LowerLeft","LowerRight","UpperLeft","UpperRight","Custom"};
    static char *tptype[]={"Plain"};
    static struct tt__OSDTextOptions osdtp;
    static int pltext = 3,dateatime = 1;
    static struct tt__IntRange ir;
    
    osdco.MaximumNumberOfOSDs = &mnoosd;
    osdco.__sizeType = 1;
    osdco.Type = &tp;
    osdco.__sizePositionOption =5;
    osdco.PositionOption = poption;
    osdco.TextOption = &osdtp;
    osdco.ImageOption = NULL;
    osdco.Extension = NULL;     
    osdco.__anyAttribute = NULL;

    mnoosd.Total = 4;
    mnoosd.PlainText = &pltext;
    mnoosd.Date = NULL;
    mnoosd.Time = NULL;
    mnoosd.DateAndTime = &dateatime;
    mnoosd.Image = NULL;
    mnoosd.__anyAttribute = NULL;

    tp = tt__OSDType__Text;

    osdtp.__sizeType = 1;
    osdtp.Type = tptype;
    osdtp.FontSizeRange = &ir;
    osdtp.__sizeDateFormat= NULL;
    osdtp.DateFormat = NULL;
    osdtp.__sizeTimeFormat = NULL;
    osdtp.TimeFormat = NULL;
    osdtp.FontColor = NULL;
    osdtp.BackgroundColor = NULL;
    osdtp.Extension = NULL;               
    osdtp.__anyAttribute = NULL;
                                        
    ir.Max = 64;
    ir.Min = 16;    

    trt__GetOSDOptionsResponse->OSDOptions = &osdco;
    trt__GetOSDOptionsResponse->__size = NULL;
    trt__GetOSDOptionsResponse->__any = NULL;
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    return SOAP_OK;
}


int __trt__SetOSD(struct soap* soap, struct _trt__SetOSD *trt__SetOSD, struct _trt__SetOSDResponse *trt__SetOSDResponse)
{   
    int i;

    for(i=0;i<3;i++)
    { 
        if (strcmp(myosd[i].token,trt__SetOSD->OSD->token ) ==  0)
        {
            strcpy(myosd[i].VideoSourceConfigurationToken,trt__SetOSD->OSD->VideoSourceConfigurationToken->__item);
            strcpy(myosd[i].PosType,trt__SetOSD->OSD->Position->Type);
            if (strcmp(trt__SetOSD->OSD->Position->Type,"Custom") ==  0)
            {
               myosd[i].PosVector_x =  *(trt__SetOSD->OSD->Position->Pos->x);
               myosd[i].PosVector_y =  *(trt__SetOSD->OSD->Position->Pos->y);                
            }
            myosd[i].fontsize =  *(trt__SetOSD->OSD->TextString->FontSize);            

            if(trt__SetOSD->OSD->TextString->PlainText[0]!='>' || trt__SetOSD->OSD->TextString->PlainText[1]!='>')
            {
                //UTF8toGBK(trt__SetOSD->OSD->TextString->PlainText, strlen(trt__SetOSD->OSD->TextString->PlainText), myosd[i].PlainText, sizeof(myosd[i].PlainText));
                strcpy(myosd[i].PlainText,trt__SetOSD->OSD->TextString->PlainText);
                myosd[i].Head = 0x5a;
                paramsSave();
                CHECK_calc(&myosd[i]);
            }
            else
            {
                //UTF8toGBK(&trt__SetOSD->OSD->TextString->PlainText[2], strlen(&trt__SetOSD->OSD->TextString->PlainText[2]), myosd[i].PlainText, sizeof(myosd[i].PlainText));
                strcpy(myosd[i].PlainText,&trt__SetOSD->OSD->TextString->PlainText[2]);
                myosd[i].Head = 0xa5;
                CHECK_calc(&myosd[i]);
            }        

            osdsend = 1;
        }
    }
    trt__SetOSDResponse->__any = NULL;
    trt__SetOSDResponse->__size = NULL;
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    return SOAP_OK;
}

int __trt__CreateOSD(struct soap* soap, struct _trt__CreateOSD *trt__CreateOSD, struct _trt__CreateOSDResponse *trt__CreateOSDResponse)
{
	int i,j;

    for(i=j=0;i<3;i++)
    {
        if(myosd[i].token[0] == 0)
        {
            sprintf(myosd[i].token,"video_source1_osd%d\0",i);
            j=i;
            printf("steup 1\n");
            strcpy(myosd[j].VideoSourceConfigurationToken,trt__CreateOSD->OSD->VideoSourceConfigurationToken->__item);
            printf("steup 2\n");
            strcpy(myosd[j].PosType,trt__CreateOSD->OSD->Position->Type);
            printf("steup 3\n");

            if (strcmp(trt__CreateOSD->OSD->Position->Type,"Custom") ==  0)
            {
                myosd[j].PosVector_x =  *(trt__CreateOSD->OSD->Position->Pos->x);
                printf("steup 4\n");
                myosd[j].PosVector_y =  *(trt__CreateOSD->OSD->Position->Pos->y);
                printf("steup 5\n");
            }
            myosd[j].fontsize =  *(trt__CreateOSD->OSD->TextString->FontSize);
            printf("steup 6\n");
            trt__CreateOSDResponse->OSDToken = myosd[j].token;
            printf("steup 7\n");
            trt__CreateOSDResponse->__any = NULL;
            trt__CreateOSDResponse->__size= NULL;
            if(trt__CreateOSD->OSD->TextString->PlainText[0]!='>' || trt__CreateOSD->OSD->TextString->PlainText[1]!='>')
            {

               // UTF8toGBK(trt__CreateOSD->OSD->TextString->PlainText, strlen(trt__CreateOSD->OSD->TextString->PlainText), myosd[i].PlainText, sizeof(myosd[i].PlainText));
                strcpy(myosd[j].PlainText,trt__CreateOSD->OSD->TextString->PlainText);
                myosd[j].Head = 0x5a;
                paramsSave();
                CHECK_calc(&myosd[j]);
            }
            else
            {
                //UTF8toGBK(&trt__CreateOSD->OSD->TextString->PlainText[2], strlen(&trt__CreateOSD->OSD->TextString->PlainText[2]), myosd[i].PlainText, sizeof(myosd[i].PlainText));
                strcpy(myosd[j].PlainText,&trt__CreateOSD->OSD->TextString->PlainText[2]);
                myosd[j].Head = 0xa5;
                CHECK_calc(&myosd[j]);
            }        
            osdsend = 1;           
            break;
        }
    }
	printf("%s:%d\n",__FUNCTION__,__LINE__);
    return SOAP_OK;
}

int __trt__DeleteOSD(struct soap* soap, struct _trt__DeleteOSD *trt__DeleteOSD, struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse)
{
    int i;
    for(i=0;i<3;i++)
    {
        if (strcmp(myosd[i].token,trt__DeleteOSD->OSDToken) ==  0)
        {
            myosd[i].token[0] = 0;
            CHECK_calc(&myosd[i]);
        }
    }
    paramsSave();
    osdsend = 1;
    printf("%s:%d\n",__FUNCTION__,__LINE__);
    return SOAP_OK;
}
