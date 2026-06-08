#include "bot.h"
#include "hand.h"
#include "monte_carlo.h"
#include <stdlib.h>

#define BOT_MONTE_CARLO_SIMULATIONS 500

typedef struct
{
    int preflopOpenRaiseScore;
    int preflopRaiseScore;
    int preflopCallScore;
    int preflopCheapCallScore;
    double postflopRaiseEquity;
    double postflopBetEquity;
    double callMargin;
    double raisePotFraction;
    double openRaiseChipFraction;
    int bluffChance;
} BotStrategy;

static BotStrategy getBotStrategy(BotType type)
{
    switch (type)
    {
        case BLUFFER:
            return (BotStrategy){6, 9, 5, 4, 0.65, 0.50, -0.02, 0.75, 0.07, 20};

        case AGGRESSIVE:
            return (BotStrategy){7, 9, 6, 4, 0.64, 0.54, 0.02, 0.75, 0.07, 8};

        case CONSERVATIVE:
            return (BotStrategy){10, 12, 8, 6, 0.80, 0.72, 0.15, 0.40, 0.04, 0};

        case BALANCED:
        case NONE:
        default:
            return (BotStrategy){8, 10, 7, 5, 0.70, 0.62, 0.08, 0.50, 0.05, 0};
    }
}

static bool shouldBluff(BotStrategy strategy)
{
    return strategy.bluffChance > 0 && (rand() % 100) < strategy.bluffChance;
}

static bool isValidBotTarget(GameState *game, int playerIndex)
{
    return game != NULL &&
           game->players != NULL &&
           game->numPlayers >= 0 &&
           game->numPlayers <= MAX_PLAYERS_COUNT &&
           playerIndex >= 0 &&
           playerIndex < game->numPlayers &&
           game->players[playerIndex] != NULL;
}

void initBot(Player_Info *bot, const char *name, int seat, int chips)
{
    initPlayer(bot, name, seat, chips, BOT_PLAYER);
    setBotType(bot, BALANCED);
}

int getChenValue(Card card)
{
    Rank rank = card.rank;

    if (rank >= TWO && rank <= TEN)
        return rank / 2;

    if (rank == JACK)
        return 6;

    if (rank == QUEEN)
        return 7;

    if (rank == KING)
        return 8;

    if (rank == ACE)
        return 10;

    if (rank == ANTEATER)
        return 12; // change later

    return 0;
}

int evalPreFlop(Card hand[2])
{
    if (hand == NULL)
        return 0;

    int score = 0;

    Rank r1 = hand[0].rank;
    Rank r2 = hand[1].rank;

    int hasAnteater = r1 == ANTEATER || r2 == ANTEATER;

    // highest value
    score += (getChenValue(hand[0]) > getChenValue(hand[1])) ? getChenValue(hand[0]) : getChenValue(hand[1]);

    // pocket pair
    if (r1 == r2)
    {
        score *= 2;

        if (score < 5)
            score = 5;
    }

    // suited
    if (hand[0].suit == hand[1].suit)
    {
        score += 2;
    }

    // gap
    if (hasAnteater)
    {
        score += 3;
    }
    else
    {
        int gap = abs(r1 - r2) - 1;

        if (gap == 1)
            score -= 1;
        else if (gap == 2)
            score -= 2;
        else if (gap == 3)
            score -= 4;
        else if (gap >= 4)
            score -= 5;

        if ((gap == 0 || gap == 1) && r1 < QUEEN && r2 < QUEEN)
            score += 1;
    }

    return score;
}

int calculateDrawBonus(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return 0;

    Player_Info *bot = game->players[playerIndex];

    Card cards[TOTAL_HAND_CARDS];
    int count = 0;
    int bonus = 0;

    cards[count++] = bot->playerCards[0];
    cards[count++] = bot->playerCards[1];

    for (int i = 0; i < game->board.count && i < BOARD_SIZE; i++)
        cards[count++] = game->board.cards[i];

    for (Suit suit = HEARTS; suit <= SPADES; suit++)
    {
        if (count_suit(cards, count, suit) >= 4)
        {
            bonus += 1;
            break;
        }
    }

    int hasRank[ACE + 1] = {0};

    for (int i = 0; i < count; i++)
    {
        if (cards[i].type == NORMAL_CARD && cards[i].rank >= TWO && cards[i].rank <= ACE)
            hasRank[cards[i].rank] = 1;
    }

    if (hasRank[ACE])
        hasRank[1] = 1;

    for (int start = 1; start <= TEN; start++)
    {
        int ranksInWindow = 0;

        for (int rank = start; rank < start + 5; rank++)
            ranksInWindow += hasRank[rank];

        if (ranksInWindow >= 4)
        {
            bonus += 1;
            break;
        }
    }

    return bonus;
}

