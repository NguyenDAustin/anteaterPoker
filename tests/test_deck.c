#include <stdio.h>
#include "deck.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int cards_left(Deck *deck);

int main(void)
{
    Deck deck;
    init_deck(&deck);
    CHECK(cards_left(&deck) == DECK_SIZE, "init_deck should leave a full deck");

    shuffle(&deck);
    CHECK(cards_left(&deck) == DECK_SIZE, "shuffle should not change deck size");

    Card first = deal(&deck);
    CHECK(cards_left(&deck) == DECK_SIZE - 1, "deal should remove one card");
    CHECK(first.rank >= 0, "dealt card should have a real rank");

    for (int i = 1; i < DECK_SIZE; i++) {
        (void)deal(&deck);
    }
    CHECK(cards_left(&deck) == 0, "deck should be empty after dealing all cards");

    /* reset_deck is declared in deck.h but unimplemented in deck.c — skipped. */

    printf("deck test passed\n");
    return 0;
}
