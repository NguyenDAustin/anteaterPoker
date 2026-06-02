#include <stdio.h>
#include <string.h>
#include "poker_protocol.h"

#define CHECK(cond, msg) do { \
    if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); return 1; } \
} while (0)

int main(void)
{
    /* action type to string */
    const char* s = pokerActionTypeToString(POKER_ACTION_RAISE);
    CHECK(s != NULL, "pokerActionTypeToString returned NULL");

    /* format then parse an action message round-trips */
    char buffer[POKER_MESSAGE_SIZE];
    CHECK(formatPokerActionMessage(buffer, sizeof(buffer), POKER_ACTION_RAISE, 100),
          "formatPokerActionMessage failed");

    PokerActionMessage parsed;
    CHECK(parsePokerActionMessage(buffer, &parsed),
          "parsePokerActionMessage failed");
    CHECK(parsed.type == POKER_ACTION_RAISE, "parsed action type mismatch");
    CHECK(parsed.amount == 100, "parsed action amount mismatch");

    /* format then parse a player name message */
    CHECK(formatPlayerNameMessage(buffer, sizeof(buffer), "Naveen"),
          "formatPlayerNameMessage failed");

    char nameBuf[64];
    CHECK(parsePlayerNameMessage(buffer, nameBuf, sizeof(nameBuf)),
          "parsePlayerNameMessage failed");
    CHECK(strcmp(nameBuf, "Naveen") == 0, "parsed name mismatch");

    /* invalid input should fail gracefully */
    CHECK(!parsePokerActionMessage("garbage", &parsed),
          "garbage input should fail to parse");

    printf("poker_protocol test passed\n");
    return 0;
}
