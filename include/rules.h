#ifndef RULES_H 
#define RULES_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"
#include "player.h"
#include "hand.h"

bool isValidCheck(GameState *game, Player *player);
bool isValidCall(GameState *game, Player *player);
bool isValidRaise(GameState *game, Player *player, int amount);
bool isValidFold(GameState *game, Player *player);
bool isValidAnteaterAction(GameState *game, Player *player, Hand *hand);

void applyCheck(GameState *game, Player *player);
void applyCall(GameState *game, Player *player);
void applyRaise(GameState *game, Player *player, int amount);
void applyFold(GameState *game, Player *player);
void applyAnteaterAction(GameState *game, Player *player, Hand *hand);


#endif 
