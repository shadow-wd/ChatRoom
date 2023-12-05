# Define compiler and compilation options
CC = gcc
CFLAGS = -Wall -I ./include
LDFLAGS = -lpthread

# Define the directory for output binary files
BIN_DIR = ./bin

# Declare phony targets to avoid conflicts with file names
.PHONY: all clean

# Default target to compile all
all: server client

# Compile the server program
server: server/opera_server.c server/opera_server.h server/server.c
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

# Compile the client program
client: client/client.c client/opera_client.c client/opera_client.h
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

# Clean up generated binary files
clean:
	rm -f $(BIN_DIR)/server
	rm -f $(BIN_DIR)/client
