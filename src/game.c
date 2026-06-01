#include "game.h"

#include <stddef.h>

#include "deck.h"
#include "hand.h"

static bool isValidPlayerIndex(const GameState *game, int playerIndex)
{
    return game && playerIndex >= 0 && playerIndex < game->numPlayers;
}

static bool isPlayerInHand(const GameState *game, int playerIndex)
{
    if (!isValidPlayerIndex(game, playerIndex)) {
        return false;
    }

    const Player_Info *player = game->players[playerIndex];
    return !player->hasFolded && player->chips >= 0;
}

static bool canGamePlayerAct(const GameState *game, int playerIndex)
{
    // LOBBY_WIRING: renamed to avoid colliding with player.h's canPlayerAct.
    const Player_Info *player = game->players[playerIndex];
    return isPlayerInHand(game, playerIndex) && player->canAct && player->chips > 0;
}

static int countPlayersInHand(const GameState *game)
{
    if (!game) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < game->numPlayers; i++) {
        if (isPlayerInHand(game, i)) {
            count++;
        }
    }

    return count;
}

static int nextPlayerFrom(const GameState *game, int startIndex)
{
    if (!game || game->numPlayers <= 0) {
        return -1;
    }

    for (int i = 1; i <= game->numPlayers; i++) {
        int index = (startIndex + i) % game->numPlayers;
        if (isPlayerInHand(game, index)) {
            return index;
        }
    }

    return -1;
}

static int nextActingPlayerFrom(const GameState *game, int startIndex)
{
    if (!game || game->numPlayers <= 0) {
        return -1;
    }

    for (int i = 1; i <= game->numPlayers; i++) {
        int index = (startIndex + i) % game->numPlayers;
        if (canGamePlayerAct(game, index)) {
            return index;
        }
    }

    return -1;
}

static int payChips(GameState *game, Player_Info *player, int amount)
{
    printf("paying chips\n"); 
    if (!game || !player || amount <= 0) {
        return 0;
    }

    int paid = amount;
    if (paid > player->chips) {
        paid = player->chips;
    }

    player->chips -= paid;
    player->currentBet += paid;
    player->betSize += paid;
    game->pot += paid;

    if(player->chips == 0){
        player->canAct = false;
    }

    printf("paid chips - player chip count %d\n", player->chips); 
    return paid;
}

static void clearBets(GameState *game)
{
    if (!game) {
        return;
    }

    game->currentBet = 0;
    game->turnNumber = 0;

    for (int i = 0; i < game->numPlayers; i++) {
        game->players[i]->currentBet = 0;
        game->players[i]->betSize = 0;
        game->players[i]->raisesThisRound = 0;
    }
}

static void postBlind(GameState *game, int playerIndex, int amount)
{
    if (!isValidPlayerIndex(game, playerIndex) || amount <= 0) {
        return;
    }

    Player_Info *player = game->players[playerIndex];
    int paid = payChips(game, player, amount);
    if (paid > game->currentBet) {
        game->currentBet = paid;
    }
}

/* Ok i just removed this - queency 
static void setPlayerCards(Player_Info *player, Card first, Card second)
{
    player->playerCards[0] = first;
    player->playerCards[1] = second;
   
    player->hand[0] = first;
    player->hand[1] = second;
    player->playerCards = player->hand;
}
*/

static void dealHoleCardsToActivePlayers(GameState *game)
{
    for (int card = 0; card < 2; card++) {
        for (int i = 0; i < game->numPlayers; i++) {
            Player_Info *player = game->players[i];
            if (player->canAct && player->chips > 0) {
                Card dealt = deal(game->deck);
                player->playerCards[card] = dealt;
            }
        }
    }
}

static void dealBoardCard(GameState *game)
{
    if (!game || game->board.count >= MAX_DEALER_CARDS) {
        return;
    }

    setDealerCard(game, game->board.count, deal(game->deck));
}

void startNewRound(GameState *game)
{
    printf("starting new round\n");
    
    if (!game || game->numPlayers <= 0) {
        printf("ERROR: game state is null\n");
        return;
    }


    int previousDealer = getDealerIndex(game); 
    int numPlayers = getJoinedPlayers(game); 

    printf("finished setting dealer index etc\n"); 

    resetGameState(game);

    for (int i = 0; i < numPlayers; i++) {
        Player_Info *player = getPlayerInfo(game, i); 
        player->hasFolded = false;
        player->canAct = player->chips > 0;
        player->playerCards[0] = empty_card(); 
        player->playerCards[1] = empty_card(); 
    }

    if (countPlayersInHand(game) < 2) {
        game->currentPlayerIndex = game->dealerIndex;
        return;
    }

     printf("finished count players in hand\n");

    game->dealerIndex = nextPlayerFrom(game, previousDealer);
    if (game->dealerIndex < 0) {
        game->dealerIndex = 0;
    }

    printf("finished doing something to dealer index\n");


    dealHoleCardsToActivePlayers(game);

    printf("finished dealing cards to all players\n");

    int smallBlindIndex = nextPlayerFrom(game, game->dealerIndex);
    int bigBlindIndex = nextPlayerFrom(game, smallBlindIndex);

    if (countPlayersInHand(game) == 2) {
        smallBlindIndex = game->dealerIndex;
        bigBlindIndex = nextPlayerFrom(game, game->dealerIndex);
    }

    postBlind(game, smallBlindIndex, game->smallBlind);
    postBlind(game, bigBlindIndex, game->bigBlind);

    game->round = ROUND_PRE_FLOP;
    game->currentPlayerIndex = nextActingPlayerFrom(game, bigBlindIndex);
    if (game->currentPlayerIndex < 0) {
        game->currentPlayerIndex = game->dealerIndex;
    }
    printf("finished starting new round\n");
}

