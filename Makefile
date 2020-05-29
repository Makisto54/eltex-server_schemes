.PHONY: clean

all: client server1 server2

client: client.c
	gcc client.c -o client -lpthread

server1: server1.c
	gcc server1.c -o server1 -lpthread

server2: server2.c
	gcc server2.c -o server2 -lpthread

clean:
	rm -rf client server1 server2