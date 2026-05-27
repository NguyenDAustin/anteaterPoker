#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include "deck.h"
#include "card.h"
#include "player.h"

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

typedef struct GameState {
	Player_Info players[MAX_PLAYERS_COUNT];
	int numPlayers;
    int dealerIndex;
    Deck deck;
    Board board;
    int pot;
    int currentPlayerIndex;
    int turnNumber;
    int smallBlind;
    int bigBlind;
    int currentBet; // current highest bet for the round
    Round round;
} GameState;

void initGameState(GameState *game);
void resetGameState(GameState *game);
void copyGameState(GameState *destination, const GameState *source);
int getCurrentPlayerIndex(const GameState *game);
bool setCurrentPlayerIndex(GameState *game, int playerIndex);
int getDealerIndex(const GameState *game);
bool setDealerIndex(GameState *game, int dealerIndex);
int getTurnNumber(const GameState *game);
void setTurnNumber(GameState *game, int turnNumber);
Round getRound(const GameState *game);
void setRound(GameState *game, Round round);
Card getPlayerHoleCard(const GameState *game, int playerIndex, int cardSlot);
bool setPlayerHoleCard(GameState *game, int playerIndex, int cardSlot, Card card);
Card getDealerCard(const GameState *game, int dealerCardIndex);
bool setDealerCard(GameState *game, int dealerCardIndex, Card card);
void saveGameState(const GameState *game, const char *filename);
void loadGameState(GameState *game, const char *filename);
int nextActivePlayerIndex(const GameState *game);
bool advanceToNextPlayer(GameState *game);
void advanceToNextRound(GameState *game);

Card empty_card(void);


#endif // STATE_H