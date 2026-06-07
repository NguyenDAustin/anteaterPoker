#include "lobby.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#include "communication.h"
#include "bot.h"
#include "game.h"
#include "poker_protocol.h"

#define BOT_STARTING_CHIPS 1000
// Password Functions
#define LOBBY_PASSWORD "anteater"

// Password Functions
static bool lobbyAuthenticated[MAX_PLAYERS_COUNT];
static bool botRandomSeeded = false;

static bool startsWithMessage(const char *message, const char *prefix)
{
    size_t prefixLength;

    if (!message || !prefix) {
        return false;
    }

    prefixLength = strlen(prefix);
    return strncmp(message, prefix, prefixLength) == 0;
}

static bool isTypedStartCommand(const char *message)
{
    const char *cursor = message;

    if (!cursor) {
        return false;
    }

    while (isspace((unsigned char)*cursor)) {
        cursor++;
    }

    return strncasecmp(cursor, "START", 5) == 0 || strncasecmp(cursor, "PLAY", 4) == 0;
}

bool formatLobbyStateMessage(char *buffer, size_t bufferSize, const GameState *game, int maxPlayers)
{
    int offset;
    int written;

    if (!buffer || bufferSize == 0 || !game) {
        return false;
    }

    written = snprintf(buffer, bufferSize, "LOBBY_STATE %d %d\n", game->numPlayers, maxPlayers);
    if (written < 0 || (size_t)written >= bufferSize) {
        return false;
    }

    offset = written;

    for (int i = 0; i < game->numPlayers && i < MAX_PLAYERS_COUNT; i++) {
        const char *name = game->players[i] ? game->players[i]->name : "Player";

        written = snprintf(buffer + offset, bufferSize - offset, "PLAYER %d %s\n", i + 1, name);
        if (written < 0 || (size_t)written >= bufferSize - offset) {
            return false;
        }

        offset += written;
    }

    return true;
}

bool parseLobbyStateMessage(const char *message, LobbyState *lobbyState)
{
    const char *line;
    int joinedPlayers;
    int maxPlayers;

    if (!message || !lobbyState) {
        return false;
    }

    memset(lobbyState, 0, sizeof(*lobbyState));

    if (sscanf(message, "LOBBY_STATE %d %d", &joinedPlayers, &maxPlayers) != 2) {
        return false;
    }

    lobbyState->joinedPlayers = joinedPlayers;
    lobbyState->maxPlayers = maxPlayers;

    line = strstr(message, "\nPLAYER ");
    while (line) {
        int playerNumber;
        char playerName[20];

        line++;
        if (sscanf(line, "PLAYER %d %19s", &playerNumber, playerName) == 2 &&
            playerNumber >= 1 &&
            playerNumber <= MAX_PLAYERS_COUNT) {
            strncpy(lobbyState->playerNames[playerNumber - 1], playerName,
                    sizeof(lobbyState->playerNames[playerNumber - 1]) - 1);
        }

        line = strstr(line, "\nPLAYER ");
    }

    return true;
}

bool isLobbyStartRequest(const char *message)
{
    return startsWithMessage(message, LOBBY_START_REQUEST) || isTypedStartCommand(message);
}

bool isLobbyStartGameMessage(const char *message)
{
    return startsWithMessage(message, LOBBY_START_GAME);
}

bool formatLobbyStartRequest(char *buffer, size_t bufferSize)
{
    int written;

    if (!buffer || bufferSize == 0) {
        return false;
    }

    written = snprintf(buffer, bufferSize, "%s\n", LOBBY_START_REQUEST);
    return written > 0 && (size_t)written < bufferSize;
}

bool canStartLobbyGame(int joinedPlayers)
{
    return joinedPlayers >= 1;
}

static BotType randomBotType(void)
{
    BotType botTypes[] = { BLUFFER, AGGRESSIVE, CONSERVATIVE, BALANCED };
    int botTypeCount = sizeof(botTypes) / sizeof(botTypes[0]);

    if (!botRandomSeeded) {
        srand((unsigned int)time(NULL));
        botRandomSeeded = true;
    }

    return botTypes[rand() % botTypeCount];
}

