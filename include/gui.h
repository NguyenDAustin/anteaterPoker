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

//GETTER FUNCTIONS
GtkWidget* getWindow(const Poker_Gui* pokerGui); 
GtkWidget* getPokerTable(const Poker_Gui* pokerGui); 
Icon** getImages(const Poker_Gui* pokerGui); 
Icon** getAvatarImages(const Poker_Gui* pokerGui); 
Icon* getChipIcon(const Poker_Gui* pokerGui); 
Player_Info* getPlayerInfo(const Poker_Gui* pokerGui); 
char* getPlayerName(const Player_Info* playerInfo); 
int getChipCount(const Player_Info* playerInfo); 
Icon* getAvatar(const Player_Info* playerInfo); 
Card* getPlayerCards(const Player_Info* playerInfo); 

//SETTER FUNCTIONS
void setWindow(Poker_Gui* pokerGui, GtkWidget* window); 
void setPokerTable(Poker_Gui* pokerGui, GtkWidget* pokerTable); 
void setImages(Poker_Gui* pokerGui, Icon** cardImages); 
void setAvatarImages(Poker_Gui* pokerGui, Icon** avatarImages); 
void setChipIcon(Poker_Gui* pokerGui, Icon* chipIcon); 
void setPlayerInfo(Poker_Gui* pokerGui, Player_Info* playerInfo); 
void setPlayerName(Player_Info* playerInfo, const char* playerName);  
void setChipCount(Player_Info* playerInfo, int chipCount); 
void setAvatar(Player_Info* playerInfo, Icon* avatarImg);
void setPlayerCards(Player_Info* playerInfo, Card* playerCards); 

//HELPER FUNCTIONS 

void loadCss(GtkWidget* window, const char* CSS); //loads a given CSS code into GTK so child widgets can use

void setStyle(GtkWidget* widget, const char* CSS); //sets a given widget's style to the passed in CSS

GtkWidget* createWindow(GtkApplication* app); //creates the window 

GtkWidget* createMainContainer(); //creates the main container

void create_poker_gui(GtkApplication *app, gpointer user_data); //creates the poker gui --> the window, the data, etc

#endif