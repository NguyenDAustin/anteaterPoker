#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "card.h"
#include "game.h"

void initBot(Player_Info *bot, const char *name, int seat, int chips);
int evalPreFlop(Card hand[2]);
PlayerAction getBotPreFlopAction(Player_Info *bot, GameState *game);
PlayerAction getBotPostFlopAction(Player_Info *bot, GameState *game);
PlayerAction getBotAction(Player_Info *bot, GameState *game);



#endif 
