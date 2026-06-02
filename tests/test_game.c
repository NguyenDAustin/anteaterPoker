#include <stdio.h>
#include "game.h"
#include "state.h"
#include "player.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    GameState game;
    initGameState(&game);

    initPlayer(game.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(game.players[1], "Bob",   1, 1000, HUMAN_PLAYER);
    initPlayer(game.players[2], "Carol", 2, 1000, HUMAN_PLAYER);
    game.numPlayers = 3;
    game.dealerIndex = 0;

    startNewRound(&game);
    CHECK(game.round == ROUND_PRE_FLOP, "startNewRound should set round to PRE_FLOP");
    /* pot should be non-zero after blinds */
    CHECK(game.pot > 0, "blinds should have been posted into the pot");

    /* current player should have hole cards dealt */
    CHECK(game.players[0]->playerCards[0].rank >= 0, "hole cards should be dealt");

    /* phase complete check shouldn't crash */
    (void)isBettingPhaseComplete(&game);
    (void)shouldEndRound(&game);

    /* advanceGameRound should move forward */
    Round before = game.round;
    advanceGameRound(&game);
    CHECK(game.round != before || game.round == ROUND_SHOWDOWN,
          "advanceGameRound should change round or land on SHOWDOWN");

    /* try a fold action via handlePlayerAction */
    PlayerAction fold = { .actionType = FOLD, .amount = 0 };
    GameActionResult res = handlePlayerAction(&game, game.currentPlayerIndex, fold);
    CHECK(res == GAME_ACTION_SUCCESS || res == GAME_ACTION_ROUND_COMPLETE,
          "fold should succeed or end the round");

    printf("game test passed\n");
    return 0;
}
