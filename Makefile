
#
#	Makefile for hello project
#

DEBUG = -g
CC = qcc
LD = qcc


TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntox86
#TARGET = -Vgcc_ntoarmv7le
#TARGET = -Vgcc_ntoaarch64le


CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)
BINS = Control Sensors RideMode
all: $(BINS)

clean:
	rm -f *.o $(BINS);
#	cd solutions; make clean


Control.o: Control.c Control.h
Sensors.o: Sensors.c Control.h
RideMode.o: RideMode.c Control.h

