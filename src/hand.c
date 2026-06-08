#include <stdio.h>
#include "hand.h"


#define BEST_HAND_SIZE 5
#define TOTAL_HAND_CARDS 7
#define HAND_SIZE 2
#define BOARD_SIZE 5
#define RANK_LIMIT 15
#define SUIT_LIMIT 4

static void clear_result(HandResult *result)
{
    result->rank = HAND_HIGH_CARD;
    for (int i = 0; i < BEST_HAND_SIZE; i++) {
        result->best_cards[i].rank = ANTEATER;
        result->best_cards[i].suit = ANTEATER_SUIT;
        result->best_cards[i].type = ANTEATER_CARD;
        result->tie_values[i] = 0;
    }
}

static int compare_results(const HandResult *a, const HandResult *b)
{
    if (a->rank > b->rank) {
        return 1;
    }
    if (a->rank < b->rank) {
        return -1;
    }

    for (int i = 0; i < BEST_HAND_SIZE; i++) {
        if (a->tie_values[i] > b->tie_values[i]) {
            return 1;
        }
        if (a->tie_values[i] < b->tie_values[i]) {
            return -1;
        }
    }

    return 0;
}

static void count_cards(Card cards[], int count, int rank_counts[RANK_LIMIT],
                        int suit_counts[SUIT_LIMIT],
                        int rank_present[RANK_LIMIT],
                        int suit_rank_present[SUIT_LIMIT][RANK_LIMIT])
{
    for (int i = 0; i < RANK_LIMIT; i++) {
        rank_counts[i] = 0;
        rank_present[i] = 0;
    }

    for (int suit = 0; suit < SUIT_LIMIT; suit++) {
        suit_counts[suit] = 0;
        for (int rank = 0; rank < RANK_LIMIT; rank++) {
            suit_rank_present[suit][rank] = 0;
        }
    }

    for (int i = 0; i < count; i++) {
        if (cards[i].type == ANTEATER_CARD) {
            continue;
        }
        if (cards[i].rank < TWO || cards[i].rank > ACE) {
            continue;
        }
        if (cards[i].suit < HEARTS || cards[i].suit > SPADES) {
            continue;
        }

        rank_counts[cards[i].rank]++;
        rank_present[cards[i].rank] = 1;
        suit_counts[cards[i].suit]++;
        suit_rank_present[cards[i].suit][cards[i].rank] = 1;
    }
}

static int best_straight_high(int present[RANK_LIMIT])
{
    for (int high = ACE; high >= SIX; high--) {
        int found = 1;
        for (int rank = high; rank > high - 5; rank--) {
            if (!present[rank]) {
                found = 0;
                break;
            }
        }
        if (found) {
            return high;
        }
    }

    if (present[ACE] && present[TWO] && present[THREE] && present[FOUR] && present[FIVE]) {
        return FIVE;
    }

    return 0;
}

static void fill_high_cards_from_counts(int rank_counts[RANK_LIMIT], int tie_values[BEST_HAND_SIZE],
                                        int start, int needed, int skip1, int skip2)
{
    int index = start;

    for (int rank = ACE; rank >= TWO && index < start + needed; rank--) {
        if (rank == skip1 || rank == skip2 || rank_counts[rank] <= 0) {
            continue;
        }

        tie_values[index] = rank;
        index++;
    }
}

