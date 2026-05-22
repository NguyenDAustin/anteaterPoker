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

//deal all cards first --> return as array (card*)

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

void combine_cards(Card combined[], Hand *hand, Card board[], int board_count)
{
    int index = 0;

    for (int i = 0; i < hand->count; i++) {
        combined[index] = hand->cards[i];
        index++;
    }

    for (int i = 0; i < board_count; i++) {
        combined[index] = board[i];
        index++;
    }
}


void sort(Card cards[], int count)
{
    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            if (cards[j].rank > cards[j + 1].rank) {

                Card temp = cards[j];

                cards[j] = cards[j + 1];
                cards[j + 1] = temp;
            }
        }
    }


}

//i actually don't think we'd need this
int highCard(Card cards[], int count)
{
    for (int i = 0; i < count; i++) {

        // skip Anteater for now
        if (cards[i].type != ANTEATER_CARD) {
            return cards[i].rank;
        }
    }

    return ANTEATER;
}

int pairs(Card cards[], int count)
{
    int temp = 0;

    for (int i = 0; i < count - 1; i++) {

        // skip Anteater cards
        if (cards[i].type == ANTEATER_CARD) {
            continue;
        }

        if (cards[i].rank == cards[i + 1].rank) {

            temp++;

            // skip duplicate card pairs
            i++;
        }
    }

    return temp;
}

int twoPair(Card cards[], int count)
{
    return pairs(cards, count) >= 2;
}

int three(Card cards[], int count)
{
    for (int i = 0; i < count - 2; i++) {

        // skip Anteater cards
        if (cards[i].type == ANTEATER_CARD) {
            continue;
        }

        if (cards[i].rank == cards[i + 1].rank && cards[i].rank == cards[i + 2].rank) {

            return 1;
        }
    }

    return 0;
}

int straight(Card cards[], int count)
{
    int temp = 1;

    for (int i = 0; i < count - 1; i++) {

        if (cards[i].type == ANTEATER_CARD) {
            continue;
        }

        // skip duplicate ranks
        if (cards[i].rank == cards[i + 1].rank) {
            continue;
        }

        // consecutive cards
        if (cards[i + 1].rank == cards[i].rank + 1) {

            temp++;

            if (temp >= 5) {
                return 1;
            }
        }
        else {
            temp = 1;
        }
    }

    return 0;
}

int flush(Card cards[], int count)
{
    if (count_suit(cards, count, HEARTS) >= 5) {
        return 1;
    }

    if (count_suit(cards, count, DIAMONDS) >= 5) {
        return 1;
    }

    if (count_suit(cards, count, CLUBS) >= 5) {
        return 1;
    }

    if (count_suit(cards, count, SPADES) >= 5) {
        return 1;
    }

    return 0;

}

int fullHouse(Card cards[], int count)
{
    int tempThree = 0;
    int tempTwo = 0;

    for (int i = 0; i < count - 2; i++) {

        if (cards[i].rank == cards[i + 1].rank && cards[i].rank == cards[i + 2].rank) {

            tempThree = 1;
        }
    }

    for (int i = 0; i < count - 1; i++) {

        if (cards[i].rank == cards[i + 1].rank) {

            // make sure pair is different rank
            if (!(i < count - 2 && cards[i].rank == cards[i + 2].rank)) {

                tempTwo = 1;
            }
        }
    }

    return tempThree && tempTwo;
}

int four(Card cards[], int count)
{
    for (int i = 0; i < count - 3; i++) {

        if (cards[i].type == ANTEATER_CARD) {
            continue;
        }

        if (cards[i].rank == cards[i + 1].rank && cards[i].rank == cards[i + 2].rank && cards[i].rank == cards[i + 3].rank) {

            return 1;
        }
    }

    return 0;
}

int straightFlush(Card cards[], int count)
{
    Card suited_cards[7];
    int suited_count = 0;

    Suit suits[] = { HEARTS, DIAMONDS, CLUBS, SPADES };

    for (int s = 0; s < 4; s++) {

        suited_count = 0;

        //one suit
        for (int i = 0; i < count; i++) {

            if (cards[i].suit == suits[s]) {

                suited_cards[suited_count] = cards[i];
                suited_count++;
            }
        }

        //flush
        if (suited_count >= 5) {

            if (straight(suited_cards, suited_count)) {
                return 1;
            }
        }
    }

    return 0;
}

int royalFlush(Card cards[], int count)
{
    Card suited_cards[7];
    int suited_count = 0;

    Suit suits[] = { HEARTS, DIAMONDS, CLUBS, SPADES };

    for (int s = 0; s < 4; s++) {

        suited_count = 0;

        //one suit
        for (int i = 0; i < count; i++) {

            if (cards[i].suit == suits[s]) {

                suited_cards[suited_count] = cards[i];
                suited_count++;
            }
        }

        if (suited_count >= 5) {

            int has_ace = 0;
            int has_king = 0;
            int has_queen = 0;
            int has_jack = 0;
            int has_ten = 0;

            for (int i = 0; i < suited_count; i++) {

                if (suited_cards[i].rank == ACE) {
                    has_ace = 1;
                }
                else if (suited_cards[i].rank == KING) {
                    has_king = 1;
                }
                else if (suited_cards[i].rank == QUEEN) {
                    has_queen = 1;
                }
                else if (suited_cards[i].rank == JACK) {
                    has_jack = 1;
                }
                else if (suited_cards[i].rank == TEN) {
                    has_ten = 1;
                }
            }

            if (has_ace && has_king && has_queen && has_jack && has_ten) {
                return 1;
            }
        }
    }

    return 0;
}
//hand eval
//compare




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


int eval_hand(Card player_cards[2])
{
    int ans = 0;

    for (int i = 0; i < 2; i++)
    {
        ans += player_cards[i].rank;
    }

    return ans;
}

int eval_points(Card player_cards[2], Card board_cards[5])
{
    Card cards[7];
    int count = 7;

    sort(cards, count);

    //add cards from hand to a new hand
    for (int i = 0; i < 2; i++) {
        cards[i] = player_cards[i];
    }

    //add cards from board to new hand
    for (int i = 0; i < 5; i++) {
        cards[i + 2] = board_cards[i];
    }

    if (royalFlush(cards, count)) {
        return 9;
    }
    else if (straightFlush(cards, count)) {
        return 8;
    }
    else if (four(cards, count)) {
        return 7;
    }
    else if (fullHouse(cards, count)) {
        return 6;
    }
    else if (flush(cards, count)) {
        return 5;
    }
    else if (straight(cards, count)) {
        return 4;
    }
    else if (three(cards, count)) {
        return 3;
    }
    else if (twoPair(cards, count)) {
        return 2;
    }
    else if (pairs(cards, count)) {
        return 1;
    }
    else {
        return 0;
    }
}

int compare_hands(Card p1_hand[2], Card p2_hand[2], Card board[5])
{
    int p1_big = eval_points(p1_hand, board);
    int p2_big = eval_points(p2_hand, board);

    int p1_small = eval_points(p1_hand);
    int p2_small = eval_points(p2_hand);

    if (p1_big > p2_big){
        return 1; //player 1 wins
    }
    else if (p1_big < p2_big){ //to check
        return 2; //player 2 wins;
    }
    else if (p1_big == p2_big){
        if (p1_small > p2_small){
            return 1;
        }
        else if (p1_small > p2_small){
            return 2; 
        }
    }
    else {
        return 0; //split the pot
    }
}



//need to work on anteater stuff
//need to make it so that ACE can be 1 and 14