static void fillEmptySeatsWithBots(GameState *game, int maxPlayers)
{
    if (!game) {
        return;
    }

    while (game->numPlayers < maxPlayers && game->numPlayers < MAX_PLAYERS_COUNT) {
        int seat = game->numPlayers;
        char botName[20];

        snprintf(botName, sizeof(botName), "Bot%d", seat + 1);
        initBot(game->players[seat], botName, seat, BOT_STARTING_CHIPS);
        setBotType(game->players[seat], randomBotType());
        game->numPlayers++;
    }
}

static void sanitizeLobbyPlayerName(char *playerName)
{
    // CUSTOM_PLAYER_NAME: keep saved names compatible with one-word state parsing.
    if (!playerName) {
        return;
    }

    for (int i = 0; playerName[i] != '\0'; i++) {
        if (isspace((unsigned char)playerName[i])) {
            playerName[i] = '_';
        }
    }
}

int broadcastLobbyStateToClients(int *clientSockets, int joinedPlayers, GameState *game, int maxPlayers)
{
    char lobbyMessage[LOBBY_MESSAGE_SIZE];
    int result = 0;

    if (!formatLobbyStateMessage(lobbyMessage, sizeof(lobbyMessage), game, maxPlayers)) {
        return -1;
    }

    for (int i = 0; i < joinedPlayers; i++) {
        if (clientSockets[i] < 0) {
            continue;
        }

        if (sendMessage(clientSockets[i], lobbyMessage) < 0) {
            result = -1;
        }
    }

    return result;
}

bool handleLobbyClientMessage(int *clientSockets, int joinedPlayers, int playerIndex, GameState *game, bool *gameStarted)
{
    int clientSocket = clientSockets[playerIndex];
    char buffer[LOBBY_MESSAGE_SIZE];
    char playerName[20];
    char password[64];
    ssize_t bytesRead;

    if (!clientSockets || !game || !gameStarted || clientSocket < 0) {
        return false;
    }

    memset(buffer, 0, sizeof(buffer));
    bytesRead = receiveMessage(clientSocket, buffer, sizeof(buffer) - 1);

    if (bytesRead <= 0) {
        printf("client disconnected in lobby\n");
        close(clientSockets[playerIndex]);
        clientSockets[playerIndex] = -1;
        // Password Functions
        lobbyAuthenticated[playerIndex] = false;
        return false;
    }

    // Password Functions
    if (parseJoinMessage(buffer, playerName, sizeof(playerName), password, sizeof(password))) {
        if (strcmp(password, LOBBY_PASSWORD) != 0) {
            sendMessage(clientSocket, "JOIN_ERROR Bad password\n");
            printf("Player %d entered a bad lobby password\n", playerIndex + 1);
            lobbyAuthenticated[playerIndex] = false;
            return false;
        }

        sanitizeLobbyPlayerName(playerName);
        strncpy(game->players[playerIndex]->name, playerName, sizeof(game->players[playerIndex]->name) - 1);
        game->players[playerIndex]->name[sizeof(game->players[playerIndex]->name) - 1] = '\0';
        lobbyAuthenticated[playerIndex] = true;

        sendMessage(clientSocket, "JOIN_OK\n");
        printf("Player %d joined as %s\n", playerIndex + 1, game->players[playerIndex]->name);
        return false;
    }

    // Password Functions
    if (!lobbyAuthenticated[playerIndex]) {
        sendMessage(clientSocket, "JOIN_ERROR Please enter your name and password first.\n");
        return false;
    }

    if (parsePlayerNameMessage(buffer, playerName, sizeof(playerName))) {
        // CUSTOM_PLAYER_NAME: save lobby name changes before the game starts.
        sanitizeLobbyPlayerName(playerName);
        strncpy(game->players[playerIndex]->name, playerName, sizeof(game->players[playerIndex]->name) - 1);
        game->players[playerIndex]->name[sizeof(game->players[playerIndex]->name) - 1] = '\0';
        printf("Player %d lobby name set to %s\n", playerIndex + 1, game->players[playerIndex]->name);
        return false;
    }

    if (!isLobbyStartRequest(buffer)) {
        printf("Unknown lobby message: %s\n", buffer);
        sendMessage(clientSocket, "Waiting room command not recognized.\n");
        return false;
    }

    if (playerIndex != 0) {
        sendMessage(clientSocket, "Only Player 1 can start the game.\n");
        return false;
    }

    if (!canStartLobbyGame(joinedPlayers)) {
        sendMessage(clientSocket, "Not enough players to start.\n");
        return false;
    }

    printf("Player 1 started the game from the lobby.\n");
    fillEmptySeatsWithBots(game, MAX_PLAYERS_COUNT);
    startNewRound(game);
    *gameStarted = true;

    for (int i = 0; i < joinedPlayers; i++) {
        if (clientSockets[i] >= 0) {
            sendMessage(clientSockets[i], LOBBY_START_GAME "\n");
        }
    }

    return true;
}

