#ifndef ENUM_H 
#define ENUM_H

#include <gtk/gtk.h>

typedef cairo_surface_t Icon;

enum {
    WINDOW_HEIGHT = 800, 
    WINDOW_WIDTH = 900,
    NUM_OF_RANKS = 13,
    MAX_PLAYERS = 6,
    MAX_PLAYER_CARDS = 2,
    MAX_DEALER_CARDS = 5,
    BACK_CARD_INDEX  = 56, //temp
    MAX_NUMBER_LENGTH = 32 //including commas
}; 

#endif 