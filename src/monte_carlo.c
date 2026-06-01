#include "monte_carlo.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "deck.h"
#include "hand.h"

#define SIM_WIN 2
#define SIM_TIE 1
#define SIM_LOSS 0

static bool cardEquals(Card a, Card b)
{
    return a.rank == b.rank && a.suit == b.suit && a.type == b.type;
}

static void seedRandomOnce(void)
{
    static bool seeded = false;

    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }
}

static int collectKnownCards(GameState *game, int playerIndex, Card knownCards[])
{
    int knownCount = 0;
    Player_Info *bot = game->players[playerIndex];

    knownCards[knownCount++] = bot->playerCards[0];
    knownCards[knownCount++] = bot->playerCards[1];

    for (int i = 0; i < game->board.count && i < BOARD_SIZE; i++) {
        knownCards[knownCount++] = game->board.cards[i];
    }

    return knownCount;
}

static int buildAvailableDeck(GameState *game, int playerIndex, Card availableDeck[])
{
    Deck deck;
    Card knownCards[HAND_SIZE + BOARD_SIZE];
    bool usedKnown[HAND_SIZE + BOARD_SIZE] = {false};
    int knownCount = collectKnownCards(game, playerIndex, knownCards);
    int availableCount = 0;

    init_deck(&deck);

    for (int i = 0; i < DECK_SIZE; i++) {
        bool isKnown = false;

        for (int j = 0; j < knownCount; j++) {
            if (!usedKnown[j] && cardEquals(deck.cards[i], knownCards[j])) {
                usedKnown[j] = true;
                isKnown = true;
                break;
            }
        }

        if (!isKnown) {
            availableDeck[availableCount++] = deck.cards[i];
        }
    }

    return availableCount;
}

static void shuffleCards(Card cards[], int count)
{
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

static int compareSimulatedHands(Card heroHand[HAND_SIZE], Card villainHand[HAND_SIZE], Card board[BOARD_SIZE])
{
    int heroPoints = eval_points(heroHand, board);
    int villainPoints = eval_points(villainHand, board);

    if (heroPoints > villainPoints)
        return 1;

    if (heroPoints < villainPoints)
        return -1;

    int heroTieValue = eval_hand(heroHand);
    int villainTieValue = eval_hand(villainHand);

    if (heroTieValue > villainTieValue)
        return 1;

    if (heroTieValue < villainTieValue)
        return -1;

    return 0;
}

static int countSimulatedOpponents(GameState *game, int playerIndex)
{
    int opponents = 0;

    for (int i = 0; i < game->numPlayers; i++) {
        Player_Info *player = game->players[i];

        if (i != playerIndex && player && !player->hasFolded) {
            opponents++;
        }
    }

    return opponents;
}

static int simulateOneHand(GameState *game, int playerIndex, Card availableDeck[], int availableCount)
{
    Card simulationDeck[DECK_SIZE];
    Card heroHand[HAND_SIZE];
    Card board[BOARD_SIZE];
    int deckIndex = 0;
    bool tiedBestHand = false;

    for (int i = 0; i < availableCount; i++) {
        simulationDeck[i] = availableDeck[i];
    }

    shuffleCards(simulationDeck, availableCount);

    heroHand[0] = game->players[playerIndex]->playerCards[0];
    heroHand[1] = game->players[playerIndex]->playerCards[1];

    for (int i = 0; i < game->board.count && i < BOARD_SIZE; i++) {
        board[i] = game->board.cards[i];
    }

    for (int i = game->board.count; i < BOARD_SIZE; i++) {
        board[i] = simulationDeck[deckIndex++];
    }

    for (int i = 0; i < game->numPlayers; i++) {
        Player_Info *villain = game->players[i];
        Card villainHand[HAND_SIZE];
        int result;

        if (i == playerIndex || !villain || villain->hasFolded) {
            continue;
        }

        villainHand[0] = simulationDeck[deckIndex++];
        villainHand[1] = simulationDeck[deckIndex++];

        result = compareSimulatedHands(heroHand, villainHand, board);

        if (result < 0)
            return SIM_LOSS;

        if (result == 0)
            tiedBestHand = true;
    }

    return tiedBestHand ? SIM_TIE : SIM_WIN;
}

double estimateBotEquity(GameState *game, int playerIndex, int simulations)
{
    Card availableDeck[DECK_SIZE];
    int availableCount;
    int opponents;
    double score = 0.0;

    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers || simulations <= 0) {
        return 0.0;
    }

    if (!game->players[playerIndex] || game->players[playerIndex]->hasFolded) {
        return 0.0;
    }

    opponents = countSimulatedOpponents(game, playerIndex);
    if (opponents == 0) {
        return 1.0;
    }

    availableCount = buildAvailableDeck(game, playerIndex, availableDeck);
    if (availableCount < (BOARD_SIZE - game->board.count) + opponents * HAND_SIZE) {
        return 0.0;
    }

    seedRandomOnce();

    for (int i = 0; i < simulations; i++) {
        int result = simulateOneHand(game, playerIndex, availableDeck, availableCount);

        if (result == SIM_WIN)
            score += 1.0;
        else if (result == SIM_TIE)
            score += 0.5;
    }

    return score / simulations;
}
