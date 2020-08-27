#include <stdio.h>
#include "wsdd.nsmap"
#include "wsddapi.h"
#include "discovery.h"

int discoveryInit(struct DiscoveryApp *disApp)
{
    struct ip_mreq mcast;
   
    soap_init2(&disApp->soap, SOAP_IO_UDP | SOAP_IO_FLUSH, SOAP_IO_UDP|SOAP_IO_FLUSH);   
    soap_set_namespaces(&disApp->soap, namespaces); 
    soap_set_mode(&disApp->soap, SOAP_C_UTFSTRING);
    disApp->soap.bind_flags        = SO_REUSEADDR;
    disApp->soap.accept_timeout    = 10;
    disApp->soap.recv_timeout      = 10;
    disApp->soap.send_timeout      = 10;
    disApp->soap.max_keep_alive    = 30;
    
    soap_register_plugin(&disApp->soap, soap_wsa);
	
	// 打开调试信息
    soap_set_recv_logfile(&disApp->soap, "./log/recv.xml");
    soap_set_sent_logfile(&disApp->soap, "./log/send.xml");
    soap_set_test_logfile(&disApp->soap, "./log/test.log");
    if(!soap_valid_socket(soap_bind(&disApp->soap, NULL, 3702, 16)))
     {
		soap_print_fault(&disApp->soap, stderr);
		exit(1);
     }

    mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mcast.imr_interface.s_addr = inet_addr("0.0.0.0");
    if(setsockopt(disApp->soap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0) {
        printf("setsockopt error!\n");
        return -1;
    }  
	return 0;

}

void discoveryFini(struct DiscoveryApp *disApp)
{
	soap_done(&disApp->soap);
}

void *thread_work(void* param)
{
	int s;

	struct DiscoveryApp* app = (struct DiscoveryApp*)param;
	while(app->running) {
        s = soap_wsdd_listen(&app->soap, 0);
        soap_destroy(&app->soap);
        soap_end(&app->soap);
        fprintf(stderr, "Socket connection successful: slave socket = %d\n", s); 
	}
	return (void*)0;
}


int discoveryRun(struct DiscoveryApp *disApp)
{
    disApp->running = 1;
    if (pthread_create(&disApp->threadId, NULL, thread_work, (void*)disApp) < 0) {
		fprintf(stderr, "pthread_create failed\n");
        return -1;
    }

    return 0;
}

