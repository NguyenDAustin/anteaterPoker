#include <stdio.h>
#include "hand.h"
#include "deck.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    Hand hand;
    init_hand(&hand);
    CHECK(hand.count == 0, "init_hand should set count to 0");

    Card c1 = cardCtor(HEARTS, ACE);
    Card c2 = cardCtor(HEARTS, KING);
    deal_hand(&hand, c1);
    deal_hand(&hand, c2);
    CHECK(hand.count == 2, "deal_hand should have filled both slots");

    Card got = get_hand_card(&hand, 0);
    CHECK(got.rank == ACE, "get_hand_card slot 0 should be ACE");

    /* one pair */
    Card pair_cards[7] = {
        cardCtor(HEARTS, KING),
        cardCtor(SPADES, KING),
        cardCtor(CLUBS, TWO),
        cardCtor(DIAMONDS, FIVE),
        cardCtor(HEARTS, SEVEN),
        cardCtor(SPADES, NINE),
        cardCtor(CLUBS, JACK)
    };
    CHECK(pairs(pair_cards, 7) > 0, "expected to detect a pair of kings");

    /* flush */
    Card flush_cards[7] = {
        cardCtor(HEARTS, TWO),
        cardCtor(HEARTS, FIVE),
        cardCtor(HEARTS, SEVEN),
        cardCtor(HEARTS, NINE),
        cardCtor(HEARTS, JACK),
        cardCtor(SPADES, KING),
        cardCtor(CLUBS, THREE)
    };
    CHECK(flush(flush_cards, 7) > 0, "expected to detect a flush");

    clear_hand(&hand);
    CHECK(hand.count == 0, "clear_hand should reset count to 0");

    printf("hand test passed\n");
    return 0;
}