void printLobbyState(const LobbyState *lobbyState, int myPlayerNumber)
{
    if (!lobbyState) {
        return;
    }

    printf("\nWaiting room (%d/%d players)\n", lobbyState->joinedPlayers, lobbyState->maxPlayers);

    for (int i = 0; i < lobbyState->joinedPlayers && i < MAX_PLAYERS_COUNT; i++) {
        const char *name = lobbyState->playerNames[i][0] ? lobbyState->playerNames[i] : "Player";
        printf("Player %d: %s\n", i + 1, name);
    }

    if (myPlayerNumber == 1) {
        if (canStartLobbyGame(lobbyState->joinedPlayers)) {
            printf("Type START or PLAY, then press Enter to start the game.\n");
        } else {
            printf("Waiting for at least %d players before Player 1 can start.\n", LOBBY_MIN_PLAYERS);
        }
    } else {
        printf("Waiting for Player 1 to start the game.\n");
    }

    fflush(stdout);
}

int waitInLobby(int socketFd, int *myPlayerNumber, char *initialState, size_t initialStateSize)
{
    char buffer[LOBBY_MESSAGE_SIZE];

    if (!myPlayerNumber) {
        return -1;
    }

    if (initialState && initialStateSize > 0) {
        initialState[0] = '\0';
    }

    printf("Connected. Waiting room is open.\n");

    while (true) {
        fd_set readSet;
        int maxFd = socketFd;

        FD_ZERO(&readSet);
        FD_SET(socketFd, &readSet);

        if (*myPlayerNumber == 1) {
            FD_SET(STDIN_FILENO, &readSet);
            if (STDIN_FILENO > maxFd) {
                maxFd = STDIN_FILENO;
            }
        }

        if (select(maxFd + 1, &readSet, NULL, NULL, NULL) < 0) {
            perror("ERROR: lobby select failed");
            return -1;
        }

        if (FD_ISSET(socketFd, &readSet)) {
            LobbyState lobbyState;
            ssize_t bytesRead;

            memset(buffer, 0, sizeof(buffer));
            bytesRead = receiveMessage(socketFd, buffer, sizeof(buffer) - 1);
            if (bytesRead <= 0) {
                printf("ERROR: server disconnected while in waiting room\n");
                return -1;
            }

            if (sscanf(buffer, "YOU_ARE %d", myPlayerNumber) == 1) {
                char *lobbyStart = strstr(buffer, "LOBBY_STATE");

                printf("You are Player %d.\n", *myPlayerNumber);

                if (lobbyStart && parseLobbyStateMessage(lobbyStart, &lobbyState)) {
                    printLobbyState(&lobbyState, *myPlayerNumber);
                }

                continue;
            }

            if (isLobbyStartGameMessage(buffer)) {
                char *stateStart = strstr(buffer, "STATE\n");

                if (stateStart && initialState && initialStateSize > 0) {
                    strncpy(initialState, stateStart, initialStateSize - 1);
                    initialState[initialStateSize - 1] = '\0';
                }

                printf("Game starting!\n");
                return 0;
            }

            if (parseLobbyStateMessage(buffer, &lobbyState)) {
                printLobbyState(&lobbyState, *myPlayerNumber);
                continue;
            }

            printf("Server says: %s\n", buffer);
        }

        if (*myPlayerNumber == 1 && FD_ISSET(STDIN_FILENO, &readSet)) {
            char startMessage[LOBBY_MESSAGE_SIZE];

            memset(buffer, 0, sizeof(buffer));
            if (!fgets(buffer, sizeof(buffer), stdin)) {
                continue;
            }

            if (!isTypedStartCommand(buffer)) {
                printf("Type START or PLAY to begin when enough players have joined.\n");
                continue;
            }

            if (formatLobbyStartRequest(startMessage, sizeof(startMessage))) {
                sendMessage(socketFd, startMessage);
            }
        }
    }
}
