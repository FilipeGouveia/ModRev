CC      = gcc
CFLAGS  = -Wall
RM      = rm -f
OS 		= $(shell uname -s)

default: all

all: functions

functions: powersetGen.c
	$(CC) $(CFLAGS) -o powersetGen powersetGen.c

clean:
	$(RM) *.o powersetGen powersetGen.exe
