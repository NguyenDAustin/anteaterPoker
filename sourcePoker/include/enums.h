#ifndef ENUM_H 
#define ENUM_H


//ALL TYPEDEFS GO HERE
typedef struct _cairo_surface Icon;
typedef struct Player_Info Player;

enum {
    WINDOW_HEIGHT = 800, 
    WINDOW_WIDTH = 900,
    NUM_OF_RANKS = 14,//CHANGED - QUEENCY b/c anteater
    MAX_PLAYERS = 6,
    MAX_PLAYER_CARDS = 2,
    MAX_DEALER_CARDS = 5,
    BACK_CARD_INDEX  = 56, //temp
    MAX_NUMBER_LENGTH = 32 //including commas
}; 


#endif 
