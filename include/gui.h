#ifndef GUI_H
#define GUI_H

#include <fontconfig/fontconfig.h>
#include <gtk/gtk.h>
#include "render.h"
#include "enums.h"

extern const char *TITLE;
extern const char *CSS;
extern const char *POKER_TABLE;
extern const int BUTTON_HEIGHT;
extern const int BUTTON_WIDTH;

typedef struct Player_Info
{ // not yet --> but I need this later
    char *name;
    int chips; // Whoever is in charge of chip count --> you will need to communicate it to me
    Icon *avatarImg;
    Card *playerCards; // Austin you need to give me this -> or give naveen this to give me
} Player_Info;

typedef struct Poker_Gui
{
    GtkWidget *Window;

    // Drawing Areas
    GtkWidget *pokerTable;
    GtkWidget *raiseSlider;

    // image resources
    Icon **images;
    Icon **avatarImages;
    Icon *chipIcon;

    // Player Infos
    Player_Info **playerInfo; // contains the info of all players --> put NULL if not active player for player cards
                              //--> Naveen I think you will need to communicate this to me

    int socket;

    // stuff other people need to communicate to me i
    int activePlayer;  // Naveen this is you -> client socket -> player number
    int turn;          // Mia this is you -> game state
    int pot;           // whoever is in charge of calculating pot amount --> I think it might be you Mia
    Card *dealerCards; // Austin this is you --> give me all 5 cards at once

} Poker_Gui;

// GETTER FUNCTIONS FOR GUI
GtkWidget *getWindow(const Poker_Gui *pokerGui);
GtkWidget *getPokerTable(const Poker_Gui *pokerGui);
GtkWidget *getRaiseSlider(const Poker_Gui *pokerGui);
Icon **getImages(const Poker_Gui *pokerGui);
Icon **getAvatarImages(const Poker_Gui *pokerGui);
Icon *getChipIcon(const Poker_Gui *pokerGui);

// GETTER FUNCTIONS FOR PLAYER INFO- GIVEN A PLAYER NUM
Player_Info **getAllPlayersInfo(const Poker_Gui *pokerGui);
Player_Info *getPlayerInfo(const Poker_Gui *pokerGui, int playerNum);
char *getPlayerName(const Player_Info **playerInfo, int playerNum);
int getPlayerChipCount(const Player_Info **playerInfo, int playerNum);
Icon *getPlayerAvatar(const Player_Info **playerInfo, int playerNum);
Card *getPlayerCards(const Player_Info **playerInfo, int playerNum);

// GETTER FUNCTIONS FOR GAME STATE STUFF
int getPot(const Poker_Gui *pokerGui);
int getSocket(const Poker_Gui *pokerGui);

// SETTER FUNCTIONS FOR GUI
void setWindow(Poker_Gui *pokerGui, GtkWidget *window);
void setPokerTable(Poker_Gui *pokerGui, GtkWidget *pokerTable);
void setRaiseSlider(Poker_Gui *pokerGui, GtkWidget *raiseSlider);
void setImages(Poker_Gui *pokerGui, Icon **cardImages);
void setAvatarImages(Poker_Gui *pokerGui, Icon **avatarImages);
void setChipIcon(Poker_Gui *pokerGui, Icon *chipIcon);

// SETTER FUNCTIONS FOR PLAYER INFO - GIVEN PLAYER NUM
void setPlayerInfo(Poker_Gui *pokerGui, Player_Info *playerInfo, int playerNum);
void setPlayerName(Player_Info **playerInfo, const char *playerName, int playerNum);
void setChipCount(Player_Info **playerInfo, int chipCount, int playerNum);
void setAvatar(Player_Info **playerInfo, Icon *avatarImg, int playerNum);
void setPlayerCards(Player_Info **playerInfo, Card *playerCards, int playerNum);

// SETTER FUNCTIONS FOR GAME STATE
void setPot(Poker_Gui *pokerGui, int potAmt);

// HELPER FUNCTIONS
void loadCss(GtkWidget *window, const char *CSS); // loads a given CSS code into GTK so child widgets can use
void loadFont(const char *fontPath);              // loads a font face into GTK so drawText can use

void setStyle(GtkWidget *widget, const char *CSS); // sets a given widget's style to the passed in CSS

GtkWidget *createWindow(GtkApplication *app); // creates the window

GtkWidget *createMainContainer(); // creates the main container

void create_poker_gui(GtkApplication *app, gpointer user_data); // creates the poker gui --> the window, the data, etc

void setPlayerNames(Poker_Gui *pokerGui, char **names); // passed in an array of all player names --> sets player info

#endif