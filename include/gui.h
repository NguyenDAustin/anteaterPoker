#ifndef GUI_H
#define GUI_H

#include <fontconfig/fontconfig.h>
#include <gtk/gtk.h>
#include "player.h"
#include "render.h"
#include "enums.h"
#include "state.h"
#include "communication_bundle.h"
#include "timer.h"

extern const char *TITLE;
extern const char *CSS;
extern const char *POKER_TABLE;
extern const int BUTTON_HEIGHT;
extern const int BUTTON_WIDTH;


typedef struct Poker_Gui
{
    GtkWidget *Window;

    // Drawing Areas
    GtkWidget *pokerTable;
    GtkWidget *raiseSlider;
    GtkWidget* nextRoundButton; 

    // image resources
    Icon **images;
    Icon **avatarImages;
    Icon *chipIcon; 

    //Game State 
    GameState* gameState; 

    //important stuff
    int socket;
    int playerNum; 
    const char* stateMsg; 

    //timer stuff
    Timer *timer;

} Poker_Gui;


// GETTER FUNCTIONS FOR GUI
GtkWidget *getWindow(const Poker_Gui *pokerGui);
GtkWidget *getPokerTable(const Poker_Gui *pokerGui);
GtkWidget *getRaiseSlider(const Poker_Gui *pokerGui);
Icon **getImages(const Poker_Gui *pokerGui);
Icon **getAvatarImages(const Poker_Gui *pokerGui);
Icon *getChipIcon(const Poker_Gui *pokerGui);
GameState* getGameState(const Poker_Gui* pokerGui);
int getSocket(const Poker_Gui *pokerGui);

// SETTER FUNCTIONS FOR GUI
void setWindow(Poker_Gui *pokerGui, GtkWidget *window);
void setPokerTable(Poker_Gui *pokerGui, GtkWidget *pokerTable);
void setRaiseSlider(Poker_Gui *pokerGui, GtkWidget *raiseSlider);
void setImages(Poker_Gui *pokerGui, Icon **cardImages);
void setAvatarImages(Poker_Gui *pokerGui, Icon **avatarImages);
void setChipIcon(Poker_Gui *pokerGui, Icon *chipIcon);
void setGameState(Poker_Gui* pokerGui, GameState* gameState);

// HELPER FUNCTIONS
void loadCss(GtkWidget *window, const char *CSS); // loads a given CSS code into GTK so child widgets can use
void loadFont(const char *fontPath);              // loads a font face into GTK so drawText can use

void setStyle(GtkWidget *widget, const char *CSS); // sets a given widget's style to the passed in CSS

GtkWidget *createWindow(GtkApplication *app); // creates the window

GtkWidget *createMainContainer(); // creates the main container

void create_poker_gui(GtkApplication *app, gpointer user_data); // creates the poker gui --> the window, the data, etc
void createWaitingRoom(GtkApplication *app, gpointer user_data); // creates the lobby gui before the poker table

void setPlayerNames(Poker_Gui *pokerGui, char **names); // passed in an array of all player names --> sets player info

#endif