static HandResult evaluate_cards_no_wild(Card cards[], int count)
{
    HandResult result;
    int rank_counts[RANK_LIMIT];
    int suit_counts[SUIT_LIMIT];
    int rank_present[RANK_LIMIT];
    int suit_rank_present[SUIT_LIMIT][RANK_LIMIT];

    clear_result(&result);
    count_cards(cards, count, rank_counts, suit_counts, rank_present, suit_rank_present);

    int best_sf_high = 0;
    for (int suit = HEARTS; suit <= SPADES; suit++) {
        if (suit_counts[suit] >= 5) {
            int high = best_straight_high(suit_rank_present[suit]);
            if (high > best_sf_high) {
                best_sf_high = high;
            }
        }
    }

    if (best_sf_high == ACE) {
        result.rank = HAND_ROYAL_FLUSH;
        result.tie_values[0] = ACE;
        return result;
    }
    if (best_sf_high > 0) {
        result.rank = HAND_STRAIGHT_FLUSH;
        result.tie_values[0] = best_sf_high;
        return result;
    }

    for (int rank = ACE; rank >= TWO; rank--) {
        if (rank_counts[rank] >= 4) {
            result.rank = HAND_FOUR_OF_KIND;
            result.tie_values[0] = rank;
            fill_high_cards_from_counts(rank_counts, result.tie_values, 1, 1, rank, 0);
            return result;
        }
    }

    int trip_rank = 0;
    int pair_rank = 0;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (rank_counts[rank] >= 3 && trip_rank == 0) {
            trip_rank = rank;
        }
        else if (rank_counts[rank] >= 2 && pair_rank == 0) {
            pair_rank = rank;
        }
    }

    if (trip_rank > 0) {
        for (int rank = ACE; rank >= TWO; rank--) {
            if (rank != trip_rank && rank_counts[rank] >= 2) {
                pair_rank = rank;
                break;
            }
        }
    }

    if (trip_rank > 0 && pair_rank > 0) {
        result.rank = HAND_FULL_HOUSE;
        result.tie_values[0] = trip_rank;
        result.tie_values[1] = pair_rank;
        return result;
    }

    for (int suit = HEARTS; suit <= SPADES; suit++) {
        if (suit_counts[suit] >= 5) {
            result.rank = HAND_FLUSH;
            int index = 0;
            for (int rank = ACE; rank >= TWO && index < BEST_HAND_SIZE; rank--) {
                if (suit_rank_present[suit][rank]) {
                    result.tie_values[index] = rank;
                    index++;
                }
            }
            return result;
        }
    }

    int straight_high = best_straight_high(rank_present);
    if (straight_high > 0) {
        result.rank = HAND_STRAIGHT;
        result.tie_values[0] = straight_high;
        return result;
    }

    if (trip_rank > 0) {
        result.rank = HAND_THREE_OF_KIND;
        result.tie_values[0] = trip_rank;
        fill_high_cards_from_counts(rank_counts, result.tie_values, 1, 2, trip_rank, 0);
        return result;
    }

    int high_pair = 0;
    int low_pair = 0;
    for (int rank = ACE; rank >= TWO; rank--) {
        if (rank_counts[rank] >= 2) {
            if (high_pair == 0) {
                high_pair = rank;
            }
            else {
                low_pair = rank;
                break;
            }
        }
    }

    if (high_pair > 0 && low_pair > 0) {
        result.rank = HAND_TWO_PAIR;
        result.tie_values[0] = high_pair;
        result.tie_values[1] = low_pair;
        fill_high_cards_from_counts(rank_counts, result.tie_values, 2, 1, high_pair, low_pair);
        return result;
    }

    if (high_pair > 0) {
        result.rank = HAND_ONE_PAIR;
        result.tie_values[0] = high_pair;
        fill_high_cards_from_counts(rank_counts, result.tie_values, 1, 3, high_pair, 0);
        return result;
    }

    result.rank = HAND_HIGH_CARD;
    fill_high_cards_from_counts(rank_counts, result.tie_values, 0, BEST_HAND_SIZE, 0, 0);
    return result;
}

