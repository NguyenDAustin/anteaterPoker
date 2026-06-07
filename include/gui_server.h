#ifndef GUI_SERVER_H
#define GUI_SERVER_H

#include <gtk/gtk.h>
#include "state.h"
#include "render.h"

typedef struct Poker_Gui Poker_Gui;

typedef struct ServerWindow ServerWindow;
ServerWindow *createServerWindow(GtkApplication *app, Poker_Gui *pokerGui);
void updateServerWindow(ServerWindow *window, const char *stateMsg, GameState *gameState);
void destroyServerWindow(ServerWindow *window);

#endif 