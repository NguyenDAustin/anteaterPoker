#include "poker_protocol.h"

#include <stdio.h>
#include <string.h>

const char* pokerActionTypeToString(PokerActionType type)
{
    switch (type) {
        case POKER_ACTION_FOLD:
            return "FOLD";
        case POKER_ACTION_CHECK:
            return "CHECK";
        case POKER_ACTION_CALL:
            return "CALL";
        case POKER_ACTION_BET:
            return "BET";
        case POKER_ACTION_RAISE:
            return "RAISE";
        default:
            return "UNKNOWN";
    }
}

static PokerActionType pokerActionTypeFromString(const char* actionName)
{
    if (strcmp(actionName, "FOLD") == 0) {
        return POKER_ACTION_FOLD;
    }

    if (strcmp(actionName, "CHECK") == 0) {
        return POKER_ACTION_CHECK;
    }

    if (strcmp(actionName, "CALL") == 0) {
        return POKER_ACTION_CALL;
    }

    if (strcmp(actionName, "BET") == 0) {
        return POKER_ACTION_BET;
    }

    if (strcmp(actionName, "RAISE") == 0) {
        return POKER_ACTION_RAISE;
    }

    return POKER_ACTION_UNKNOWN;
}

bool parsePokerActionMessage(const char* message, PokerActionMessage* action)
{
    char actionName[16];
    int amount = 0;
    int parsedItems;

    if (!message || !action) {
        return false;
    }

    parsedItems = sscanf(message, "%15s %d", actionName, &amount);

    if (parsedItems < 1) {
        return false;
    }

    action->type = pokerActionTypeFromString(actionName);
    action->amount = amount;

    return action->type != POKER_ACTION_UNKNOWN;
}

bool formatPokerActionMessage(char* buffer, size_t bufferSize, PokerActionType type, int amount)
{
    int charsWritten;

    if (!buffer || bufferSize == 0 || type == POKER_ACTION_UNKNOWN) {
        return false;
    }

    charsWritten = snprintf(buffer, bufferSize, "%s %d\n", pokerActionTypeToString(type), amount);

    return charsWritten > 0 && (size_t)charsWritten < bufferSize;
}

// ------------------------ FULL GAME STATE PROTOCOL ------------------------

static Cardtype cardTypeForSuit(int suit)
{
    return (suit == ANTEATER_SUIT) ? ANTEATER_CARD : NORMAL_CARD;
}

bool formatFullGameState(char* buffer, size_t bufferSize, const GameState* gameState)
{
    if (!buffer || bufferSize == 0 || !gameState) {
        return false;
    }

    int offset = 0;
    int written;

    written = snprintf(buffer + offset, bufferSize - offset,
                       "Turn = %d | Pot = %d | Board = ",
                       gameState->currentPlayerIndex + 1, gameState->pot);
    if (written < 0 || (size_t)written >= bufferSize - offset) return false;
    offset += written;

    for (int i = 0; i < gameState->board.count; i++) {
        written = snprintf(buffer + offset, bufferSize - offset, "%s%d:%d",
                           (i == 0) ? "" : ",",
                           gameState->board.cards[i].rank,
                           (int)gameState->board.cards[i].suit);
        if (written < 0 || (size_t)written >= bufferSize - offset) return false;
        offset += written;
    }

    for (int i = 0; i < gameState->numPlayers; i++) {
        const PlayerState* p = &gameState->players[i];
        written = snprintf(buffer + offset, bufferSize - offset,
                           " | Player %d Chips = %d | Player %d Folded = %d | Player %d Cards = %d:%d,%d:%d",
                           i + 1, p->chips,
                           i + 1, p->hasFolded ? 1 : 0,
                           i + 1,
                           p->hole_cards[0].rank, (int)p->hole_cards[0].suit,
                           p->hole_cards[1].rank, (int)p->hole_cards[1].suit);
        if (written < 0 || (size_t)written >= bufferSize - offset) return false;
        offset += written;
    }

    return true;
}

