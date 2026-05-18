#include "bot.h"

void initBot(Bot *bot, const char *name, int seat, int chips, BotDifficulty difficulty)
{
    initPlayer(&bot->player, name, seat, chips, BOT_PLAYER);
    bot->difficulty = difficulty;
}

int getChenValue(Card card) {
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
        return 12;          //change later

    return 0;
}

int evalPreFlop(Card hand[2]) {
    int score = 0;

    Rank r1 = hand[0].rank;
    Rank r2 = hand[1].rank;

    int hasAnteater = r1 == ANTEATER || r2 == ANTEATER;

    //highest value
    score += (getChenValue(hand[0]) > getChenValue(hand[1])) ? getChenValue(hand[0]) : getChenValue(hand[1]);

    //pocket pair
    if(r1 == r2) {
        score *= 2;

        if(score < 5)
            score = 5;
    }

    //suited
    if(hand[0].suit == hand[1].suit) {
        score += 2;
    }

    //gap
    if(hasAnteater) {
        score += 3;
    }
    else {
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
PlayerAction getBotAction(Player *bot)
{
    if(score <= 3) 
        return { FOLD, 0 };
    else    
        return { CHECK, 0 };
    
}
