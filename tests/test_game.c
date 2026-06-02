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

    GameState bustedGame;
    initGameState(&bustedGame);

    initPlayer(bustedGame.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(bustedGame.players[1], "Bob",   1,    0, HUMAN_PLAYER);
    initPlayer(bustedGame.players[2], "Carol", 2, 1000, HUMAN_PLAYER);
    bustedGame.numPlayers = 3;
    bustedGame.dealerIndex = 0;

    startNewRound(&bustedGame);
    CHECK(bustedGame.players[1]->hasFolded, "busted player should be out of the next hand");
    CHECK(!bustedGame.players[1]->canAct, "busted player should not be able to act");
    CHECK(bustedGame.players[1]->playerCards[0].rank < 0,
          "busted player should not receive hole cards");
    CHECK(bustedGame.dealerIndex == 2, "dealer rotation should skip busted players");

    printf("game test passed\n");
    return 0;
}
