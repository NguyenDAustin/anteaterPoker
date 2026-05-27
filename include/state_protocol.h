#ifndef STATE_PROTOCOL_H
#define STATE_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>

#include "state.h"

#define GAME_STATE_MESSAGE_SIZE 1024

// serializes the GameState into a text message for transmission over the socket.
// returns true on success, false if the buffer is too small or arguments are invalid.
bool formatGameStateMessage(char* buffer, size_t bufferSize, const GameState* game);

// parses a game state message and writes the values into the GameState.
// returns true on success, false on parse error or invalid input.
// unknown/hidden cards use rank -1 (matches state.c's empty_card()).
bool parseGameStateMessage(const char* message, GameState* game);

#endif
