#include "gsoapapp.h"
#include "MediaBinding.nsmap"

int gsoapAppInit(struct GsoapApp* app)
{

    if (!app->bindPort)
        app->bindPort = 80;

	strcpy(app->hostAddress,  get_local_ip("eth0"));//eth0
    snprintf(app->xAddr, sizeof(app->xAddr), "http://%s:%d/onvif/device_service",
             app->hostAddress, app->bindPort);
    snprintf(app->xAddrRoot, sizeof(app->xAddrRoot), "http://%s:%d/onvif",
             app->hostAddress, app->bindPort);
	
    soap_init2(&app->soap, SOAP_ENC_MTOM | SOAP_ENC_MIME, 0 );
    soap_set_mode(&app->soap,SOAP_C_UTFSTRING );
	soap_set_namespaces(&app->soap, namespaces1);
//    soap_set_recv_logfile(&app->soap, "./log/recv.xml");
 //   soap_set_sent_logfile(&app->soap, "./log/send.xml");
 //   soap_set_test_logfile(&app->soap, "./log/test.log");

    app->soap.bind_flags        = SO_REUSEADDR;
    app->soap.accept_timeout    = 10;
    app->soap.recv_timeout      = 10;
    app->soap.send_timeout      = 10;
    app->soap.max_keep_alive    = 30;

    app->masterSocket = soap_bind(&app->soap, "0.0.0.0", app->bindPort, 100);
    if (app->masterSocket < 0) {
        soap_print_fault(&app->soap, stderr);
        return -1;
    }

    return 0;
}


void gsoapAppFini(struct GsoapApp* app)
{
    if (app->forked) {
        pthread_join(app->threadId, NULL);
        app->threadId = 0;
    }
}


static void* thread_work(void* arg)
{
    int err = 0;
    struct GsoapApp* app = (struct GsoapApp*)arg;
    while (app->running)
    {
        app->soap.socket_flags = MSG_NOSIGNAL;
        app->slaveSocket = soap_accept(&app->soap);
        if (!soap_valid_socket(app->slaveSocket))
        {
            if (app->soap.errnum) {
                soap_print_fault(&app->soap, stderr);
            }
            continue; 
        }
        fprintf(stderr, "accept web service connection %d from IP %d.%d.%d.%d\n",
               app->slaveSocket,
               (app->soap.ip >> 24) & 0xFF,
               (app->soap.ip >> 16) & 0xFF,
               (app->soap.ip >> 8) & 0xFF,
               app->soap.ip & 0xFF);

        err = soap_serve(&app->soap);
        if (err != SOAP_OK)
            fprintf(stderr, "soap_serve() error: \"%s\"\n",
                   *soap_faultstring(&app->soap));

        fprintf(stderr, "soap->action: %s\n", app->soap.action);

        soap_destroy(&app->soap);
        soap_end(&app->soap);
    }
    soap_done(&app->soap);
    return (void*)0;
}

int gsoapAppRun(struct GsoapApp* app, int forked)
{
    app->running = 1;
    if (!forked)
        return (int)thread_work(app);
    if (pthread_create(&app->threadId, NULL, thread_work, (void*)app) < 0) {
		fprintf(stderr, "pthread_create failed\n");
        return -1;
    }
    app->forked = 1;
    return 0;
}

