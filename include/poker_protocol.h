#ifndef POKER_PROTOCOL_H
#define POKER_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>

#include "state.h"

#define POKER_MESSAGE_SIZE 128
#define GAME_STATE_MESSAGE_SIZE 4096

typedef enum {
    POKER_ACTION_UNKNOWN = 0,
    POKER_ACTION_FOLD = 1,
    POKER_ACTION_CHECK = 2,
    POKER_ACTION_CALL = 3,
    POKER_ACTION_BET = 4,
    POKER_ACTION_RAISE = 5
} PokerActionType;



typedef struct {
    PokerActionType type;
    int amount;
} PokerActionMessage;

const char* pokerActionTypeToString(PokerActionType type);
bool parsePokerActionMessage(const char* message, PokerActionMessage* action);
bool formatPokerActionMessage(char* buffer, size_t bufferSize, PokerActionType type, int amount);
bool parsePlayerNameMessage(const char* message, char* nameBuffer, size_t nameBufferSize);
bool formatPlayerNameMessage(char* buffer, size_t bufferSize, const char* playerName);

// FULL GAME STATE PROTOCOL
// message format (single line, pipe-separated):
//   "Turn = 2 | Pot = 5000 | Board = 7:0,13:1,12:2 | Player 1 Chips = 1000 | Player 1 Folded = 0 | Player 1 Cards = 14:0,13:3 | Player 2 Chips = 2000 | ..."
// cards encoded as "rank:suit"; rank -1 = unknown/hidden.
// player numbers in the message are 1-indexed.

// builds the full game state message from gameState.
// returns true on success, false on buffer overflow or invalid input.
bool formatFullGameState(char* buffer, size_t bufferSize, const GameState* gameState);

// individual setters --> each scans the whole message for its field and
// updates that field in gameState. fields not present in the message
// leave gameState untouched.
void applyTurn(const char* message, GameState* gameState);
void applyPot(const char* message, GameState* gameState);
void applyDealerCards(const char* message, GameState* gameState);
void applyPlayerChips(const char* message, GameState* gameState, int playerNum);
void applyPlayerFolded(const char* message, GameState* gameState, int playerNum);
void applyPlayerCards(const char* message, GameState* gameState, int playerNum);
void applyPlayerName(const char* message, GameState* gameState, int playerNum);

// convenience parser --> calls every setter for every player.
void parseFullGameState(const char* message, GameState* gameState);

#endif
