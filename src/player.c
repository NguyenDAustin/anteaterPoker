#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "player.h"

void initPlayer(Player_Info *player, const char *name, int seat, int chips, PlayerType type)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot initialize player\n");
        return;
    }

    if (!name)
    {
        name = "";
    }

    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';

    player->seat = seat;
    player->chips = chips;
    player->betSize = 0;
    player->currentBet = 0;
    player->type = type;
    player->canAct = chips > 0; //can act in the current hand if they have chips
    player->hasFolded = false;
    player->avatarImg = NULL;

    player->playerCards[0] = empty_card();
    player->playerCards[1] = empty_card();
    

    //player->playerCards = NULL; 
    
   
}

void resetPlayer(Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot reset player\n");
        return;
    }

    //player->playerCards = player->hand;

    player->playerCards[0] = empty_card();
    player->playerCards[1] = empty_card();

    //player->playerCards = NULL; //reset this way
    player->betSize = 0;
    player->currentBet = 0;
    player->canAct = player->chips > 0;
    player->hasFolded = false;
}

const char *getName(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return NULL;
    }

    return player->name;
}

int getSeat(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return -1;
    }

    return player->seat;
}

int getChipCount(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return -1;
    }

    return player->chips;
}

int getBetSize(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return -1;
    }

    return player->betSize;
}

PlayerType getPlayerType(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return HUMAN_PLAYER;
    }

    return player->type;
}

Icon *getAvatar(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return NULL;
    }

    return player->avatarImg;
}

Card *getCards(Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return NULL;
    }

    return player->playerCards;
}

const Card *getCardsConst(const Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL\n");
        return NULL;
    }

    return player->playerCards;
}

bool canPlayerAct(const Player_Info *player)
{
    if (!player)
    {
        return false;
    }

    return player->canAct;
}

void setName(Player_Info *player, const char *name)
{
    if (!player || !name)
    {
        printf("ERROR: player or name is NULL cannot set name\n");
        return;
    }

    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';
}

void setSeat(Player_Info *player, int seat)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot set seat\n");
        return;
    }

    player->seat = seat;
}

void setChipCount(Player_Info *player, int chipCount)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot set chip count\n");
        return;
    }

    if (chipCount < 0)
    {
        chipCount = 0;
    }

    player->chips = chipCount;
}

void setBetSize(Player_Info *player, int betSize)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot set bet size\n");
        return;
    }

    if (betSize < 0)
    {
        betSize = 0;
    }

    player->betSize = betSize;
}

void setPlayerType(Player_Info *player, PlayerType type)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot set player type\n");
        return;
    }

    player->type = type;
}

void setAvatar(Player_Info *player, Icon *avatarImg)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot set avatar\n");
        return;
    }

    player->avatarImg = avatarImg;
}

void setCards(Player_Info* player, Card* playerCards){
    if (!player)
    {
        printf("ERROR: player is NULL cannot set cards\n");
        return;
    }

    if (!playerCards) {
        player->playerCards[0] = empty_card();
        player->playerCards[1] = empty_card();
        return;
    }

    player->playerCards[0] = playerCards[0]; 
    player->playerCards[1] = playerCards[1]; 
}

void foldPlayer(Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot fold player\n");
        return;
    }

    player->canAct = false;
    player->hasFolded = true; 
}

void dealHoleCards(Player_Info *player, Card card1, Card card2)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot deal hole cards\n");
        return;
    }

    player->playerCards[0] = card1; 
    player->playerCards[1] = card2;
}

void resetBetSize(Player_Info *player)
{
    if (!player)
    {
        printf("ERROR: player is NULL cannot reset bet size\n");
        return;
    }

    player->betSize = 0;
}