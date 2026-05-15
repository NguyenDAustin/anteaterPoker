#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

typedef struct Poker_Gui{
    GtkWidget* Window; 
} Poker_Gui; 

void create_poker_gui(GtkApplication *app, gpointer user_data);

#endif