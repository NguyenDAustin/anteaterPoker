#include <stdio.h>
#include "card.h"
#include "deck.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    Card c = empty_card();
    CHECK(c.rank == -1, "empty_card rank should be -1");
    CHECK(c.type == NORMAL_CARD, "empty_card type should be NORMAL_CARD");

    Card a = cardCtor(SPADES, ACE);
    CHECK(a.rank == ACE, "cardCtor rank mismatch");
    CHECK(a.suit == SPADES, "cardCtor suit mismatch");

    Card anteater = cardCtor(ANTEATER_SUIT, ANTEATER);
    CHECK(anteater.suit == ANTEATER_SUIT, "anteater suit mismatch");

    printf("card test passed\n");
    return 0;
}
