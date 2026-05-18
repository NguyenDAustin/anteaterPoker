#include <stdlib.h>

#include "player.h"

void initPlayer(Player *player, const char *name, int seat, int chips, PlayerType type)
{
    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';
    player->seat = seat;
    player->chips = chips;
    player->type = type;
    player->isActive = true;
}

void resetPlayer(Player *player)
{
    player->hand[0] = (Card){0, 0};
    player->hand[1] = (Card){0, 0};
    player->betSize = 0;
    player->isActive = true;
}

void dealHoleCards(Player *player, Card card1, Card card2)
{
    player->hand[0] = card1;
    player->hand[1] = card2;
}

void foldPlayer(Player *player)
{
    player->isActive = false;
}

bool isPlayerActive(const Player *player)
{
    return player->isActive;
}