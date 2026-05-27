#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "player.h"
#include "state.h"

typedef enum {
    GAME_ACTION_SUCCESS,
    GAME_ACTION_INVALID_GAME,
    GAME_ACTION_INVALID_PLAYER,
    GAME_ACTION_NOT_PLAYERS_TURN,
    GAME_ACTION_INVALID_AMOUNT,
    GAME_ACTION_NOT_ENOUGH_CHIPS,
    GAME_ACTION_CANNOT_CHECK,
    GAME_ACTION_ROUND_COMPLETE
} GameActionResult;

void startNewRound(GameState *game);
void dealCommunityCards(GameState *game);
GameActionResult handlePlayerAction(GameState *game, int playerIndex, PlayerAction action);
bool isBettingPhaseComplete(const GameState *game);
bool shouldEndRound(const GameState *game);
void advanceGameRound(GameState *game);
int determineWinner(const GameState *game);
void awardPotToWinner(GameState *game, int winnerIndex);

#endif
