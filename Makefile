CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread

all: server client
server: server.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f server
	rm -f client
