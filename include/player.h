#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "enums.h"
#include <stdbool.h>

typedef enum
{
    HUMAN_PLAYER,
    BOT_PLAYER
} PlayerType;

typedef struct Player_Info
{
    char name[20];
    int seat;
    int chips;
    Card hand[2];
    Card hole_cards[2];
    Card *playerCards;
    Icon *avatarImg;
    int betSize;
    int currentBet;
    PlayerType type;
    bool isActive;
    bool hasFolded;
} Player_Info;

typedef Player_Info Player;

typedef enum
{
    FOLD,
    CHECK,
    CALL,
    BET,
    RAISE
} PlayerActionType;

typedef struct
{
    PlayerActionType actionType;
    int amount;
} PlayerAction;

//GETTER FUNCTIONS FOR PLAYER INFO - NO PLAYER NUM GIVEN 
const char* getName(const Player_Info* playerInfo); 
int getChipCount(const Player_Info* playerInfo); 
Icon* getAvatar(const Player_Info* playerInfo); 
Card* getCards(Player_Info* playerInfo); 

// start of program
void initPlayer(Player_Info *player, const char *name, int seat, int chips, PlayerType type);

// start of each hand
void resetPlayer(Player_Info *player);
void dealHoleCards(Player_Info *player, Card card1, Card card2);

// start of each betting round
void resetBetSize(Player_Info *player);

void takeAction(Player_Info *player, PlayerAction action);
void foldPlayer(Player_Info *player);
bool isPlayerActive(const Player_Info *player);

#endif
