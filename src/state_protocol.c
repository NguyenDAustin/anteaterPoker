#include "state_protocol.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static int appendFormatted(char* buffer, size_t bufferSize, int offset, const char* format, ...)
{
    if (offset < 0 || (size_t)offset >= bufferSize) {
        return -1;
    }

    va_list args;
    va_start(args, format);
    int written = vsnprintf(buffer + offset, bufferSize - offset, format, args);
    va_end(args);

    if (written < 0 || (size_t)written >= bufferSize - offset) {
        return -1;
    }

    return offset + written;
}

bool formatGameStateMessage(char* buffer, size_t bufferSize, const GameState* game)
{
    if (!buffer || bufferSize == 0 || !game) {
        return false;
    }

    int offset = 0;

    offset = appendFormatted(buffer, bufferSize, offset,
                             "STATE\n"
                             "pot %d\n"
                             "round %d\n"
                             "currentPlayer %d\n"
                             "dealer %d\n"
                             "currentBet %d\n"
                             "smallBlind %d\n"
                             "bigBlind %d\n"
                             "numPlayers %d\n"
                             "boardCount %d\n",
                             game->pot, (int)game->round, game->currentPlayerIndex,
                             game->dealerIndex, game->currentBet,
                             game->smallBlind, game->bigBlind,
                             game->numPlayers, game->board.count);
    if (offset < 0) return false;

    offset = appendFormatted(buffer, bufferSize, offset, "board");
    if (offset < 0) return false;

    for (int i = 0; i < game->board.count; i++) {
        offset = appendFormatted(buffer, bufferSize, offset, " %d %d",
                                 game->board.cards[i].rank,
                                 (int)game->board.cards[i].suit);
        if (offset < 0) return false;
    }

    offset = appendFormatted(buffer, bufferSize, offset, "\n");
    if (offset < 0) return false;

    for (int i = 0; i < game->numPlayers; i++) {
        const Player_Info* p = game->players[i]; //change player state to player_info - queency
        offset = appendFormatted(buffer, bufferSize, offset,
                                 "player %d %s %d %d %d %d %d %d %d %d\n",
                                 i, p->name, p->chips, p->currentBet,
                                 p->hasFolded ? 1 : 0, p->canAct ? 1 : 0,
                                 p->playerCards[0].rank, (int)p->playerCards[0].suit,
                                 p->playerCards[1].rank, (int)p->playerCards[1].suit);
        if (offset < 0) return false;
    }

    offset = appendFormatted(buffer, bufferSize, offset, "END\n");
    return offset >= 0;
}

static void writeCard(Card* card, int rank, int suit)
{
    card->rank = rank;
    card->suit = (Suit)suit;
    card->type = (suit == ANTEATER_SUIT) ? ANTEATER_CARD : NORMAL_CARD;
}

bool parseGameStateMessage(const char* message, GameState* game)
{

    if (!message || !game) {
        return false;
    }

    char buffer[GAME_STATE_MESSAGE_SIZE];
    strncpy(buffer, message, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* savePtr = NULL;
    char* line = strtok_r(buffer, "\n", &savePtr);

    if (!line || strcmp(line, "STATE") != 0) {
        return false;
    }

    bool sawEnd = false;
    int value;

    game->board.count = 0;
    for (int i = 0; i < MAX_DEALER_CARDS; i++) {
        game->board.cards[i] = empty_card();
    }

    while ((line = strtok_r(NULL, "\n", &savePtr)) != NULL) {
        if (strcmp(line, "END") == 0) {
            sawEnd = true;
            break;
        }

        if (sscanf(line, "pot %d", &value) == 1) {
            game->pot = value;
        } else if (sscanf(line, "round %d", &value) == 1) {
            game->round = (Round)value;
        } else if (sscanf(line, "currentPlayer %d", &value) == 1) {
            game->currentPlayerIndex = value;
        } else if (sscanf(line, "dealer %d", &value) == 1) {
            game->dealerIndex = value;
        } else if (sscanf(line, "currentBet %d", &value) == 1) {
            game->currentBet = value;
        } else if (sscanf(line, "smallBlind %d", &value) == 1) {
            game->smallBlind = value;
        } else if (sscanf(line, "bigBlind %d", &value) == 1) {
            game->bigBlind = value;
        } else if (sscanf(line, "numPlayers %d", &value) == 1) {
            game->numPlayers = value;
        } else if (sscanf(line, "boardCount %d", &value) == 1) {
            if (value >= 0 && value <= MAX_DEALER_CARDS) {
                game->board.count = value;
            }
        }
        else if (sscanf(line, "nextRoundPlayers %d", &value) == 1) {
            game->nextRoundPlayers = value;
        } else if (strncmp(line, "board", 5) == 0 && (line[5] == ' ' || line[5] == '\0')) {
            const char* cursor = line + 5;
            int idx = 0;
            int rank, suit, consumed;
            while (idx < MAX_DEALER_CARDS &&
                   sscanf(cursor, " %d %d%n", &rank, &suit, &consumed) == 2) {
                writeCard(&(game->board.cards[idx]), rank, suit);
                idx++;
                cursor += consumed;
            }
            game->board.count = idx;
        } else if (strncmp(line, "player ", 7) == 0) {
            int idx, chips, currentBet, hasFolded, canAct;
            int r1, s1, r2, s2;
            char name[32];
            int matched = sscanf(line, "player %d %31s %d %d %d %d %d %d %d %d",
                                 &idx, name, &chips, &currentBet,
                                 &hasFolded, &canAct,
                                 &r1, &s1, &r2, &s2);
            if (matched == 10 && idx >= 0 && idx < MAX_PLAYERS_COUNT) {
                Player_Info* p = game->players[idx]; //changed player state to Player_Info
                strncpy(p->name, name, sizeof(p->name) - 1);
                p->name[sizeof(p->name) - 1] = '\0';
                p->chips = chips;
                p->currentBet = currentBet;
                p->hasFolded = hasFolded != 0;
                p->canAct = canAct != 0;
                writeCard(&p->playerCards[0], r1, s1);
                writeCard(&p->playerCards[1], r2, s2);
            }
        }
    }

    return sawEnd;
}
