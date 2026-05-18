#include "card.h"
#include <stdbool.h>

typedef enum
{
    HUMAN_PLAYER,
    BOT_PLAYER
} PlayerType;

typedef struct
{
    char name[20];
    int seat;
    int chips;
    Card hand[2];
    int betSize;
    PlayerType type;
    bool isActive;
} Player;

typedef enum
{
    FOLD,
    CALL,
    RAISE
} PlayerAction;

void initPlayer(Player *player, const char *name, int seat, int chips, PlayerType type);
void resetPlayer(Player *player);
void dealHoleCards(Player *player, Card card1, Card card2);
void takeAction(Player *player, PlayerAction action, int amount);
void foldPlayer(Player *player);
bool isPlayerActive(const Player *player);