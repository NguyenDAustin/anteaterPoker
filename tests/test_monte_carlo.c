#include <stdio.h>
#include "monte_carlo.h"
#include "state.h"
#include "player.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    GameState game;
    initGameState(&game);

    initPlayer(game.players[0], "Hero",    0, 1000, BOT_PLAYER);
    initPlayer(game.players[1], "Villain", 1, 1000, HUMAN_PLAYER);
    game.numPlayers = 2;

    /* give the hero two known cards */
    game.players[0]->playerCards[0] = cardCtor(HEARTS, ACE);
    game.players[0]->playerCards[1] = cardCtor(SPADES, ACE);

    double equity = estimateBotEquity(&game, 0, 100);

    CHECK(equity >= 0.0 && equity <= 1.0, "equity must be in [0,1]");
    /* pocket aces preflop heads-up should be very strong */
    CHECK(equity > 0.5, "pocket aces preflop should have equity > 0.5");

    printf("monte_carlo test passed (equity=%.3f)\n", equity);
    return 0;
}
