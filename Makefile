CC = gcc
CFLAGS = -Wall -g -Iinclude

GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

HOST = bondi.eecs.uci.edu
PORT = 10215 

COMMON_SRC = 

SERVER_SRC = src/server.c $(COMMON_SRC)
CLIENT_SRC = src/client.c $(COMMON_SRC)
GUI_SRC = src/gui_client.c src/gui.c $(COMMON_SRC)

all: server client

server: $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o server 

client: $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o client 


gui-client: $(GUI_SRC)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(GUI_SRC) -o gui-client $(GTK_LIBS)

run-server: server
	./server $(PORT)

run-client: client
	./client $(HOST) $(PORT)

run-gui-client: gui-client
	./gui-client $(HOST) $(PORT)

clean:
	rm -f server client server.exe client.exe