# Hisilicon Hi3516 sample Makefile

include ../Makefile.param
#ifeq ($(SAMPLE_PARAM_FILE), )
#     SAMPLE_PARAM_FILE:=../Makefile.param
#     include $(SAMPLE_PARAM_FILE)
#endif
curdir = $(shell pwd)

SENSOR_LIBS += $(curdir)/rtsp_lib/librtsp.a
SENSOR_LIBS += $(curdir)/../../lib/libsecurec.a 
CFLAGS += -I$(curdir)/rtsp_lib/
CFLAGS += -I$(curdir)/include/

OSD_DIR:=$(curdir)/osd
OSD_SRC := $(wildcard $(OSD_DIR)/*.c)
OSD_OBJ := $(OSD_SRC:%.c=%.o)

TARGET_PATH := $(PWD)

# compile linux or HuaweiLite
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak

# target source
SRC  := $(wildcard *.c) 
OBJ  := $(SRC:%.c=%.o)

TARGET := $(OBJ:%.o=%)
.PHONY : clean all

	
	
all: $(TARGET)
	@cd rtsp_lib;   make
$(TARGET):%:%.o $(COMM_OBJ) $(OSD_OBJ)
	$(CC) $(CFLAGS) -lpthread -lm -o $@ $^ $(MPI_LIBS) $(AUDIO_LIBA) $(SENSOR_LIBS)  $(INCFLAGS)

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)
	@rm -f $(COMM_OBJ)
	@rm -f $(OSD_OBJ)

cleanstream:
	@rm -f *.h264
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4
