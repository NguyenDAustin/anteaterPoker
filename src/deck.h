// deck.h
#ifndef DECK_H
#define DECK_H

#define DECK_SIZE 56

typedef enum { 
	ANTEATER = 0, 
	ACE= 1, 
	TWO= 2, 
	THREE= 3, 
	FOUR = 4, 
	FIVE = 5, 
	SIX = 6, 
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13
} Rank; 

typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
    ANTEATER_SUIT
} Suit;

typedef enum {
    NORMAL_CARD,
    ANTEATER_CARD
} Cardtype;

typedef struct {
    int rank;
    Suit suit;
    Cardtype type;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    int top;
} Deck;

void init_deck(Deck *deck);

void shuffle(Deck *deck);

Card deal(Deck *deck);

void reset_deck(Deck *deck);

void print_card(Card card);

#endif