static HandResult evaluate_player_cards(Card player_cards[2], Card board_cards[5])
{
    HandResult best;
    int found = 0;
    Card hand[HAND_SIZE] = { player_cards[0], player_cards[1] };

    clear_result(&best);

    for (Rank first_rank = TWO; first_rank <= ACE; first_rank++) {
        for (Suit first_suit = HEARTS; first_suit <= SPADES; first_suit++) {
            Card first_card = hand[0];
            if (hand[0].type == ANTEATER_CARD) {
                first_card.rank = first_rank;
                first_card.suit = first_suit;
                first_card.type = NORMAL_CARD;
            }

            for (Rank second_rank = TWO; second_rank <= ACE; second_rank++) {
                for (Suit second_suit = HEARTS; second_suit <= SPADES; second_suit++) {
                    Card cards[TOTAL_HAND_CARDS];
                    Card second_card = hand[1];

                    if (hand[1].type == ANTEATER_CARD) {
                        second_card.rank = second_rank;
                        second_card.suit = second_suit;
                        second_card.type = NORMAL_CARD;
                    }
                    else if (second_rank != TWO || second_suit != HEARTS) {
                        continue;
                    }

                    cards[0] = first_card;
                    cards[1] = second_card;
                    for (int i = 0; i < BOARD_SIZE; i++) {
                        cards[i + HAND_SIZE] = board_cards[i];
                    }

                    HandResult current = evaluate_cards_no_wild(cards, TOTAL_HAND_CARDS);
                    if (!found || compare_results(&current, &best) > 0) {
                        best = current;
                        found = 1;
                    }
                }
            }

            if (hand[0].type != ANTEATER_CARD) {
                break;
            }
        }
        if (hand[0].type != ANTEATER_CARD) {
            break;
        }
    }

    return best;
}

void init_hand(Hand *hand) //initialize hand
{
    hand->count = 0;
}

void clear_hand(Hand *hand) //clear hand
{
    hand->count = 0;
}

Card deal_hand(Hand *hand, Card card) //draw cards
{
    if (hand->count >= HAND_SIZE) {
        printf("Error: hand is full.\n");
        return card;
    }

    hand->cards[hand->count] = card;
    hand->count++;

    return card;
}

void draw_hand(Deck *deck, Hand *hand) //this is your array queency for hand
{
    clear_hand(hand);

    for (int i = 0; i < HAND_SIZE; i++) {
        deal_hand(hand, deal(deck));
    }
}


//this is your array queency but for the hand
void draw_board(Deck *deck, Card board[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = deal(deck);
    }

    anteater_board(deck, board);
}

void anteater_board(Deck *deck, Card board[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        while (board[i].type == ANTEATER_CARD) {
            board[i] = deal(deck);
        }
    }
}


void print_hand(Hand *hand) //display for test
{
    printf("Hand:\n");

    for (int i = 0; i < hand->count; i++) {
        printf("Card %d: ", i + 1);
        print_card(hand->cards[i]);
    }
}

void print_board(Card board[BOARD_SIZE]) //display for test
{
    printf("Board:\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("Card %d: ", i + 1);
        print_card(board[i]);
    }
}



//hand getter
Card get_hand_card(Hand *hand, int index)
{
    //error check
    if (index < 0 || index >= hand->count) {
        printf("Error: invalid hand index.\n");
        return hand->cards[0]; // fallback
    }

    return hand->cards[index];
}

//hand setter
void set_hand_card(Hand *hand, int index, Card card)
{
    //error check
    if (index < 0 || index >= HAND_SIZE) {
        printf("Error: invalid hand index.\n");
        return;
    }

    hand->cards[index] = card;

    if (index >= hand->count) {
        hand->count = index + 1;
    }
}

Card get_board_card(Card board[BOARD_SIZE], int index)
{
    //error check
    if (index < 0 || index >= BOARD_SIZE) {
        printf("Error: invalid board index.\n");
        return board[0];
    }

    return board[index];
}

void set_board_card(Card board[BOARD_SIZE], int index, Card card)
{
    //error check
    if (index < 0 || index >= BOARD_SIZE) {
        printf("Error: invalid board index.\n");
        return;
    }

    board[index] = card;
}

void combine_cards(Card combined[], Hand *hand, Card board[], int board_count) //for eval
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


void sort(Card cards[], int count) //helper
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
    int best = ANTEATER;

    for (int i = 0; i < count; i++) {
        if (cards[i].type != ANTEATER_CARD && cards[i].rank > best) {
            best = cards[i].rank;
        }
    }

    return best;
}

int pairs(Card cards[], int count)
{
    int rank_counts[RANK_LIMIT] = {0};
    int total = 0;

    for (int i = 0; i < count; i++) {
        if (cards[i].type != ANTEATER_CARD && cards[i].rank >= TWO && cards[i].rank <= ACE) {
            rank_counts[cards[i].rank]++;
        }
    }

    for (int rank = TWO; rank <= ACE; rank++) {
        if (rank_counts[rank] >= 2) {
            total++;
        }
    }

    return total;
}

