#include "rules.h"

bool isValidCheck(GameState *game, Player *player){
    if (!player->isActive) {
        return false; 
    }
    return true;
}

bool isValidCall(GameState *game, Player *player){
    if (!player->isActive) {
        return false; 
    }
    else if (player->currentBet >= game->currentBet) {
        return false; 
    }
    else if (player->chips < (game->currentBet - player->currentBet)) {
        return false; 
    }
    return true;
}

bool isValidRaise(GameState *game, Player *player, int amount){
    if (!player->isActive) {
        return false; // Player has already folded
    } else if (amount <= 0) {
        return false; // Raise amount must be positive
    } else if (amount > player->chips) {
        return false; // Player does not have enough chips to raise
    }
    return true;
}

bool isValidFold(GameState *game, Player *player){
    if (!player->isActive) {
        return false; // Player has already folded
    }

    return true;
}

bool isValidAnteaterAction(GameState *game, Player *player, Hand *hand){
    if (!player->isActive) {
        return false; // Player has already folded
    }
    else if (player->playerCards[0].rank != 0 && player->playerCards[1].rank != 0) { // if no anteater cards are present, the action is invalid
        return false;
    }
    return true;
}


void applyCheck(GameState *game, Player *player){
    if (!isValidCheck(game, player)) {
        return; // Invalid check action
    }
    else if (player->currentBet < game->currentBet) {
        return; // Player cannot check if they have not matched the current bet
    }
    else {
        game->currentPlayerIndex = (game->currentPlayerIndex + 1) % game->numPlayers;
    }
}

void applyCall(GameState *game, Player *player){
    if (!isValidCall(game, player)) {
        return; // Invalid call action
    }
    else {
        int callAmount = game->currentBet - player->currentBet;
        if (callAmount > player->chips) {
            callAmount = player->chips; // Player can only call with available chips
        }
        player->chips -= callAmount;
        player->currentBet += callAmount;
        game->pot += callAmount;

    }
}

void applyRaise(GameState *game, Player *player, int amount){
    if (!isValidRaise(game, player, amount)) {
        return; // Invalid raise action
    }
    else {
        int totalBet = game->currentBet + amount;
        if (totalBet > player->chips) {
            totalBet = player->chips; // Player can only raise with available chips
        }
        player->chips -= totalBet;
        player->currentBet += totalBet;
        game->pot += totalBet;
        game->currentBet = totalBet;

    }
}

void applyFold(GameState *game, Player *player){
    if (!isValidFold(game, player)) {
        return; // Invalid fold action
    }
    else {
        player->isActive = false; // Player folds and is no longer active in the hand

    }
}
// need to complete
void applyAnteaterAction(GameState *game, Player *player, Hand *hand){
    if (!isValidAnteaterAction(game, player, hand)) {
        return; // Invalid anteater action
    }
    else {
        if (player->playerCards[0].rank == 0) {
            player->playerCards[0].rank = 0; 

        } else if (player->playerCards[1].rank == 0) {
            player->playerCards[1].rank = 0; 
        }

        
    }
}
