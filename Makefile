
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
BINS = sensors server
all: $(BINS)

clean:
	rm -f *.o $(BINS);
#	cd solutions; make clean


server.o: server.c suspension.h
sensors.o: sensors.c suspension.h

