#ifndef HAND_H
#define HAND_H

#include "deck.h"

#define BEST_HAND_SIZE 5
#define TOTAL_HAND_CARDS 7
#define HAND_SIZE 2

typedef struct {
    Card cards[2];
    int count;
} Hand;


typedef enum {
    HAND_HIGH_CARD,
    HAND_ONE_PAIR,
    HAND_TWO_PAIR,
    HAND_THREE_OF_KIND,
    HAND_STRAIGHT,
    HAND_FLUSH,
    HAND_FULL_HOUSE,
    HAND_FOUR_OF_KIND,
    HAND_STRAIGHT_FLUSH,
    HAND_ROYAL_FLUSH
} HandRank;

typedef struct {
    HandRank rank;
    Card best_cards[BEST_HAND_SIZE];
    int tie_values[BEST_HAND_SIZE];
} HandResult;


void init_hand(Hand *hand);
Card deal_hand(Hand *hand, Card card);
void clear_hand(Hand *hand);
void print_hand(Hand *hand);


//check
int eval_hand(Card player_cards[2]);
int eval_points(Card player_cards[2], Card board_cards[5]);

//compare hands
int compare_hands(Card p1_hand[2], Card p2_hand[2], Card board[5]);
//helper functions
int count_rank(Card cards[], int count, Rank rank);
int count_suit(Card cards[], int count, Suit suit);
int has_anteater(Card cards[], int count);
int count_anteaters(Card cards[], int count);

void sort(Card cards[], int count);


//checks wins
int royalFlush(Card cards[], int count);
int straightFlush(Card cards[], int count);
int four(Card cards[], int count);
int fullHouse(Card cards[], int count);
int flush(Card cards[], int count);
int straight(Card cards[], int count);
int three(Card cards[], int count);
int twoPair(Card cards[], int count);
int pairs(Card cards[], int count);
int highCard(Card cards[], int count);


/*
Royale Flush: 9
Straight Flush: 8
four: 7
full house: 6
flush: 5
straight: 4
three: 3
two pair: 2
pair: 1
*/

#endif
