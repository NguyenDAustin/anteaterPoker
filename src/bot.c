#include "bot.h"
#include "hand.h"
#include <stdlib.h>

void initBot(Player_Info *bot, const char *name, int seat, int chips)
{
    initPlayer(bot, name, seat, chips, BOT_PLAYER);
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
    if (game == NULL || playerIndex < 0 || playerIndex >= game->numPlayers)
        return 0;

    Player_Info *bot = game->players[playerIndex];

    if (bot == NULL)
        return 0;

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
    if (game == NULL || playerIndex < 0 || playerIndex >= game->numPlayers)
        return 0;

    Player_Info *bot = game->players[playerIndex];

    if (bot == NULL)
        return 0;

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
    if (game == NULL || playerIndex < 0 || playerIndex >= game->numPlayers)
        return 0;

    Player_Info *bot = game->players[playerIndex];

    if (bot == NULL || bot->chips <= 0)
        return 0;

    int raiseAmount = game->pot / 2;

    if (raiseAmount < game->bigBlind)
        raiseAmount = game->bigBlind;

    if (raiseAmount > bot->chips)
        raiseAmount = bot->chips;

    return raiseAmount;
}

int calculateCallCost(GameState *game, int playerIndex)
{
    if (game == NULL || playerIndex < 0 || playerIndex >= game->numPlayers)
        return 0;

    Player_Info *bot = game->players[playerIndex];

    if (bot == NULL)
        return 0;

    int callCost = game->currentBet - bot->currentBet;

    if (callCost < 0)
        return 0;

    if (callCost > bot->chips)
        return bot->chips;

    return callCost;
}

double calculateCallPrice(GameState *game, int playerIndex)
{
    if (game == NULL)
        return 0.0;

    int callCost = calculateCallCost(game, playerIndex);

    if (callCost <= 0)
        return 0.0;

    return (double)callCost / (game->pot + callCost);
}

PlayerAction getBotPreFlopAction(GameState *game, int playerIndex)
{
    Player_Info *bot = game->players[playerIndex];
    int score = evalPreFlop(bot->playerCards);
    int callAmount = game->currentBet - bot->currentBet;

    if (callAmount <= 0)
    {
        if (score >= 8)
            return (PlayerAction) {RAISE, 0.05 * bot->chips};
        else
            return (PlayerAction) {CHECK, 0};
    }

    if (score >= 10)
    {
        return (PlayerAction) {RAISE, callAmount};
    }
    else if (score >= 7)
    {
        return (PlayerAction) {CALL, callAmount};
    }
    else if (score >= 5 && callAmount <= game->bigBlind)
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
    int strength = evalPostFlop(game, playerIndex);
    int callCost = calculateCallCost(game, playerIndex);

    if (callCost == 0) {
        if (strength >= 4) {
            return (PlayerAction){RAISE, calculateBotRaiseAmount(game, playerIndex)};
        }

        return (PlayerAction){CHECK, 0};
    }

    if (strength >= 5) {
        return (PlayerAction){RAISE, calculateBotRaiseAmount(game, playerIndex)};
    }

    if (strength >= 2) {
        return (PlayerAction){CALL, callCost};
    }

    if (strength >= 1 && callCost <= game->bigBlind) {
        return (PlayerAction){CALL, callCost};
    }

    return (PlayerAction){FOLD, 0};
}

PlayerAction getBotAction(GameState *game, int playerIndex)
{
    if (game->round == ROUND_PRE_FLOP)
        return getBotPreFlopAction(game, playerIndex);

    else
        return getBotPostFlopAction(game, playerIndex);
}
