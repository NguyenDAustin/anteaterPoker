#ifndef RENDER_H 
#define RENDER_H 

#include <gtk/gtk.h>

#define MAX_CARDS 52
extern const char* CARDS_RESOURCES[MAX_CARDS]; 

typedef cairo_surface_t Icon; 

Icon* imageToSurface(const char *filename); 

void createImages(cairo_surface_t **imgs, int numOfImgs); 

void scale(cairo_t *cr, Icon *img, float targetSize);

void drawImg(cairo_t* cr, Icon *img); 

gboolean drawPokerTable(GtkWidget *widget, cairo_t *cr, gpointer user_data); 

#endif