int twoPair(Card cards[], int count)
{
    return pairs(cards, count) >= 2;
}

int three(Card cards[], int count)
{
    for (Rank rank = TWO; rank <= ACE; rank++) {
        if (count_rank(cards, count, rank) >= 3) {
            return 1;
        }
    }

    return 0;
}

int straight(Card cards[], int count)
{
    int present[RANK_LIMIT] = {0};

    for (int i = 0; i < count; i++) {
        if (cards[i].type != ANTEATER_CARD && cards[i].rank >= TWO && cards[i].rank <= ACE) {
            present[cards[i].rank] = 1;
        }
    }

    return best_straight_high(present) > 0;
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
    int trip_rank = 0;

    for (Rank rank = ACE; rank >= TWO; rank--) {
        if (count_rank(cards, count, rank) >= 3) {
            trip_rank = rank;
            break;
        }
    }

    if (trip_rank == 0) {
        return 0;
    }

    for (Rank rank = ACE; rank >= TWO; rank--) {
        if (rank != trip_rank && count_rank(cards, count, rank) >= 2) {
            return 1;
        }
    }

    return 0;
}

int four(Card cards[], int count)
{
    for (Rank rank = TWO; rank <= ACE; rank++) {
        if (count_rank(cards, count, rank) >= 4) {
            return 1;
        }
    }

    return 0;
}

int straightFlush(Card cards[], int count)
{
    HandResult result = evaluate_cards_no_wild(cards, count);

    if (result.rank == HAND_STRAIGHT_FLUSH || result.rank == HAND_ROYAL_FLUSH) {
        return 1;
    }

    return 0;
}

int royalFlush(Card cards[], int count)
{
    HandResult result = evaluate_cards_no_wild(cards, count);

    if (result.rank == HAND_ROYAL_FLUSH) {
        return 1;
    }

    return 0;
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

Card best_anteater_card(Card player_cards[2], Card board_cards[5])
{
    Card best_card;
    HandResult best_result;
    int found = 0;

    best_card.rank = ANTEATER;
    best_card.suit = ANTEATER_SUIT;
    best_card.type = ANTEATER_CARD;
    clear_result(&best_result);

    for (Suit suit = HEARTS; suit <= SPADES; suit++) {
        for (Rank rank = TWO; rank <= ACE; rank++) {

            Card test_card;
            test_card.rank = rank;
            test_card.suit = suit;
            test_card.type = NORMAL_CARD;

            Card temp_hand[2];
            temp_hand[0] = player_cards[0];
            temp_hand[1] = player_cards[1];

            for (int i = 0; i < 2; i++) {
                if (temp_hand[i].type == ANTEATER_CARD) {
                    temp_hand[i] = test_card;
                    break;
                }
            }

            HandResult current = evaluate_player_cards(temp_hand, board_cards);

            if (!found || compare_results(&current, &best_result) > 0) {
                best_result = current;
                best_card = test_card;
                found = 1;
            }
        }
    }

    return best_card;
}


int eval_hand(Card player_cards[2])
{
    int first = player_cards[0].type == ANTEATER_CARD ? ACE : player_cards[0].rank;
    int second = player_cards[1].type == ANTEATER_CARD ? ACE : player_cards[1].rank;

    if (first < second) {
        int temp = first;
        first = second;
        second = temp;
    }

    return first * RANK_LIMIT + second;
}

int eval_points(Card player_cards[2], Card board_cards[5])
{
    HandResult result = evaluate_player_cards(player_cards, board_cards);
    return result.rank;
}

int compare_hands(Card p1_hand[2], Card p2_hand[2], Card board[5])
{
    HandResult p1_result = evaluate_player_cards(p1_hand, board);
    HandResult p2_result = evaluate_player_cards(p2_hand, board);
    int comparison = compare_results(&p1_result, &p2_result);

    if (comparison > 0) {
        return 1;
    }
    if (comparison < 0) {
        return 2;
    }

    return 0; // split the pot
}
