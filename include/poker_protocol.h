#ifndef POKER_PROTOCOL_H
#define POKER_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>

#define POKER_MESSAGE_SIZE 128

typedef enum {
    POKER_ACTION_UNKNOWN = 0,
    POKER_ACTION_FOLD,
    POKER_ACTION_CHECK,
    POKER_ACTION_CALL,
    POKER_ACTION_BET,
    POKER_ACTION_RAISE
} PokerActionType;

typedef struct {
    PokerActionType type;
    int amount;
} PokerActionMessage;

const char* pokerActionTypeToString(PokerActionType type);
bool parsePokerActionMessage(const char* message, PokerActionMessage* action);
bool formatPokerActionMessage(char* buffer, size_t bufferSize, PokerActionType type, int amount);

#endif
