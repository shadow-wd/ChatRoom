CC = gcc
CFLAGS = -Wall -I ./include
LDFLAGS = -lpthread
BIN_DIR = ./bin

all: server client
server: ./server/server.c
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

client: ./client/client.c
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(BIN_DIR)/server
	rm -f $(BIN_DIR)/client
