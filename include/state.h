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
	//Player_Info players[MAX_PLAYERS_COUNT]; 
    Player_Info** players; //changing to double pointer

	int numPlayers;
    int dealerIndex;
    Deck* deck; //change to pointer - queency
    Board board; //what the hell is board ? --> apparently dealer cards

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

//GETTERS - GAME STATE
Player_Info** getPlayersInfo(const GameState* game); //gets all player info 
Player_Info* getPlayerInfo(const GameState* game, int playerIndex);  //gets a certain player's info
int getJoinedPlayers(const GameState* game); 
int getCurrentPlayerIndex(const GameState *game);
int getDealerIndex(const GameState *game);
int getTurnNumber(const GameState *game);
Round getRound(const GameState *game);
Card getPlayerCard(const GameState *game, int playerIndex, int cardSlot);
Card* getPlayerCards(const GameState* game, int playerIndex); 
Card getDealerCard(const GameState *game, int dealerCardIndex);
Card* getDealerCards(const GameState* game);
int getPlayerChipCount(const GameState* game, int playerIndex); 
int getPot(const GameState* game); 

//SETTERS - GAME STATE 
void setPlayersInfo(GameState* game, Player_Info** playersInfo); //sets all players' info 
void setPlayerInfo(GameState* game, Player_Info* playerInfo, int playerIndex); //sets one player's info
void setJoinedPlayers(GameState* game, int joinedPlayers); 
bool setCurrentPlayerIndex(GameState *game, int playerIndex);
bool setDealerIndex(GameState *game, int dealerIndex);
void setTurnNumber(GameState *game, int turnNumber);
void setRound(GameState *game, Round round);
bool setPlayerCard(GameState *game, int playerIndex, int cardSlot, Card card); 
void setPlayerCards(GameState* game, int playerIndex, Card* playerCards);
bool setDealerCard(GameState *game, int dealerCardIndex, Card card);
bool setDealerCards(GameState* game, Card* dealerCards);
void setPlayerChipCount(GameState* game, int playerIndex, int chipCount); 
void setPot(GameState* game, int potAmt);

//GENERAL FUNCTIONS
void saveGameState(const GameState *game, const char *filename);
void loadGameState(GameState *game, const char *filename);
int nextActivePlayerIndex(const GameState *game);
bool advanceToNextPlayer(GameState *game);
void advanceToNextRound(GameState *game);

Card empty_card(void);


#endif // STATE_H