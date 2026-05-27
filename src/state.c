#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"

// Function to create an empty card
Card empty_card() {
    Card card;
    card.rank = -1; 
    card.suit = HEARTS; 
    card.type = NORMAL_CARD; 
    return card;
}

//check if a player is active and has not folded
static bool isGamePlayerActive(const GameState *game, int playerIndex)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers) {
        return false;
    }

    const Player_Info *player = &game->players[playerIndex];
    return player->isActive && !player->hasFolded;
}

// Find the next active player index
int nextActivePlayerIndex(const GameState *game)
{
    if (!game || game->numPlayers == 0) {
        return -1;
    }

    int idx = game->currentPlayerIndex;
    for (int i = 1; i <= game->numPlayers; i++) {
        int next = (idx + i) % game->numPlayers;
        if (isGamePlayerActive(game, next)) {
            return next;
        }
    }

    return -1;
}
 
// Advance to the next active player
bool advanceToNextPlayer(GameState *game)
{
    int nextIndex = nextActivePlayerIndex(game);
    setCurrentPlayerIndex(game, nextIndex);
    game->turnNumber++;
    return true;
}

// Advance to the next round when players expose cards
void advanceToNextRound(GameState *game)
{
    if (!game) {
        return;
    }

    if (game->round < ROUND_SHOWDOWN) {
        game->round = (Round)(game->round + 1);
    }

    game->currentBet = 0;
    game->turnNumber = 0;
    game->currentPlayerIndex = nextActivePlayerIndex(game);
    if (game->currentPlayerIndex < 0) {
        game->currentPlayerIndex = 0;
    }
}

//get player whose turn it is
int getCurrentPlayerIndex(const GameState *game)
{
    return game ? game->currentPlayerIndex : -1;
}

//set player whose turn it is
bool setCurrentPlayerIndex(GameState *game, int playerIndex)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers) {
        return false;
    }

    game->currentPlayerIndex = playerIndex;
    return true;
}

// Get the dealer index
int getDealerIndex(const GameState *game)
{
    return game ? game->dealerIndex : -1;
}

// Set the dealer index
bool setDealerIndex(GameState *game, int dealerIndex)
{
    if (!game || dealerIndex < 0 || dealerIndex >= game->numPlayers) {
        return false;
    }

    game->dealerIndex = dealerIndex;
    return true;
}

// Get the turn number of the game
int getTurnNumber(const GameState *game)
{
    return game ? game->turnNumber : -1;
}

// Set the turn number of the game
void setTurnNumber(GameState *game, int turnNumber)
{
    if (!game || turnNumber < 0) {
        return;
    }

    game->turnNumber = turnNumber;
}

// Get the current round of the game
Round getRound(const GameState *game)
{
    return game ? game->round : ROUND_PRE_FLOP;
}

// Set the current round of the game
void setRound(GameState *game, Round round)
{
    if (!game) {
        return;
    }

    if (round >= ROUND_PRE_FLOP && round <= ROUND_SHOWDOWN) {
        game->round = round;
    }
}

// Get a player's hole card
Card getPlayerHoleCard(const GameState *game, int playerIndex, int cardSlot)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers || cardSlot < 0 || cardSlot >= 2) {
        return empty_card();
    }

    return game->players[playerIndex].hole_cards[cardSlot];
}

// Set a player's hole card
bool setPlayerHoleCard(GameState *game, int playerIndex, int cardSlot, Card card)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers || cardSlot < 0 || cardSlot >= 2) {
        return false;
    }

    game->players[playerIndex].hole_cards[cardSlot] = card;
    return true;
}

// Get a dealer card
Card getDealerCard(const GameState *game, int dealerCardIndex)
{
    if (!game || dealerCardIndex < 0 || dealerCardIndex >= MAX_DEALER_CARDS) {
        return empty_card();
    }

    return game->board.cards[dealerCardIndex];
}

// Set a dealer card
bool setDealerCard(GameState *game, int dealerCardIndex, Card card)
{
    if (!game || dealerCardIndex < 0 || dealerCardIndex >= MAX_DEALER_CARDS) {
        return false;
    }

    game->board.cards[dealerCardIndex] = card;
    if (dealerCardIndex >= game->board.count) {
        game->board.count = dealerCardIndex + 1;
    }

    return true;
}

// Initialize the game state
void initGameState(GameState *game)
{
    if (!game) {
        return;
    }

    memset(game, 0, sizeof(GameState));
    init_deck(&game->deck);
    shuffle(&game->deck);

    game->numPlayers = 0;
    game->dealerIndex = 0;
    game->turnNumber = 0;
    game->pot = 0;
    game->currentPlayerIndex = 0;
    game->smallBlind = 1;
    game->bigBlind = 2;
    game->currentBet = 0;
    game->round = ROUND_PRE_FLOP;
    game->board.count = 0;
    for (int i = 0; i < MAX_DEALER_CARDS; i++) {
        game->board.cards[i] = empty_card();
    }

    for (int i = 0; i < MAX_PLAYERS_COUNT; i++) {
        game->players[i].seat = i;
        game->players[i].chips = 0;
        game->players[i].betSize = 0;
        game->players[i].currentBet = 0;
        game->players[i].hasFolded = false;
        game->players[i].isActive = false;
        game->players[i].hole_cards[0] = empty_card(); 
        game->players[i].hole_cards[1] = empty_card();
    }
}

// Reset the game state for a new round
void resetGameState(GameState *game)
{
    if (!game) {
        return;
    }

    for (int i = 0; i < game->numPlayers; i++) {
        Player_Info *player = &game->players[i];
        player->isActive = true;
        player->currentBet = 0;
        player->betSize = 0;
        player->hole_cards[0] = empty_card();
        player->hole_cards[1] = empty_card();
    }

    game->board.count = 0;
    for (int i = 0; i < MAX_DEALER_CARDS; i++) {
        game->board.cards[i] = empty_card();
    }

    game->pot = 0;
    game->currentBet = 0;
    game->round = ROUND_PRE_FLOP;
    game->turnNumber = 0;
    init_deck(&game->deck);
    shuffle(&game->deck);
    game->currentPlayerIndex = nextActivePlayerIndex(game);
    if (game->currentPlayerIndex < 0) {
        game->currentPlayerIndex = 0;
    }
}

// Copy the game state from source to destination
void copyGameState(GameState *destination, const GameState *source)
{
    if (!destination || !source) {
        return;
    }

    memcpy(destination, source, sizeof(GameState));
}

// Save the game state to a file
void saveGameState(const GameState *game, const char *filename)
{
    if (!game || !filename) {
        return;
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        return;
    }

    fwrite(game, sizeof(GameState), 1, file);
    fclose(file);
}

// Load the game state from a file
void loadGameState(GameState *game, const char *filename)
{
    if (!game || !filename) {
        return;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        return;
    }

    fread(game, sizeof(GameState), 1, file);
    fclose(file);
}
