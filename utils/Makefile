CC=gcc
CFLAGS=-c -std=c89 -Wall

all: restree
clean: 
	rm *.o

config: config.c config.h
	$(CC) $(CFLAGS) config.c

restree: config restree.c
	$(CC) $(CFLAGS) restree.c -o restree
