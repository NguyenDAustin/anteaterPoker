#include <stdio.h>
#include <string.h>
#include "state_protocol.h"
#include "state.h"
#include "player.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    GameState src;
    initGameState(&src);

    initPlayer(src.players[0], "Alice", 0, 1000, HUMAN_PLAYER);
    initPlayer(src.players[1], "Bob",   1, 750,  HUMAN_PLAYER);
    src.numPlayers = 2;
    src.pot = 250;
    src.currentBet = 50;
    src.round = ROUND_FLOP;
    src.dealerIndex = 0;
    src.currentPlayerIndex = 1;
    src.board.count = 3;
    src.board.cards[0] = cardCtor(HEARTS, ACE);
    src.board.cards[1] = cardCtor(SPADES, KING);
    src.board.cards[2] = cardCtor(CLUBS, QUEEN);
    src.players[0]->playerCards[0] = cardCtor(HEARTS, JACK);
    src.players[0]->playerCards[1] = cardCtor(DIAMONDS, TEN);

    char buffer[GAME_STATE_MESSAGE_SIZE];
    CHECK(formatGameStateMessage(buffer, sizeof(buffer), &src),
          "formatGameStateMessage failed");

    /* parse into a fresh GameState */
    GameState dst;
    initGameState(&dst);
    initPlayer(dst.players[0], "", 0, 0, HUMAN_PLAYER);
    initPlayer(dst.players[1], "", 1, 0, HUMAN_PLAYER);
    dst.numPlayers = 2;

    CHECK(parseGameStateMessage(buffer, &dst), "parseGameStateMessage failed");

    CHECK(dst.pot == 250, "pot did not survive round-trip");
    CHECK(dst.currentBet == 50, "currentBet did not survive round-trip");
    CHECK(dst.numPlayers == 2, "numPlayers did not survive round-trip");
    CHECK(dst.round == ROUND_FLOP, "round did not survive round-trip");
    CHECK(dst.board.count == 3, "board count did not survive round-trip");
    CHECK(dst.board.cards[0].rank == ACE, "board card 0 rank mismatch");
    CHECK(strcmp(dst.players[0]->name, "Alice") == 0, "player name did not survive");
    CHECK(dst.players[1]->chips == 750, "player chips did not survive");

    src.round = ROUND_PRE_FLOP;
    src.board.count = 0;
    for (int i = 0; i < MAX_DEALER_CARDS; i++) {
        src.board.cards[i] = empty_card();
    }

    CHECK(formatGameStateMessage(buffer, sizeof(buffer), &src),
          "formatGameStateMessage failed for empty board");
    CHECK(parseGameStateMessage(buffer, &dst),
          "parseGameStateMessage failed for empty board");
    CHECK(dst.board.count == 0, "empty board count did not parse");
    CHECK(dst.board.cards[0].rank == -1, "old board card should be cleared");

    /* parsing garbage should fail */
    CHECK(!parseGameStateMessage("not a real message", &dst),
          "garbage should fail to parse");

    printf("state_protocol test passed\n");
    return 0;
}
