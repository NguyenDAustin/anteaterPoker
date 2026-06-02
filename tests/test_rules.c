#include <stdio.h>
#include "rules.h"
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
    game.numPlayers = 2;
    game.currentBet = 0;

    Player *alice = game.players[0];
    Player *bob   = game.players[1];

    /* with currentBet == 0, check should be valid */
    CHECK(isValidCheck(&game, alice), "check should be valid when no bet outstanding");

    /* raise of 50 should be valid */
    CHECK(isValidRaise(&game, alice, 50), "raise of 50 with 1000 chips should be valid");

    /* apply the raise and confirm pot/chips moved */
    int prePot = game.pot;
    int preChips = alice->chips;
    applyRaise(&game, alice, 50);
    CHECK(game.pot > prePot, "applyRaise should increase pot");
    CHECK(alice->chips < preChips, "applyRaise should decrease player chips");
    CHECK(game.currentBet >= 50, "applyRaise should bump currentBet");

    /* bob calls */
    CHECK(isValidCall(&game, bob), "call should be valid against a raise");
    int prePot2 = game.pot;
    applyCall(&game, bob);
    CHECK(game.pot > prePot2, "applyCall should increase pot");

    /* fold path */
    CHECK(isValidFold(&game, bob), "fold should always be valid");
    applyFold(&game, bob);
    CHECK(bob->hasFolded, "applyFold should set hasFolded");

    printf("rules test passed\n");
    return 0;
}
