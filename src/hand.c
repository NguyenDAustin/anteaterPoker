#include <stdio.h>
#include "hand.h"

void init_hand(Hand *hand)
{
    hand->count = 0;
}

void clear_hand(Hand *hand)
{
    hand->count = 0;
}

Card deal_hand(Hand *hand, Card card)
{
    if (hand->count >= HAND_SIZE) {
        printf("Error: hand is full.\n");
        return card;
    }

    hand->cards[hand->count] = card;
    hand->count++;

    return card;
}

void print_hand(Hand *hand)
{
    printf("Hand:\n");

    for (int i = 0; i < hand->count; i++) {
        printf("Card %d: ", i + 1);
        print_card(hand->cards[i]);
    }
}

int count_rank(Card cards[], int count, Rank rank)
{
    int total = 0;

    for (int i = 0; i < count; i++) {

        if (cards[i].rank == rank) {
            total++;
        }
    }

    return total;
}

int count_suit(Card cards[], int count, Suit suit)
{
    int total = 0;

    for (int i = 0; i < count; i++) {

        if (cards[i].suit == suit) {
            total++;
        }
    }

    return total;
}

int has_anteater(Card cards[], int count)
{
    for (int i = 0; i < count; i++) {

        if (cards[i].type == ANTEATER_CARD) {
            return 1;
        }
    }

    return 0;
}

int count_anteaters(Card cards[], int count)
{
    int total = 0;

    for (int i = 0; i < count; i++) {

        if (cards[i].type == ANTEATER_CARD) {
            total++;
        }
    }

    return total;
}

void sort(Card cards[], int count)
{
// add card to hand
// bubble sort


}
//hand eval
//compare

//checker
int multiples(Card cards[], int count)
{
    //add cards from board to hand
    //sort the cards
    //check the cards
}
