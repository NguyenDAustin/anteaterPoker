CC = gcc
CFLAGS = -Wall -g -Iinclude

HOST = bondi.eecs.uci.edu
PORT = 10215 

COMMON_SRC = 

SERVER_SRC = src/server.c $(COMMON_SRC)
CLIENT_SRC = src/client.c $(COMMON_SRC)

all: server client

server: $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o server

client: $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o client

run-server: server
	./server $(PORT)

run-client: client
	./client localhost $(HOST) $(PORT)

clean:
	rm -f server client server.exe client.exe