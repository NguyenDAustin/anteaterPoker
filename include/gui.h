#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "render.h"
#include "enums.h"

extern const char* TITLE; 
extern const char* CSS; 
extern const char* POKER_TABLE; 
extern const int BUTTON_HEIGHT; 
extern const int BUTTON_WIDTH;


typedef struct Player_Info{ //not yet --> but I need this later
    char* name; 
    int chips; 
    Icon* avatarImg; 
    Card* playerCards; 
} Player_Info; 


typedef struct Poker_Gui{
    GtkWidget* Window; 

    //Drawing Areas 
    GtkWidget* pokerTable; 

    //image resources
    Icon** images; 
    Icon** avatarImages; 
    Icon* chipIcon;

    //Player Info 
    Player_Info* playerInfo; 

} Poker_Gui; 

//should have an array of all the avatar images


//HELPER FUNCTIONS 

void loadCss(GtkWidget* window, const char* CSS); 
//loads a given CSS code into GTK so child widgets can use

void setStyle(GtkWidget* widget, const char* CSS); 
//sets a given widget's style to the passed in CSS

GtkWidget* createWindow(GtkApplication* app); 
//creates the window 

GtkWidget* createMainContainer(); 
//creates the main container

void create_poker_gui(GtkApplication *app, gpointer user_data);

#endif