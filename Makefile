CC = gcc
CFLAGS = -Wall -g -Iinclude

FONTCONFIG_CFLAGS = $(shell pkg-config --cflags fontconfig)
FONTCONFIG_LIBS = $(shell pkg-config --libs fontconfig)
GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

HOST = bondi.eecs.uci.edu
PORT = 10215 
BIN_DIR = bin
ARCHIVE = Poker_Beta_src.tar.gz
CUSTOMER_ARCHIVE = Poker_Beta.tar.gz

COMMON_SRC = src/deck.c src/hand.c src/state.c src/game.c src/communication.c src/poker_protocol.c src/player.c src/card.c src/state_protocol.c src/lobby.c src/monte_carlo.c src/bot.c 

SERVER_SRC = src/server.c $(COMMON_SRC)
GUI_SRC =  src/render.c src/gui_client.c src/gui.c src/playerbox_render.c src/timer.c $(COMMON_SRC)
TEST_COMM_SRC = tests/test_comm.c src/communication.c

.PHONY: all server client run-server run-client test_server test_client \
	test_comm tar customer_tar clean

all: server client

server: $(BIN_DIR)/poker_server

client: $(BIN_DIR)/poker_client

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/poker_server: $(SERVER_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $@

$(BIN_DIR)/poker_client: $(GUI_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(FONTCONFIG_CFLAGS) $(GUI_SRC) -o $@ $(GTK_LIBS) $(FONTCONFIG_LIBS) -lm

run-server: server
	./$(BIN_DIR)/poker_server $(PORT)

run-client: client
	./$(BIN_DIR)/poker_client $(HOST) $(PORT)

test_server: run-server

test_client: run-client

test_comm: $(BIN_DIR)/test_comm
	./$(BIN_DIR)/test_comm

$(BIN_DIR)/test_comm: $(TEST_COMM_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_COMM_SRC) -o $@

tar: all
	tar -czf $(ARCHIVE) --transform='s|^Poker_UserManual.pdf$$|doc/Poker_UserManual.pdf|' README COPYRIGHT INSTALL Makefile $(BIN_DIR)/.gitkeep $(BIN_DIR)/poker_client $(BIN_DIR)/poker_server doc include resources src tests Poker_UserManual.pdf

customer_tar: all
	tar -czf $(CUSTOMER_ARCHIVE) --transform='s|^Poker_UserManual.pdf$$|doc/Poker_UserManual.pdf|' README COPYRIGHT INSTALL $(BIN_DIR)/poker_client $(BIN_DIR)/poker_server doc resources Poker_UserManual.pdf

clean:
	rm -f $(BIN_DIR)/poker_server $(BIN_DIR)/poker_client $(BIN_DIR)/poker_text_client $(BIN_DIR)/poker_gui_client $(BIN_DIR)/test_comm $(ARCHIVE) $(CUSTOMER_ARCHIVE)
