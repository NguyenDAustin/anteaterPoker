#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "card.h"
#include "game.h"


void initBot(Player_Info *bot, const char *name, int seat, int chips);
int evalPreFlop(Card hand[2]);
int evalPostFlop(GameState *game, int playerIndex);
int calculateDrawBonus(GameState *game, int playerIndex);
int calculateCallCost(GameState *game, int playerIndex);
double calculateCallPrice(GameState *game, int playerIndex);
int calculateBotRaiseAmount(GameState *game, int playerIndex);
PlayerAction getBotPreFlopAction(GameState *game, int playerIndex);
PlayerAction getBotPostFlopAction(GameState *game, int playerIndex);
PlayerAction getBotAction(GameState *game, int playerIndex);

#endif