int evalPostFlop(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return 0;

    Player_Info *bot = game->players[playerIndex];

    Card cards[TOTAL_HAND_CARDS];
    int count = 0;

    cards[count++] = bot->playerCards[0];
    cards[count++] = bot->playerCards[1];

    for (int i = 0; i < game->board.count && i < BOARD_SIZE; i++)
        cards[count++] = game->board.cards[i];

    sort(cards, count);

    int strength = 0;

    if (royalFlush(cards, count))
        strength = 9;
    else if (straightFlush(cards, count))
        strength = 8;
    else if (four(cards, count))
        strength = 7;
    else if (fullHouse(cards, count))
        strength = 6;
    else if (flush(cards, count))
        strength = 5;
    else if (straight(cards, count))
        strength = 4;
    else if (three(cards, count))
        strength = 3;
    else if (twoPair(cards, count))
        strength = 2;
    else if (pairs(cards, count))
        strength = 1;

    return strength + calculateDrawBonus(game, playerIndex);
}

int calculateBotRaiseAmount(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return 0;

    Player_Info *bot = game->players[playerIndex];

    if (bot->chips <= 0)
        return 0;

    int callCost = calculateCallCost(game, playerIndex);
    int availableRaiseChips = bot->chips - callCost;
    if (availableRaiseChips <= 0)
        return 0;

    BotStrategy strategy = getBotStrategy(bot->botType);
    int raiseAmount = (int)((game->pot + callCost) * strategy.raisePotFraction);

    if (callCost > 0 && raiseAmount < callCost)
        raiseAmount = callCost;

    if (raiseAmount < game->bigBlind)
        raiseAmount = game->bigBlind;

    if (raiseAmount > availableRaiseChips)
        raiseAmount = availableRaiseChips;

    return raiseAmount;
}

int calculateCallCost(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return 0;

    Player_Info *bot = game->players[playerIndex];

    int callCost = game->currentBet - bot->currentBet;

    if (callCost < 0)
        return 0;

    if (callCost > bot->chips)
        return bot->chips;

    return callCost;
}

double calculateCallPrice(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return 0.0;

    int callCost = calculateCallCost(game, playerIndex);

    if (callCost <= 0)
        return 0.0;

    if (game->pot + callCost <= 0)
        return 0.0;

    return (double)callCost / (game->pot + callCost);
}

PlayerAction getBotPreFlopAction(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return (PlayerAction){FOLD, 0};

    Player_Info *bot = game->players[playerIndex];
    BotStrategy strategy = getBotStrategy(bot->botType);
    int score = evalPreFlop(bot->playerCards);
    int callAmount = calculateCallCost(game, playerIndex);
    bool canRaiseAgain = bot->raisesThisRound == 0;

    if (callAmount <= 0)
    {
        if (canRaiseAgain && (score >= strategy.preflopOpenRaiseScore || shouldBluff(strategy))) {
            int raiseAmount = (int)(bot->chips * strategy.openRaiseChipFraction);
            if (raiseAmount < game->bigBlind)
                raiseAmount = game->bigBlind;
            if (raiseAmount > bot->chips)
                raiseAmount = bot->chips;

            if (raiseAmount > 0)
                return (PlayerAction) {RAISE, raiseAmount};
        }

        return (PlayerAction) {CHECK, 0};
    }

    if (score >= strategy.preflopRaiseScore)
    {
        if (canRaiseAgain) {
            int raiseAmount = calculateBotRaiseAmount(game, playerIndex);
            if (raiseAmount > 0)
                return (PlayerAction) {RAISE, raiseAmount};
        }

        return (PlayerAction) {CALL, callAmount};
    }
    else if (score >= strategy.preflopCallScore)
    {
        return (PlayerAction) {CALL, callAmount};
    }
    else if (score >= strategy.preflopCheapCallScore && callAmount <= game->bigBlind)
    {
        return (PlayerAction) {CALL, callAmount};
    }
    else
    {
        return (PlayerAction) {FOLD, 0};
    }
}

PlayerAction getBotPostFlopAction(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return (PlayerAction){FOLD, 0};

    Player_Info *bot = game->players[playerIndex];
    BotStrategy strategy = getBotStrategy(bot->botType);
    int callCost = calculateCallCost(game, playerIndex);
    bool canRaiseAgain = bot->raisesThisRound == 0;
    double equity = estimateBotEquity(game, playerIndex, BOT_MONTE_CARLO_SIMULATIONS);
    double callPrice = calculateCallPrice(game, playerIndex);

    if (callCost == 0) {
        if (canRaiseAgain && (equity >= strategy.postflopBetEquity || shouldBluff(strategy))) {
            int raiseAmount = calculateBotRaiseAmount(game, playerIndex);
            if (raiseAmount > 0)
                return (PlayerAction){RAISE, raiseAmount};
        }

        return (PlayerAction){CHECK, 0};
    }

    if (equity >= strategy.postflopRaiseEquity && canRaiseAgain) {
        int raiseAmount = calculateBotRaiseAmount(game, playerIndex);
        if (raiseAmount > 0)
            return (PlayerAction){RAISE, raiseAmount};
    }

    if (equity >= callPrice + strategy.callMargin)
        return (PlayerAction){CALL, callCost};

    return (PlayerAction){FOLD, 0};
}

PlayerAction getBotAction(GameState *game, int playerIndex)
{
    if (!isValidBotTarget(game, playerIndex))
        return (PlayerAction){FOLD, 0};

    if (game->round == ROUND_PRE_FLOP)
        return getBotPreFlopAction(game, playerIndex);

    else
        return getBotPostFlopAction(game, playerIndex);
}
