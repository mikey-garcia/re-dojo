CC=gcc

CFLAGS=-m32 -Wall -O0 -fno-stack-protector -fno-pie -no-pie

all: server client

server: server.c
	$(CC) $(CFLAGS) -o server server.c
	strip server

client: client.c
	$(CC) $(CFLAGS) -o client client.c
	strip client

clean:
	rm -f server client