CC = gcc

CFLAGS = -Wall

TARGET = rmdup

DEPS = rmdup.h lstdir.h

all: rmdup lstdir

bin: 
  mkdir bin

rmdup: rmdup.c 
	$(CC) $(CFLAGS) -o bin/rmdup rmdup.c 

lstdir: lstdir.c
	$(CC) $(CFLAGS) -o bin/lstdir lstdir.c
