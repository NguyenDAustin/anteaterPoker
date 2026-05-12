#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "deck.h"

void init_deck(Deck *deck) //initalize deck
{
    int index = 0;

    for (int suit = HEARTS; suit <= SPADES; suit++){
        for (Rank rank = TWO; rank <= ACE; rank++){
            deck->cards[index].rank = rank;
            deck->cards[index].suit = suit;
            deck->cards[index].type = NORMAL_CARD;
            index++;
        }
    }

    for (int i = 0; i < 4; i++){
        deck->cards[index].rank = ANTEATER;
        deck->cards[index].suit = ANTEATER;
        deck->cards[index].type = ANTEATER_CARD;
        index++;
    }

    deck->top = 0;
}

void shuffle(Deck *deck)
{
    srand(time(NULL));

    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i+1);

        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[i] = temp;
    }

    deck->top = 0;
}

Card deal(Deck *deck)
{
    if (deck->top >= DECK_SIZE){ //error check
        Card empty_card;
        empty_card.rank = ANTEATER;
        empty_card.suit = ANTEATER_SUIT;
        empty_card.type = ANTEATER_CARD;

        return empty_card;
    }

    Card dealt_card = deck->cards[deck->top]; //deals the card and moves the top
    deck->top++;

    return dealt_card;
} 

int cards_left(Deck *deck) { //update deck size 
    return DECK_SIZE - deck->top;
}


//this part is temp to test until team lead finishes the gui
 
void print_card(Card card) {
    if (card.type == ANTEATER_CARD) {
        printf("Anteater Wildcard\n");
        return;
    }

    const char *rank_names[] = {
        "Anteater",
        "ACE",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "Jack",
        "Queen",
        "King",
    };

    const char *suit_names[] = {
        "Hearts",
        "Diamonds",
        "Clubs",
        "Spades",
        "Anteater Suit"
    };

    printf("%s of %s\n", rank_names[card.rank], suit_names[card.suit]); //display
}