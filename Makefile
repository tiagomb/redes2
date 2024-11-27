CFLAGS = -g -O3
CC = gcc

objects = TCP.o UDP.o utils.o

all: TCP UDP Graphic

TCP: TCP.o utils.o
	$(CC) -o TCP TCP.o utils.o

UDP: UDP.o utils.o
	$(CC) -o UDP UDP.o utils.o

TCP.o: TCP.c
	$(CC) -c TCP.c  $(CFLAGS)

UDP.o: UDP.c
	$(CC) -c UDP.c  $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -c utils.c  $(CFLAGS)

clean: all
	rm -f $(objects)

purge: clean
	rm -f TCP UDP
