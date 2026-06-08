#include <stdio.h>
#include "game.h"
#include "state.h"
#include "player.h"

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
    GameState game;
    initGameState(&game);

    initPlayer(game.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(game.players[1], "Bob",   1, 1000, HUMAN_PLAYER);
    initPlayer(game.players[2], "Carol", 2, 1000, HUMAN_PLAYER);
    game.numPlayers = 3;
    game.dealerIndex = 0;

    startNewRound(&game);
    CHECK(game.round == ROUND_PRE_FLOP, "startNewRound should set round to PRE_FLOP");
    /* pot should be non-zero after blinds */
    CHECK(game.pot > 0, "blinds should have been posted into the pot");

    /* current player should have hole cards dealt */
    CHECK(game.players[0]->playerCards[0].rank >= 0, "hole cards should be dealt");

    /* phase complete check shouldn't crash */
    (void)isBettingPhaseComplete(&game);
    (void)shouldEndRound(&game);

    /* advanceGameRound should move forward */
    Round before = game.round;
    advanceGameRound(&game);
    CHECK(game.round != before || game.round == ROUND_SHOWDOWN,
          "advanceGameRound should change round or land on SHOWDOWN");

    /* try a fold action via handlePlayerAction */
    PlayerAction fold = { .actionType = FOLD, .amount = 0 };
    GameActionResult res = handlePlayerAction(&game, game.currentPlayerIndex, fold);
    CHECK(res == GAME_ACTION_SUCCESS || res == GAME_ACTION_ROUND_COMPLETE,
          "fold should succeed or end the round");

    GameState bustedGame;
    initGameState(&bustedGame);

    initPlayer(bustedGame.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(bustedGame.players[1], "Bob",   1,    0, HUMAN_PLAYER);
    initPlayer(bustedGame.players[2], "Carol", 2, 1000, HUMAN_PLAYER);
    bustedGame.numPlayers = 3;
    bustedGame.dealerIndex = 0;

    startNewRound(&bustedGame);
    CHECK(bustedGame.players[1]->hasFolded, "busted player should be out of the next hand");
    CHECK(!bustedGame.players[1]->canAct, "busted player should not be able to act");
    CHECK(bustedGame.players[1]->currentBet == 0, "busted player's bet should reset");
    CHECK(bustedGame.players[1]->playerCards[0].rank < 0,
          "busted player should not receive hole cards");
    CHECK(bustedGame.dealerIndex == 2, "dealer rotation should skip busted players");

    GameState allInGame;
    initGameState(&allInGame);
    initPlayer(allInGame.players[0], "Alice", 0, 0, HUMAN_PLAYER);
    initPlayer(allInGame.players[1], "Bob",   1, 0, BOT_PLAYER);
    allInGame.numPlayers = 2;
    allInGame.round = ROUND_PRE_FLOP;
    allInGame.pot = 200;
    allInGame.players[0]->canAct = false;
    allInGame.players[1]->canAct = false;
    allInGame.players[0]->playerCards[0] = normal_card(HEARTS, ACE);
    allInGame.players[0]->playerCards[1] = normal_card(SPADES, ACE);
    allInGame.players[1]->playerCards[0] = normal_card(HEARTS, KING);
    allInGame.players[1]->playerCards[1] = normal_card(SPADES, KING);

    advanceGameRound(&allInGame);
    CHECK(allInGame.round == ROUND_SHOWDOWN, "all-in hand should auto-advance to showdown");
    CHECK(allInGame.pot == 0, "all-in showdown should award the pot");

    GameState winnerGame;
    initGameState(&winnerGame);
    initPlayer(winnerGame.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(winnerGame.players[1], "Bob",   1, 1000, HUMAN_PLAYER);
    winnerGame.numPlayers = 2;
    winnerGame.board.count = 5;
    winnerGame.board.cards[0] = normal_card(CLUBS, TWO);
    winnerGame.board.cards[1] = normal_card(DIAMONDS, FIVE);
    winnerGame.board.cards[2] = normal_card(HEARTS, SEVEN);
    winnerGame.board.cards[3] = normal_card(CLUBS, NINE);
    winnerGame.board.cards[4] = normal_card(DIAMONDS, JACK);
    winnerGame.players[0]->playerCards[0] = normal_card(HEARTS, ACE);
    winnerGame.players[0]->playerCards[1] = normal_card(SPADES, ACE);
    winnerGame.players[1]->playerCards[0] = normal_card(HEARTS, KING);
    winnerGame.players[1]->playerCards[1] = normal_card(SPADES, QUEEN);
    CHECK(determineWinner(&winnerGame) == 0,
          "pair of aces should beat king high");

    winnerGame.pot = 75;
    awardPotToWinner(&winnerGame, 0);
    CHECK(winnerGame.pot == 0, "awardPotToWinner should clear the pot");
    CHECK(winnerGame.players[0]->chips == 1075,
          "awardPotToWinner should add pot to winner");

    GameState splitGame;
    initGameState(&splitGame);
    initPlayer(splitGame.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(splitGame.players[1], "Bob",   1, 1000, HUMAN_PLAYER);
    splitGame.numPlayers = 2;
    splitGame.pot = 101;
    splitGame.board.count = 5;
    splitGame.board.cards[0] = normal_card(HEARTS, TEN);
    splitGame.board.cards[1] = normal_card(HEARTS, JACK);
    splitGame.board.cards[2] = normal_card(HEARTS, QUEEN);
    splitGame.board.cards[3] = normal_card(HEARTS, KING);
    splitGame.board.cards[4] = normal_card(HEARTS, ACE);
    splitGame.players[0]->playerCards[0] = normal_card(CLUBS, TWO);
    splitGame.players[0]->playerCards[1] = normal_card(DIAMONDS, THREE);
    splitGame.players[1]->playerCards[0] = normal_card(CLUBS, FOUR);
    splitGame.players[1]->playerCards[1] = normal_card(DIAMONDS, FIVE);

    awardPotToWinners(&splitGame);
    CHECK(splitGame.pot == 0, "awardPotToWinners should clear split pot");
    CHECK(splitGame.players[0]->chips == 1051, "first tied player should get odd chip");
    CHECK(splitGame.players[1]->chips == 1050, "second tied player should get split pot");

    printf("game test passed\n");
    return 0;
}
