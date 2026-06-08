#include <stdio.h>
#include "hand.h"
#include "deck.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

static Card normal_card(Suit suit, Rank rank)
{
    Card card = cardCtor(suit, rank);
    card.type = NORMAL_CARD;
    return card;
}

int main(void)
{
    Hand hand;
    init_hand(&hand);
    CHECK(hand.count == 0, "init_hand should set count to 0");

    Card c1 = normal_card(HEARTS, ACE);
    Card c2 = normal_card(HEARTS, KING);
    deal_hand(&hand, c1);
    deal_hand(&hand, c2);
    CHECK(hand.count == 2, "deal_hand should have filled both slots");

    Card got = get_hand_card(&hand, 0);
    CHECK(got.rank == ACE, "get_hand_card slot 0 should be ACE");

    /* one pair */
    Card pair_cards[7] = {
        normal_card(HEARTS, KING),
        normal_card(SPADES, KING),
        normal_card(CLUBS, TWO),
        normal_card(DIAMONDS, FIVE),
        normal_card(HEARTS, SEVEN),
        normal_card(SPADES, NINE),
        normal_card(CLUBS, JACK)
    };
    CHECK(pairs(pair_cards, 7) > 0, "expected to detect a pair of kings");

    /* flush */
    Card flush_cards[7] = {
        normal_card(HEARTS, TWO),
        normal_card(HEARTS, FIVE),
        normal_card(HEARTS, SEVEN),
        normal_card(HEARTS, NINE),
        normal_card(HEARTS, JACK),
        normal_card(SPADES, KING),
        normal_card(CLUBS, THREE)
    };
    CHECK(flush(flush_cards, 7) > 0, "expected to detect a flush");

    Card royal_hand[2] = {
        normal_card(HEARTS, ACE),
        normal_card(HEARTS, KING)
    };
    Card royal_board[5] = {
        normal_card(HEARTS, QUEEN),
        normal_card(HEARTS, JACK),
        normal_card(HEARTS, TEN),
        normal_card(CLUBS, TWO),
        normal_card(DIAMONDS, THREE)
    };
    CHECK(eval_points(royal_hand, royal_board) == 9,
          "expected royal flush to rank highest");

    Card straight_flush_hand[2] = {
        normal_card(HEARTS, NINE),
        normal_card(HEARTS, EIGHT)
    };
    Card straight_flush_board[5] = {
        normal_card(HEARTS, SEVEN),
        normal_card(HEARTS, SIX),
        normal_card(HEARTS, FIVE),
        normal_card(CLUBS, TWO),
        normal_card(DIAMONDS, THREE)
    };
    CHECK(eval_points(straight_flush_hand, straight_flush_board) == 8,
          "expected straight flush");

    Card four_hand[2] = {
        normal_card(HEARTS, NINE),
        normal_card(SPADES, NINE)
    };
    Card four_board[5] = {
        normal_card(CLUBS, NINE),
        normal_card(DIAMONDS, NINE),
        normal_card(HEARTS, FIVE),
        normal_card(CLUBS, TWO),
        normal_card(DIAMONDS, THREE)
    };
    CHECK(eval_points(four_hand, four_board) == 7,
          "expected four of a kind");

    Card full_house_hand[2] = {
        normal_card(HEARTS, NINE),
        normal_card(SPADES, NINE)
    };
    Card full_house_board[5] = {
        normal_card(CLUBS, NINE),
        normal_card(DIAMONDS, FIVE),
        normal_card(HEARTS, FIVE),
        normal_card(CLUBS, TWO),
        normal_card(DIAMONDS, THREE)
    };
    CHECK(eval_points(full_house_hand, full_house_board) == 6,
          "expected full house");

    Card eval_flush_hand[2] = {
        normal_card(HEARTS, ACE),
        normal_card(HEARTS, KING)
    };
    Card eval_flush_board[5] = {
        normal_card(HEARTS, TWO),
        normal_card(HEARTS, FIVE),
        normal_card(HEARTS, NINE),
        normal_card(CLUBS, THREE),
        normal_card(DIAMONDS, FOUR)
    };
    CHECK(eval_points(eval_flush_hand, eval_flush_board) == 5,
          "expected flush");

    Card wheel_hand[2] = {
        normal_card(HEARTS, ACE),
        normal_card(SPADES, TWO)
    };
    Card wheel_board[5] = {
        normal_card(CLUBS, THREE),
        normal_card(DIAMONDS, FOUR),
        normal_card(HEARTS, FIVE),
        normal_card(CLUBS, NINE),
        normal_card(DIAMONDS, JACK)
    };
    CHECK(eval_points(wheel_hand, wheel_board) == 4,
          "expected ace low straight");

    Card trips_hand[2] = {
        normal_card(HEARTS, SEVEN),
        normal_card(SPADES, SEVEN)
    };
    Card trips_board[5] = {
        normal_card(CLUBS, SEVEN),
        normal_card(DIAMONDS, TWO),
        normal_card(HEARTS, FIVE),
        normal_card(CLUBS, NINE),
        normal_card(DIAMONDS, JACK)
    };
    CHECK(eval_points(trips_hand, trips_board) == 3,
          "expected three of a kind");

    Card two_pair_hand[2] = {
        normal_card(HEARTS, ACE),
        normal_card(SPADES, ACE)
    };
    Card two_pair_board[5] = {
        normal_card(CLUBS, KING),
        normal_card(DIAMONDS, KING),
        normal_card(HEARTS, FIVE),
        normal_card(CLUBS, NINE),
        normal_card(DIAMONDS, JACK)
    };
    CHECK(eval_points(two_pair_hand, two_pair_board) == 2,
          "expected two pair");

    clear_hand(&hand);
    CHECK(hand.count == 0, "clear_hand should reset count to 0");

    printf("hand test passed\n");
    return 0;
}
