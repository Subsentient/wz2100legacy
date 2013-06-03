CC=gcc
CFLAGS=-c -std=c89 -Wall

all: restree

config: config.c config.h
	$(CC) $(CFLAGS) config.c

restree: config restree.c
	$(CC) $(CFLAGS) restree.c -o restree
