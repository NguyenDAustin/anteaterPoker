CC = gcc
CFLAGS = -Wall -g -Iinclude

HOST = bondi.eecs.uci.edu
PORT = 10215 
BIN_DIR = bin
ARCHIVE = Poker_Beta_src.tar.gz
CUSTOMER_ARCHIVE = Poker_Beta.tar.gz

TEST_COMM_SRC = tests/test_comm.c src/communication.c

.PHONY: all server client run-server run-client test_server test_client \
	test_gui test_comm test test-gui test-comm tar customer_tar clean

all server client:
	$(MAKE) -C src $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run-server: server
	./$(BIN_DIR)/poker_server $(PORT)

run-client: client
	./$(BIN_DIR)/poker_client $(HOST) $(PORT)

test_server: run-server

test_client: run-client

test_gui: test_client

test_comm: $(BIN_DIR)/test_comm
	./$(BIN_DIR)/test_comm

test: test_comm

test-gui: test_gui

test-comm: test_comm

$(BIN_DIR)/test_comm: $(TEST_COMM_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_COMM_SRC) -o $@

tar: all
	tar -czf $(ARCHIVE) README COPYRIGHT INSTALL Makefile $(BIN_DIR)/.gitkeep $(BIN_DIR)/poker_client $(BIN_DIR)/poker_server doc include resources src tests

customer_tar: all
	tar -czf $(CUSTOMER_ARCHIVE) README COPYRIGHT INSTALL $(BIN_DIR)/poker_client $(BIN_DIR)/poker_server doc/Poker_UserManual.pdf resources

clean:
	$(MAKE) -C src clean
	rm -f $(BIN_DIR)/poker_text_client $(BIN_DIR)/poker_gui_client $(BIN_DIR)/test_comm $(ARCHIVE) $(CUSTOMER_ARCHIVE)
