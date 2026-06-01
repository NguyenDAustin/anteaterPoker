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

    //Card hand[2];
    //Card playerCards[2];

    Card playerCards[MAX_PLAYER_CARDS];
    Icon *avatarImg;
    int betSize;
    int currentBet;
    int raisesThisRound;
    PlayerType type;
    bool canAct;
    bool hasFolded;
    bool isReady; 

} Player_Info;

typedef enum
{
    FOLD = 1,
    CHECK = 2,
    CALL = 3,
    BET = 4,
    RAISE = 5
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
Card* getCards(const Player_Info* playerInfo);
bool getReadyStatus(const Player_Info* playerInfo); 

//SETTER FUNCTIONS FOR PLAYER INFO - NO PLAYER NUM GIVEN
void setName(Player_Info *player, const char *name);
void setSeat(Player_Info *player, int seat);
void setChipCount(Player_Info *player, int chipCount);
void setBetSize(Player_Info *player, int betSize);
void setPlayerType(Player_Info *player, PlayerType type);
void setAvatar(Player_Info *player, Icon *avatarImg);
void setCards(Player_Info* player, Card* playerCards);
void setReadyStatus(Player_Info* player, bool isReady);

// start of program
void initPlayer(Player_Info *player, const char *name, int seat, int chips, PlayerType type);

// start of each hand
void resetPlayer(Player_Info *player);
void dealHoleCards(Player_Info *player, Card card1, Card card2);

// start of each betting round
void resetBetSize(Player_Info *player);

void foldPlayer(Player_Info *player);
bool canPlayerAct(const Player_Info *player);

#endif
