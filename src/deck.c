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
        deck->cards[index].suit = ANTEATER_SUIT;
        deck->cards[index].type = ANTEATER_CARD;
        index++;
    }

    deck->top = 0;
}

void shuffle(Deck *deck) //fixed shuffle - queency
{
    srand(time(NULL));

    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
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

    const char *suit_names[] = {
        "Hearts",
        "Diamonds",
        "Clubs",
        "Spades",
        "Anteater Suit"
    };

    const char *rank_name = "Unknown";

    switch (card.rank) {
    case TWO: rank_name = "2"; break;
    case THREE: rank_name = "3"; break;
    case FOUR: rank_name = "4"; break;
    case FIVE: rank_name = "5"; break;
    case SIX: rank_name = "6"; break;
    case SEVEN: rank_name = "7"; break;
    case EIGHT: rank_name = "8"; break;
    case NINE: rank_name = "9"; break;
    case TEN: rank_name = "10"; break;
    case JACK: rank_name = "Jack"; break;
    case QUEEN: rank_name = "Queen"; break;
    case KING: rank_name = "King"; break;
    case ACE: rank_name = "Ace"; break;
    default: break;
    }

    printf("%s of %s\n", rank_name, suit_names[card.suit]); //display
}



//Queency Added Functions 
Card cardCtor(Suit suit, int rank){
    Card card; 
    card.suit = suit; 
    card.rank = rank; 
    card.type = (rank == ANTEATER || suit == ANTEATER_SUIT) ? ANTEATER_CARD : NORMAL_CARD;
    return card; 
} 
