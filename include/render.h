#ifndef RENDER_H 
#define RENDER_H 

#include "enums.h"
#include "deck.h"
#include <gtk/gtk.h>

#define MAX_CARDS 56
extern const char* CARDS_RESOURCES[MAX_CARDS]; 

typedef cairo_surface_t Icon; 

Icon* imageToSurface(const char *filename); 

void createImages(Icon** imgs, int numOfImgs); 

void scale(cairo_t *cr, Icon *img, float targetSize);



void drawImg(cairo_t* cr, Icon *img, float xPos, float yPos, float targetSize); 

void drawCard(cairo_t* cr, Icon* img, int xPos, int yPos); 

void drawPlayerCards(cairo_t* cr, Icon** cards); 

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data); 

#endif