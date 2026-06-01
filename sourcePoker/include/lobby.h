#ifndef LOBBY_H
#define LOBBY_H

#include <stdbool.h>
#include <stddef.h>

#include "state.h"

#define LOBBY_MESSAGE_SIZE 1024
#define LOBBY_START_REQUEST "START_REQUEST"
#define LOBBY_START_GAME "START_GAME"
#define LOBBY_MIN_PLAYERS 1

typedef struct {
    int joinedPlayers;
    int maxPlayers;
    char playerNames[MAX_PLAYERS_COUNT][20];
} LobbyState;

bool formatLobbyStateMessage(char *buffer, size_t bufferSize, const GameState *game, int maxPlayers);
bool parseLobbyStateMessage(const char *message, LobbyState *lobbyState);
bool isLobbyStartRequest(const char *message);
bool isLobbyStartGameMessage(const char *message);
bool formatLobbyStartRequest(char *buffer, size_t bufferSize);
bool canStartLobbyGame(int joinedPlayers);
void printLobbyState(const LobbyState *lobbyState, int myPlayerNumber);
int waitInLobby(int socketFd, int *myPlayerNumber, char *initialState, size_t initialStateSize);
int broadcastLobbyStateToClients(int *clientSockets, int joinedPlayers, GameState *game, int maxPlayers);
bool handleLobbyClientMessage(int *clientSockets, int joinedPlayers, int playerIndex, GameState *game, bool *gameStarted);

#endif
