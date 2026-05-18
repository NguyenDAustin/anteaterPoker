#include "player.h"

void initBot(Player *player, const char *name, int seat, int chips);
PlayerAction botTakeAction(Player *bot);
void monteCarlo();