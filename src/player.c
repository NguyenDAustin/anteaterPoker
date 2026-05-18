#include <string.h>

#include "player.h"

void initPlayer(Player *player, const char *name, int seat, int chips, PlayerType type)
{
    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';
    player->seat = seat;
    player->chips = chips;
    player->betSize = 0;
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

void resetBetSize(Player *player)
{
    player->betSize = 0;
}

void takeAction(Player *player, PlayerAction action)
{
    int amount = action.amount;

    switch (action.actionType)
    {
        case FOLD:
            foldPlayer(player);
            break;
        case CHECK:
            break;
        case CALL:
        case BET:
        case RAISE:
            if (amount <= player->chips) {
                player->chips -= amount;
                player->betSize += amount;
            } else {
                player->betSize += player->chips;
                player->chips = 0;
            }
            break;
    }
}

void foldPlayer(Player *player)
{
    player->isActive = false;
}

bool isPlayerActive(const Player *player)
{
    return player->isActive;
}