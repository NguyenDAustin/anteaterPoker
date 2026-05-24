#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "card.h"

typedef enum {
    EASY,
    MEDIUM,
    HARD
} BotDifficulty;

typedef struct {
    Player player;
    BotDifficulty difficulty;
} Bot;

void initBot(Bot *bot, const char *name, int seat, int chips);
int evalPreFlop(Card hand[2]);
PlayerAction getBotAction(Player *bot);



#endif 