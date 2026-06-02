#include <stdio.h>
#include "state.h"
#include "player.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    GameState game;
    initGameState(&game);

    CHECK(game.deck != NULL, "initGameState should allocate deck");
    CHECK(game.players != NULL, "initGameState should allocate players");
    CHECK(game.pot == 0, "pot should start at 0");
    CHECK(game.round == ROUND_PRE_FLOP, "round should start at PRE_FLOP");
    CHECK(game.board.count == 0, "board should start empty");
    CHECK(game.smallBlind == 1, "default smallBlind should be 1");
    CHECK(game.bigBlind == 2, "default bigBlind should be 2");

    /* properly init a couple of players */
    initPlayer(game.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(game.players[1], "Bob", 1, 1000, HUMAN_PLAYER);
    game.numPlayers = 2;

    /* setter/getter round-trips */
    setPot(&game, 250);
    CHECK(getPot(&game) == 250, "setPot/getPot round-trip failed");

    setTurnNumber(&game, 5);
    CHECK(getTurnNumber(&game) == 5, "setTurnNumber/getTurnNumber failed");

    setRound(&game, ROUND_FLOP);
    CHECK(getRound(&game) == ROUND_FLOP, "setRound/getRound failed");

    CHECK(setCurrentPlayerIndex(&game, 1), "setCurrentPlayerIndex(1) should succeed");
    CHECK(getCurrentPlayerIndex(&game) == 1, "current player index mismatch");

    setPlayerChipCount(&game, 0, 750);
    CHECK(getPlayerChipCount(&game, 0) == 750, "player chip getter/setter failed");

    /* nextActivePlayerIndex with both active */
    int next = nextActivePlayerIndex(&game);
    CHECK(next >= 0 && next < 2, "nextActivePlayerIndex out of range");

    resetGameState(&game);
    CHECK(game.pot == 0, "resetGameState should clear pot");
    CHECK(game.round == ROUND_PRE_FLOP, "resetGameState should reset round");

    printf("state test passed\n");
    return 0;
}
