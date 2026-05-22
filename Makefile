CC = gcc
CFLAGS = -Wall -g -Iinclude

FONTCONFIG_CFLAGS = $(shell pkg-config --cflags fontconfig)
FONTCONFIG_LIBS = $(shell pkg-config --libs fontconfig)
GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

HOST = bondi.eecs.uci.edu
PORT = 10215 

COMMON_SRC = src/deck.c src/hand.c

SERVER_SRC = src/server.c $(COMMON_SRC)
CLIENT_SRC = src/client.c $(COMMON_SRC)
GUI_SRC =  src/render.c src/gui_client.c src/gui.c src/playerbox_render.c $(COMMON_SRC)

all: server client

server: $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o server 

client: $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o client 


gui-client: $(GUI_SRC)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(FONTCONFIG_CFLAGS) $(GUI_SRC) -o gui-client $(GTK_LIBS) $(FONTCONFIG_LIBS) -lm

run-server: server
	./server $(PORT)

run-client: client
	./client $(HOST) $(PORT)

run-gui-client: gui-client
	./gui-client $(HOST) $(PORT)

clean:
	rm -f server client server.exe client.exe