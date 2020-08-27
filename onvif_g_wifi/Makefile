CC =  arm-himix100-linux-gcc
INCLUDE = -I$(GSOAP_ROOT)
#CFLAGS = -DDEBUG
CFLAGS = -Os
LDFLAGS = -lpthread -lm


SERVER_OBJS = dom.o soapC.o soapServer.o soapClient.o stdsoap2.o duration.o wdd_ssom.o  trt_ssom.o tds_ssom.o\
              utils.o discovery.o gsoapapp.o wsaapi.o threads.o wsddapi.o tdn_ssom.o onvif.o

all: onvif
onvif: $(SERVER_OBJS) 
	$(CC) -o $@ $(SERVER_OBJS) $(LDFLAGS) 

%.o : %.c
	$(CC) -c  $^ $(CFLAGS)


clean:
	rm -f *.o onvif
