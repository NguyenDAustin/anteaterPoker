CC = gcc
CFLAGS = -Wall -g -I../include

FONTCONFIG_CFLAGS = $(shell pkg-config --cflags fontconfig)
FONTCONFIG_LIBS = $(shell pkg-config --libs fontconfig)
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

BIN_DIR = ../bin

COMMON_SRC = deck.c hand.c state.c game.c communication.c poker_protocol.c \
	player.c card.c state_protocol.c lobby.c monte_carlo.c bot.c

SERVER_SRC = server.c gui_server.c timer.c $(COMMON_SRC)
GUI_SRC = render.c gui_client.c gui.c playerbox_render.c timer.c gui_server.c $(COMMON_SRC)

.PHONY: all server client clean

all: server client

server: $(BIN_DIR)/poker_server

client: $(BIN_DIR)/poker_client

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/poker_server: $(SERVER_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(SERVER_SRC) -o $@ $(GTK_LIBS) 

$(BIN_DIR)/poker_client: $(GUI_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(FONTCONFIG_CFLAGS) $(GUI_SRC) -o $@ $(GTK_LIBS) $(FONTCONFIG_LIBS) -lm

clean:
	rm -f $(BIN_DIR)/poker_server $(BIN_DIR)/poker_client
