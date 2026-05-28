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

    const Player_Info *player = game->players[playerIndex];
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


Player_Info** getPlayersInfo(const GameState* game){ 
     if(!game){
        printf("ERROR: could not get all players info b/c game is NULL\n"); 
        return NULL; 
    }
    return game->players; 
}

Player_Info* getPlayerInfo(const GameState* game, int playerIndex){ 
     if(!game){
        printf("ERROR: could not get  %d players info b/c game is NULL\n", playerIndex); 
        return NULL; 
    }
    return game->players[playerIndex]; 
}

int getJoinedPlayers(const GameState* game){
    if(!game){
        printf("ERROR: could not get number of joined players b/c game is NULL\n"); 
        return -1; 
    }
    return game->numPlayers; 
}

void setPlayersInfo(GameState* game, Player_Info** playersInfo){
    if(!game){
        printf("ERROR: could not set all players info b/c game is NULL\n"); 
        return;
    }
    game->players = playersInfo; 
}

void setPlayerInfo(GameState* game, Player_Info* playerInfo, int playerIndex){ 
    if(!game){
        printf("ERROR: could not set %d players info b/c game is NULL\n", playerIndex); 
        return;
    }
    game->players[playerIndex] = playerInfo; 
}

void setJoinedPlayers(GameState* game, int joinedPlayers){ 
    if(!game){ 
        printf("ERROR: could not set number of joined players b /c game is NULL\n"); 
        return; 
    }

    game->numPlayers = joinedPlayers; 
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
Card getPlayerCard(const GameState *game, int playerIndex, int cardSlot)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers || cardSlot < 0 || cardSlot >= 2) {
        return empty_card();
    }

    return game->players[playerIndex]->playerCards[cardSlot];
}

Card* getPlayerCards(const GameState* game, int playerIndex){ 
    if(!game || playerIndex < 0 || playerIndex >= game->numPlayers){ 
        printf("ERROR: could not get player %d cards b/c game is null or invalid index\n", playerIndex); 
        return NULL; 
    }
    return game->players[playerIndex]->playerCards; 
}

// Set a player's hole card
bool setPlayerCard(GameState *game, int playerIndex, int cardSlot, Card card)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers || cardSlot < 0 || cardSlot >= 2) {
        printf("ERROR: could not set player card b/c index is invalid or gamestate is NULL\n"); 
        return false;
    }

    game->players[playerIndex]->playerCards[cardSlot] = card;
    return true;
}

void setPlayerCards(GameState* game, int playerIndex, Card* playerCards){
    Player_Info* player = getPlayerInfo(game, playerIndex); 

    if(!game || !player){
        printf("ERROR: could not get %d player cards b/c player or game is NULL\n", playerIndex); 
        return; 
    }
    setCards(player, playerCards); 
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
        printf("ERROR could not set dealer card b/c invalid index or game is NULL\n");
        return false;
    }

    game->board.cards[dealerCardIndex] = card;
    if (dealerCardIndex >= game->board.count) {
        game->board.count = dealerCardIndex + 1;
    }

    return true;
}

Card* getDealerCards(const GameState* game){  
    if(!game){
        printf("ERROR could not get dealer cards b/c gamestate is NULL\n"); 
        return NULL; 
    }
    return game->board.cards; 
}


bool setDealerCards(GameState* game, Card* dealerCards){ 
    if(!game){ 
        printf("ERROR could not set dealer cards b/c gamestate is NULL\n");
        return false; 
    }

    for(int i = 0; i < MAX_DEALER_CARDS; i++){ 
        setDealerCard(game, i, dealerCards[i]); 
    }
    return true; 
}

void setPlayerChipCount(GameState* game, int playerIndex, int chipCount){ 
    if(!game){ 
        printf("ERROR could not set %d player chip count b/c gamestate is NULL\n", playerIndex);
        return; 
    }
    Player_Info* player = getPlayerInfo(game, playerIndex); 

    if(!player){ 
        printf("ERROR could not set %d player chip count b/c player is NULL\n", playerIndex);
        return; 
    }

    setChipCount(player, chipCount); 
}

void setPot(GameState* game, int potAmt){
    if(!game){ 
        printf("ERROR could not set pot b/c gamestate is NULL\n");
        return; 
    }
    game->pot = potAmt; 
}

int getPlayerChipCount(const GameState* game, int playerIndex){ 
    if(!game){ 
        printf("ERROR could not get chip count b/c gamestate is NULL\n");
        return -1; 
    }

    Player_Info* player = getPlayerInfo(game, playerIndex); 
    return getChipCount(player); 
}

int getPot(const GameState* game){
    if(!game){ 
        printf("ERROR could not get pot b/c gamestate is NULL\n");
        return -1; 
    }
    return game->pot; 
}

//to properly allocate player info 
void allocatePlayerInfos(Player_Info **playerInfo)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        playerInfo[i] = malloc(sizeof(Player_Info));
    }
}

// Initialize the game state
void initGameState(GameState *game)
{
    printf("initializing game state!\n"); 
    if (!game) {
        return;
    }

    game->deck = malloc(sizeof(Deck)); //alloc memory
    game->players = malloc(sizeof(Player_Info) * MAX_PLAYERS); 
    allocatePlayerInfos(game->players); 

    //memset(game, 0, sizeof(GameState)); --> ok i allocate memory outside of function (in lobby) - queency

    printf("trying to create deck/shuffle\n"); 
    init_deck(game->deck);
    shuffle(game->deck);
    printf("created deck/shuffle\n"); 

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

    printf("finished initializing basic game state stuff\n"); 

    for (int i = 0; i < MAX_DEALER_CARDS; i++) {
        game->board.cards[i] = empty_card();
    }

    printf("finished initializing board\n"); 

    for (int i = 0; i < MAX_PLAYERS_COUNT; i++) {
        game->players[i]->seat = i;
        game->players[i]->chips = 0;
        game->players[i]->betSize = 0;
        game->players[i]->currentBet = 0;
        game->players[i]->hasFolded = false;
        game->players[i]->isActive = false;
        game->players[i]->playerCards = NULL; 

        /*
        game->players[i]->playerCards[0] = empty_card(); 
        game->players[i]->playerCards[1] = empty_card();
        */
    }

     printf("finished initializing game state!\n"); 
}

// Reset the game state for a new round
void resetGameState(GameState *game)
{ 

    if (!game) {
        return;
    }


    for (int i = 0; i < game->numPlayers; i++) {
        Player_Info *player = game->players[i];
        player->isActive = true;
        player->currentBet = 0;
        player->betSize = 0;
        player->playerCards[0] = empty_card();
        player->playerCards[1] = empty_card();
    }

    game->board.count = 0;
    for (int i = 0; i < MAX_DEALER_CARDS; i++) {
        game->board.cards[i] = empty_card();
    }

    game->pot = 0;
    game->currentBet = 0;
    game->round = ROUND_PRE_FLOP;
    game->turnNumber = 0;
    init_deck(game->deck);
    shuffle(game->deck);
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
