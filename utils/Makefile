CC=gcc
CFLAGS=-Wall -g3 -O0 -std=c89 -pedantic

all: config restree
clean:
	rm *.o

config: config.c config.h config_test.c
	$(CC) $(CFLAGS) config.c config_test.c -o config_test.o

restree: config restree.c
	$(CC) $(CFLAGS) restree.c -o restree.o
