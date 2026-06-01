#ifndef RENDER_H 
#define RENDER_H 

#include <gtk/gtk.h>
#include <math.h>
#include "enums.h"
#include "deck.h"
#include "playerbox_render.h"
#include "state.h"
#include "state_protocol.h"
#include "communication_bundle.h"

#define MAX_CARDS 57 //b/c we including back of card

extern const char* CARDS_RESOURCES[MAX_CARDS]; 

typedef struct Poker_Gui Poker_Gui;

typedef struct Color{
    double r; 
    double g; 
    double b; 
} Color; 

extern const Color CHIP_COUNT_TEXT_COLOR; 
extern const Color PLAYER_TEXT_COLOR;               //light yellow color
extern const Color POT_COLOR;                       //light green color
extern const Color TABLE_INNER_OUTLINE_COLOR;       //light green color
extern const Color TABLE_COLOR;                     //saturated green color 
extern const Color TABLE_RAIL_COLOR;                //wooden color 
extern const Color PLAYER_BOX_COLOR;                //dark blueish blackish color
extern const Color PLAYER_BOX_BORDER_COLOR;         //dark blueish grayish color
extern const Color AVATAR_BOX_BORDER_COLOR; 

typedef struct Seat_Info{
    double xPos; 
    double yPos; 
} Seat_Info; 

//COLOR FUNCTIONS 
double getRed(const Color color); 
double getGreen(const Color color); 
double getBlue(const Color color); 

//SEAT FUNCTIONS
Seat_Info seatInfoCtor(double xPos, double yPos); //hold's the player's seat

//IMAGE HELPER FUNCTIONS 
Icon* imageToSurface(const char *filename); //takes in the file path of an image and turns it into an image that gtk can render
void createImages(Icon** imgs, const char** resources, int numOfImgs); //creates all images from a certain path --> modifies Icon** img
void createCardImages(Icon** imgs, int numOfImgs); //creates all the card images
void createAvatarImages(Icon** imgs); //creates all the avatar images --> modifies Icon** imgs

Icon* getCardImage(Icon** images, Card card); 

void scale(cairo_t *cr, Icon *img, float targetXSize, float targetYSize); 
void drawImg(cairo_t* cr, Icon *img, float xPos, float yPos, float targetW, float targetH); 


//TEXT FUNCTIONS 
void drawText(cairo_t* cr, const Color textColor, const char* text, double fontSize, double xPos, double yPos); //draws text --> returns width of text
double getTextWidth(cairo_t* cr, const char* fontFace, const char* text, double fontSize);  //gets the width of text 

//DRAW FUNCTIONS
gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data); //draws the poker table --> including cards, pot, player box etc. 
void drawHiddenCards(cairo_t* cr, Icon** images, float xPos, float yPos, float targetW, float targetH); //draws the card backs  

//DEALER DRAW FUNCTIONS
void drawDealerCards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* dealerCards, int cardsToDeal); //draws the dealer cards at center of poker table

//PLAYER DRAW CARDS FUNCTIONS
void drawCards(cairo_t* cr, Icon** images, int numOfCards, Card* playerCards, double width, double height, double xPos, double yPos); //general function to draw cards
void drawPlayer1Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards); //draws player 1's cards at designated position (internal seat var)
void drawPlayer2Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards); //draws player 2's cards at designated position (internal seat var)
void drawPlayer3Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards); //draws player 3's cards at designated position (internal seat var)
void drawPlayer4Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards); //draws player 4's cards at designated position (internal seat var)
void drawPlayer5Cards(GtkWidget* pokerTable, cairo_t* cr, Icon** images, Card* playerCards); //draws player 5's cards at designated position (internal seat var)
double getCardGap(GtkWidget *pokerTable);
void drawBorder(cairo_t* cr, Color borderColor, double borderWidth, double width, double height, double xPos, double yPos);

//PLAYER DRAW HELPER FUNCTIONS 
void drawPlayerCards(cairo_t* cr, GtkWidget* pokerTable, Icon** images, Card* playerCards, Seat_Info* playerSeat); //draws a player's cards at a designated position (seat)

//PATH HELPER FUNCTIONS
void drawRoundedBoxPath(cairo_t* cr, double xPos, double yPos, double boxWidth, double boxHeight); //draws the rounded rect path for player info box
void roundedTablePath(cairo_t *cr, double x, double y, double w, double h); //draws the rounded pill shape path for table 

//POSITION HELPER FUNCTIONS
double getCenter(double top, double bot); 

#endif