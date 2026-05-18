#include "bot.h"

void initBot(Player *player, const char *name, int seat, int chips)
{
    initPlayer(player, name, seat, chips, BOT_PLAYER);
}
