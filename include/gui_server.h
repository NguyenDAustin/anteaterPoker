#ifndef GUI_SERVER_H
#define GUI_SERVER_H

#include <gtk/gtk.h>
#include "state.h"

typedef struct ServerWindow ServerWindow;
ServerWindow *createServerWindow(GtkApplication *app, GameState *gameState);
void updateServerWindow(ServerWindow *window, const char *stateMsg, GameState *gameState);
void destroyServerWindow(ServerWindow *window);

#endif 