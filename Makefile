CC=gcc
CFLAGS=-Wall

all: client.o server.o

client.o: client.c 
	$(CC) $(CFLAGS) client.c -o client

server.o: server.c
	$(CC) $(CFLAGS) server.c -o server

clean:
	rm -rf *.o