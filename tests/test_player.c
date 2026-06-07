#include <stdio.h>
#include <string.h>
#include "player.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    Player_Info p;
    initPlayer(&p, "Naveen", 0, 1000, HUMAN_PLAYER);

    CHECK(strcmp(p.name, "Naveen") == 0, "name not set");
    CHECK(p.chips == 1000, "chips not set");
    CHECK(p.seat == 0, "seat not set");
    CHECK(p.playerType == HUMAN_PLAYER, "type not set");
    CHECK(p.hasFolded == false, "hasFolded should default to false");
    CHECK(p.canAct == true, "canAct should be true for a player with chips");

    setChipCount(&p, 500);
    CHECK(getChipCount(&p) == 500, "setChipCount/getChipCount round-trip failed");

    setName(&p, "Renamed");
    CHECK(strcmp(getName(&p), "Renamed") == 0, "setName/getName round-trip failed");

    foldPlayer(&p);
    CHECK(p.hasFolded == true, "foldPlayer should set hasFolded");
    CHECK(canPlayerAct(&p) == false, "folded player should not be able to act");

    resetPlayer(&p);
    CHECK(p.hasFolded == false, "resetPlayer should clear hasFolded");

    /* NULL guards should not crash */
    initPlayer(NULL, "x", 0, 100, HUMAN_PLAYER);
    resetPlayer(NULL);

    printf("player test passed\n");
    return 0;
}