void applyTurn(const char* message, GameState* gameState)
{
    if (!message || !gameState) return;

    const char* p = strstr(message, "Turn = ");
    if (!p) return;

    int value;
    if (sscanf(p, "Turn = %d", &value) == 1) {
        gameState->currentPlayerIndex = value - 1;
    }
}

void applyPot(const char* message, GameState* gameState)
{
    if (!message || !gameState) return;

    const char* p = strstr(message, "Pot = ");
    if (!p) return;

    int value;
    if (sscanf(p, "Pot = %d", &value) == 1) {
        gameState->pot = value;
    }
}

void applyDealerCards(const char* message, GameState* gameState)
{
    if (!message || !gameState) return;

    const char* p = strstr(message, "Board = ");
    if (!p) return;
    p += strlen("Board = ");

    int idx = 0;
    int rank, suit, consumed;
    while (idx < MAX_DEALER_CARDS && sscanf(p, "%d:%d%n", &rank, &suit, &consumed) == 2) {
        gameState->board.cards[idx].rank = rank;
        gameState->board.cards[idx].suit = (Suit)suit;
        gameState->board.cards[idx].type = cardTypeForSuit(suit);
        idx++;
        p += consumed;
        if (*p == ',') {
            p++;
        } else {
            break;
        }
    }

    gameState->board.count = idx;
}

void applyPlayerChips(const char* message, GameState* gameState, int playerNum)
{
    if (!message || !gameState || playerNum < 1 || playerNum > MAX_PLAYERS_COUNT) return;

    char key[32];
    snprintf(key, sizeof(key), "Player %d Chips = ", playerNum);
    const char* p = strstr(message, key);
    if (!p) return;

    int value;
    char fmt[32];
    snprintf(fmt, sizeof(fmt), "Player %d Chips = %%d", playerNum);
    if (sscanf(p, fmt, &value) == 1) {
        gameState->players[playerNum - 1].chips = value;
    }
}

void applyPlayerFolded(const char* message, GameState* gameState, int playerNum)
{
    if (!message || !gameState || playerNum < 1 || playerNum > MAX_PLAYERS_COUNT) return;

    char key[32];
    snprintf(key, sizeof(key), "Player %d Folded = ", playerNum);
    const char* p = strstr(message, key);
    if (!p) return;

    int value;
    char fmt[32];
    snprintf(fmt, sizeof(fmt), "Player %d Folded = %%d", playerNum);
    if (sscanf(p, fmt, &value) == 1) {
        gameState->players[playerNum - 1].hasFolded = (value != 0);
    }
}

void applyPlayerCards(const char* message, GameState* gameState, int playerNum)
{
    if (!message || !gameState || playerNum < 1 || playerNum > MAX_PLAYERS_COUNT) return;

    char key[32];
    snprintf(key, sizeof(key), "Player %d Cards = ", playerNum);
    const char* p = strstr(message, key);
    if (!p) return;
    p += strlen(key);

    int r1, s1, r2, s2;
    if (sscanf(p, "%d:%d,%d:%d", &r1, &s1, &r2, &s2) == 4) {
        PlayerState* player = &gameState->players[playerNum - 1];
        player->hole_cards[0].rank = r1;
        player->hole_cards[0].suit = (Suit)s1;
        player->hole_cards[0].type = cardTypeForSuit(s1);
        player->hole_cards[1].rank = r2;
        player->hole_cards[1].suit = (Suit)s2;
        player->hole_cards[1].type = cardTypeForSuit(s2);
    }
}

void parseFullGameState(const char* message, GameState* gameState)
{
    if (!message || !gameState) return;

    applyTurn(message, gameState);
    applyPot(message, gameState);
    applyDealerCards(message, gameState);

    for (int i = 1; i <= MAX_PLAYERS_COUNT; i++) {
        applyPlayerChips(message, gameState, i);
        applyPlayerFolded(message, gameState, i);
        applyPlayerCards(message, gameState, i);
    }
}