void dealCommunityCards(GameState *game)
{
    if (!game) {
        return;
    }

    switch (game->round) {
    case ROUND_FLOP:
        while (game->board.count < 3) {
            dealBoardCard(game);
        }
        break;
    case ROUND_TURN:
        while (game->board.count < 4) {
            dealBoardCard(game);
        }
        break;
    case ROUND_RIVER:
    case ROUND_SHOWDOWN:
        while (game->board.count < 5) {
            dealBoardCard(game);
        }
        break;
    case ROUND_PRE_FLOP:
    default:
        break;
    }
}

GameActionResult handlePlayerAction(GameState *game, int playerIndex, PlayerAction action)
{
    if (!game) {
        return GAME_ACTION_INVALID_GAME;
    }
    if (!isPlayerInHand(game, playerIndex)) {
        return GAME_ACTION_INVALID_PLAYER;
    }
    if (playerIndex != game->currentPlayerIndex) {
        return GAME_ACTION_NOT_PLAYERS_TURN;
    }

    Player_Info *player = getPlayerInfo(game, playerIndex);

    switch (action.actionType) {
    case FOLD:
        player->hasFolded = true;
        player->canAct = false;
        game->turnNumber++;
        break;
    case CHECK:
        if (player->currentBet != game->currentBet) {
            return GAME_ACTION_CANNOT_CHECK;
        }
        game->turnNumber++;
        break;
    case CALL:
        payChips(game, player, game->currentBet - player->currentBet);
        game->turnNumber++;
        break;
    case BET:
        if (game->currentBet != 0 || action.amount <= 0) {
            return GAME_ACTION_INVALID_AMOUNT;
        }
        if (action.amount > player->chips) {
            return GAME_ACTION_NOT_ENOUGH_CHIPS;
        }
        payChips(game, player, action.amount);
        game->currentBet = player->currentBet;
        player->raisesThisRound++;
        game->turnNumber = 1;
        break;
    case RAISE:
    {
        if (action.amount <= 0) {
            return GAME_ACTION_INVALID_AMOUNT;
        }

        int callAmount = game->currentBet - player->currentBet;
        int totalPayment = callAmount + action.amount;
        if (totalPayment > player->chips) {
            return GAME_ACTION_NOT_ENOUGH_CHIPS;
        }

        payChips(game, player, totalPayment);
        game->currentBet = player->currentBet;
        player->raisesThisRound++;
        game->turnNumber = 1;
        break;
    }
    default:
        return GAME_ACTION_INVALID_AMOUNT;
    }

    if (shouldEndRound(game)) {
        int winner = determineWinner(game);
        awardPotToWinner(game, winner);
        game->round = ROUND_SHOWDOWN;
        return GAME_ACTION_ROUND_COMPLETE;
    }

    if (isBettingPhaseComplete(game)) {
        printf("we are advanding round!\n");
        advanceGameRound(game);
        return GAME_ACTION_ROUND_COMPLETE;
    }

    game->currentPlayerIndex = nextActingPlayerFrom(game, playerIndex);
    return GAME_ACTION_SUCCESS;
}

bool isBettingPhaseComplete(const GameState *game)
{
    if (!game || countPlayersInHand(game) <= 1) {
        return true;
    }

    int playersWhoCanAct = 0;
    for (int i = 0; i < game->numPlayers; i++) {
        const Player_Info *player = game->players[i];
        if (!isPlayerInHand(game, i) || player->chips == 0) {
            continue;
        }

        playersWhoCanAct++;
        if (player->currentBet != game->currentBet) {
            return false;
        }
    }

    return game->turnNumber >= playersWhoCanAct;
}

bool shouldEndRound(const GameState *game)
{
    if (!game) {
        return true;
    }

    return countPlayersInHand(game) <= 1 || game->round == ROUND_SHOWDOWN;
}

void advanceGameRound(GameState *game)
{
    if (!game) {
        return;
    }

    if (game->round == ROUND_RIVER) {
        int winner = determineWinner(game);
        awardPotToWinner(game, winner);
        game->round = ROUND_SHOWDOWN;
        return;
    }

    if (game->round < ROUND_SHOWDOWN) {
        game->round = (Round)(game->round + 1);
    }

    clearBets(game);
    dealCommunityCards(game);

    game->currentPlayerIndex = nextActingPlayerFrom(game, game->dealerIndex);
    if (game->currentPlayerIndex < 0) {
        game->currentPlayerIndex = game->dealerIndex;
    }
}

int determineWinner(const GameState *game)
{
    if (!game) {
        return -1;
    }

    int winner = -1;
    int bestPoints = -1;
    int bestTieValue = -1;

    for (int i = 0; i < game->numPlayers; i++) {
        const Player_Info *player = game->players[i];
        if (!isPlayerInHand(game, i)) {
            continue;
        }

        if (winner == -1) {
            winner = i;
        }

        if (game->board.count < MAX_DEALER_CARDS) {
            continue;
        }

        Card handCards[2] = { player->playerCards[0], player->playerCards[1] };
        Card boardCards[MAX_DEALER_CARDS];
        for (int card = 0; card < MAX_DEALER_CARDS; card++) {
            boardCards[card] = game->board.cards[card];
        }

        int points = eval_points(handCards, boardCards);
        int tieValue = eval_hand(handCards);

        if (points > bestPoints || (points == bestPoints && tieValue > bestTieValue)) {
            bestPoints = points;
            bestTieValue = tieValue;
            winner = i;
        }
    }

    return winner;
}

void awardPotToWinner(GameState *game, int winnerIndex)
{
    if (!isValidPlayerIndex(game, winnerIndex) || game->pot <= 0) {
        return;
    }

    game->players[winnerIndex]->chips += game->pot;
    game->pot = 0;
}
