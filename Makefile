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

# Specify the directories where Makefile should look for files
VPATH = database:server:client

# Define source files for server (without paths)
SERVER_SRCS =    opera_server.c \
				 opera_server.h \
				 server.c \
				 opera_db.h \
				 opera_db.c

# Define source files for client (without paths)
CLIENT_SRCS = 	client.c \
				opera_client.c \
				opera_client.h



# Compile the server program
server: $(SERVER_SRCS)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)  -lsqlite3

# Compile the client program
client: $(CLIENT_SRCS)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LDFLAGS)

# Clean up generated binary files
clean:
	rm -f $(BIN_DIR)/server
	rm -f $(BIN_DIR)/client
