CFLAGS = -g -O3

all: TCP UDP

TCP: TCP.o utils.o
	gcc -o TCP TCP.o utils.o

UDP: UDP.o utils.o
	gcc -o UDP UDP.o utils.o

TCP.o: TCP.c
	gcc -c TCP.c  $(CFLAGS)

UDP.o: UDP.c
	gcc -c UDP.c  $(CFLAGS)

utils.o: utils.c utils.h
	gcc -c utils.c  $(CFLAGS)

removeObjects: all
	rm *.o