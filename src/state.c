#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"

Card empty_card() {
    Card card;
    card.rank = -1; // Use -1 to indicate an empty card
    card.suit = HEARTS; // Default suit (can be any value)
    card.type = NORMAL_CARD; // Default type
    return card;
}

static bool isPlayerEligible(const GameState *game, int playerIndex)
{
    if (!game || playerIndex < 0 || playerIndex >= game->numPlayers) {
        return false;
    }

    const PlayerState *player = &game->players[playerIndex];
    return player->isActive && !player->hasFolded;
}

int nextActivePlayerIndex(const GameState *game, int startIndex)
{
    if (!game || game->numPlayers == 0) {
        return -1;
    }

    int idx = startIndex;
    for (int i = 1; i <= game->numPlayers; i++) {
        int candidate = (idx + i) % game->numPlayers;
        if (isPlayerEligible(game, candidate)) {
            return candidate;
        }
    }

    return -1;
}

bool advanceToNextPlayer(GameState *game)
{
    if (!game || game->numPlayers == 0) {
        return false;
    }

    int nextIndex = nextActivePlayerIndex(game, game->currentPlayerIndex);
    if (nextIndex < 0) {
        return false;
    }

    game->currentPlayerIndex = nextIndex;
    return true;
}

void advanceToNextRound(GameState *game)
{
    if (!game) {
        return;
    }

    if (game->round < ROUND_SHOWDOWN) {
        game->round = (Round)(game->round + 1);
    }

    game->currentBet = 0;
    game->currentPlayerIndex = nextActivePlayerIndex(game, game->dealerIndex);
    if (game->currentPlayerIndex < 0) {
        game->currentPlayerIndex = 0;
    }
}

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
    game->pot = 0;
    game->currentPlayerIndex = 0;
    game->smallBlind = 1;
    game->bigBlind = 2;
    game->currentBet = 0;
    game->round = ROUND_PRE_FLOP;

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

void resetGameState(GameState *game)
{
    if (!game) {
        return;
    }

    for (int i = 0; i < game->numPlayers; i++) {
        PlayerState *player = &game->players[i];
        player->hasFolded = false;
        player->currentBet = 0;
        player->betSize = 0;
        player->hole_cards[0] = empty_card();
        player->hole_cards[1] = empty_card();
    }

    game->board.count = 0;
    game->pot = 0;
    game->currentBet = 0;
    game->round = ROUND_PRE_FLOP;
    init_deck(&game->deck);
    shuffle(&game->deck);
    game->currentPlayerIndex = nextActivePlayerIndex(game, game->dealerIndex);
    if (game->currentPlayerIndex < 0) {
        game->currentPlayerIndex = 0;
    }
}

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
