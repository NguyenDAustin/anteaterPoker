#include "bot.h"
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

PlayerAction getBotPreFlopAction(Player_Info *bot, GameState *game)
{
    int score = evalPreFlop(bot->playerCards);
    int callAmount = game->currentBet - bot->currentBet;

    if (callAmount <= 0)
    {
        if (score >= 9)
            return (PlayerAction) {RAISE, 2 * game->bigBlind};
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

PlayerAction getBotPostFlopAction(Player_Info *bot, GameState *game)
{
    return (PlayerAction){CHECK, 0};
}

PlayerAction getBotAction(Player_Info *bot, GameState *game)
{
    if (game->round == ROUND_PRE_FLOP)
        return getBotPreFlopAction(bot, game);

    else
        return getBotPostFlopAction(bot, game);
}
