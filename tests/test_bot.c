#include <stdio.h>
#include <string.h>
#include "bot.h"
#include "state.h"
#include "player.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    Player_Info bot;
    initBot(&bot, "BotBob", 1, 1000);
    CHECK(strcmp(bot.name, "BotBob") == 0, "initBot should set bot name");
    CHECK(bot.type == BOT_PLAYER, "initBot should set type to BOT_PLAYER");
    CHECK(bot.chips == 1000, "initBot should set chip count");

    /* preflop eval: pocket aces should score higher than 7-2 offsuit */
    Card aces[2]   = { cardCtor(HEARTS, ACE), cardCtor(SPADES, ACE) };
    Card trash[2]  = { cardCtor(HEARTS, SEVEN), cardCtor(SPADES, TWO) };

    int acesScore  = evalPreFlop(aces);
    int trashScore = evalPreFlop(trash);
    CHECK(acesScore > trashScore, "pocket aces should beat 7-2 offsuit preflop");

    /* getBotAction should produce a sane action for a bot in a real game */
    GameState game;
    initGameState(&game);
    initPlayer(game.players[0], "Hero", 0, 1000, BOT_PLAYER);
    initPlayer(game.players[1], "Vil",  1, 1000, HUMAN_PLAYER);
    game.numPlayers = 2;
    game.players[0]->playerCards[0] = cardCtor(HEARTS, ACE);
    game.players[0]->playerCards[1] = cardCtor(SPADES, ACE);

    PlayerAction action = getBotAction(&game, 0);
    CHECK(action.actionType >= FOLD && action.actionType <= RAISE,
          "bot action type out of range");

    printf("bot test passed (action=%d amount=%d)\n", action.actionType, action.amount);
    return 0;
}
