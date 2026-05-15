
#include "gui.h"

const int WINDOW_HEIGHT = 900; 
const int WINDOW_WIDTH = 900; 
const char* TITLE = "ANTEATER POKER"; 
const char* POKER_TABLE_CSS = ".poker-bg {background-color: rgb(22,47,31);}"; 

GtkWidget* createWindow(GtkApplication* app){
    GtkWidget* window = gtk_application_window_new(app); 
    gtk_window_set_title(GTK_WINDOW(window), TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_HEIGHT, WINDOW_WIDTH);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_class(context, POKER_TABLE_CSS);
    
    return window; 
}

void create_poker_gui(GtkApplication *app, gpointer user_data){
    Poker_Gui* pokerGui = g_malloc(sizeof(Poker_Gui));
    pokerGui->Window =  createWindow(app); 
    gtk_widget_show_all(pokerGui->Window);  
}