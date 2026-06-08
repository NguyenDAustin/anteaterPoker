CC = gcc
CFLAGS = -Wall -g -Iinclude

HOST = bondi.eecs.uci.edu
PORT = 10215 
BIN_DIR = bin
ARCHIVE = Poker_V1.0_src.tar.gz
CUSTOMER_ARCHIVE = Poker_V1.0.tar.gz

TEST_COMM_SRC = tests/test_comm.c src/communication.c

# Pure-logic sources used by most unit tests (no GTK, no main()).
LOGIC_SRC = src/card.c src/deck.c src/hand.c src/player.c src/state.c \
	src/rules.c src/game.c src/monte_carlo.c src/bot.c \
	src/poker_protocol.c src/state_protocol.c

UNIT_TESTS = test_card test_deck test_hand test_player test_state test_rules \
	test_game test_monte_carlo test_bot test_poker_protocol test_state_protocol

.PHONY: all server client run-server run-client test_server test_client \
	test test_gui test-gui test-comm test-unit tar customer-tar clean \
	$(UNIT_TESTS)

all server client:
	$(MAKE) -C src $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run-server: server
	./$(BIN_DIR)/poker_server $(PORT)

run-client: client
	./$(BIN_DIR)/poker_client $(HOST) $(PORT)

test_server: server
	test -x ./$(BIN_DIR)/poker_server

test_client: client
	test -x ./$(BIN_DIR)/poker_client

test-comm: $(BIN_DIR)/test_comm
	./$(BIN_DIR)/test_comm

test: test-comm test-unit

test-gui: test_client

test_gui: test-gui

# Build and run all unit tests sequentially. Stops at first failure.
test-unit: $(UNIT_TESTS)

$(UNIT_TESTS): %: $(BIN_DIR)/%
	./$(BIN_DIR)/$@

$(BIN_DIR)/test_comm: $(TEST_COMM_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_COMM_SRC) -o $@

# Generic build rule for unit tests. Each links against all pure-logic sources;
# the linker drops unused symbols.
$(BIN_DIR)/test_%: tests/test_%.c $(LOGIC_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LOGIC_SRC) -o $@ -lm

tar: all
	tar -czf $(ARCHIVE) README COPYRIGHT INSTALL Makefile $(BIN_DIR)/poker_client $(BIN_DIR)/poker_server doc include resources src tests

customer-tar: all
	tar -czf $(CUSTOMER_ARCHIVE) README COPYRIGHT INSTALL $(BIN_DIR)/poker_client $(BIN_DIR)/poker_server doc/Poker_UserManual.pdf resources

clean:
	$(MAKE) -C src clean
	rm -f $(BIN_DIR)/poker_text_client $(BIN_DIR)/poker_gui_client \
		$(BIN_DIR)/test_comm $(addprefix $(BIN_DIR)/,$(UNIT_TESTS)) \
		$(ARCHIVE) $(CUSTOMER_ARCHIVE)
