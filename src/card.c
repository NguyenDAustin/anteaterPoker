#include "card.h"


Card empty_card() {
    Card card;
    card.rank = -1; 
    card.suit = HEARTS; 
    card.type = NORMAL_CARD; 
    return card;
}
