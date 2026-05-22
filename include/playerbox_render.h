#ifndef PLAYERBOX_RENDER_H 
#define PLAYERBOX_RENDER_H 

#include <gtk/gtk.h>
#include "gui.h"
#include "enums.h"

typedef struct Seat_Info Seat_Info; 
typedef struct Poker_Gui Poker_Gui;
typedef struct Player_Info Player_Info; 

//HELPER FUNCTIONS
double getPlayerBoxHeight(GtkWidget* pokerTable);  
double getPlayerBoxWidth(GtkWidget* pokerTable); 

//DRAW HELPER FUNCTIONS
void drawRoundedBoxPath(cairo_t* cr, double x, double y, double boxWidth, double boxHeight); 
void drawAvatar(cairo_t* cr, Icon* avatarImg, double xPos, double yPos, double avatarWidth, double avatarHeight, double inset); 

//DRAW FUNCTIONS
void drawPlayerInfoBox(cairo_t* cr, GtkWidget* drawArea, Poker_Gui* pokerGui, Player_Info* playerInfo, double xPos, double yPos); 
void drawPlayerBoxes(cairo_t* cr, GtkWidget* pokerTable, Poker_Gui* pokerGui, Seat_Info* seats); 

void moneyBuilder(char* text, int chipCount); //turns number into text with commas

#endif