#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include "deck.h"

#define MAX_PLAYERS_COUNT 6
#define MAX_DEALER_CARDS 5

typedef enum {
    ROUND_PRE_FLOP,
    ROUND_FLOP,
    ROUND_TURN,
    ROUND_RIVER,
    ROUND_SHOWDOWN
} Round;

typedef struct {
    Card cards[MAX_DEALER_CARDS];
    int count;
} Board;

typedef struct PlayerState {
	char name[32];
	int seat;
	int chips;
	Card hole_cards[2];
	int betSize;           // static bet size or last bet placed
	int currentBet;        // amount the player has put into the pot this betting round
	bool hasFolded;
	bool isActive;
} PlayerState;


typedef struct GameState {
	PlayerState players[MAX_PLAYERS_COUNT];
	int numPlayers;
    int dealerIndex;
    Deck deck;
    Board board;
    int pot;
    int currentPlayerIndex;
    int smallBlind;
    int bigBlind;
    int currentBet; // current highest bet for the round
    Round round;
} GameState;

void initGameState(GameState *game);
void resetGameState(GameState *game);
void copyGameState(GameState *destination, const GameState *source);
void saveGameState(const GameState *game, const char *filename);
void loadGameState(GameState *game, const char *filename);
int nextActivePlayerIndex(const GameState *game, int startIndex);
bool advanceToNextPlayer(GameState *game);
void advanceToNextRound(GameState *game);
Card empty_card(void);


#endif // STATE_H