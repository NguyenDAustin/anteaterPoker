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
