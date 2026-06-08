/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <gtk/gtk.h>

#include "gui_server.h"
#include "communication.h"
#include "bot.h"
#include "game.h"
#include "lobby.h"
#include "player.h"
#include "poker_protocol.h"
#include "server.h"
#include "state.h"


#define SERVER_MAX_PLAYERS 6//the max amount of clients the server will allow to connect
#define STARTING_CHIPS 1000
#define BOT_TURN_DELAY_SECONDS 1

/* Helper used to request a GUI update on the GTK main loop from worker threads */

typedef struct {
    ServerWindow *win;
    GameState *game;
} ServerUpdateArgs;

static gboolean idle_update_server(gpointer user_data)
{
    ServerUpdateArgs *args = (ServerUpdateArgs *)user_data;
    if (args && args->win && args->game) {
        updateServerWindow(args->win, NULL, args->game);
    }
    g_free(args);
    return FALSE; /* run once */
}

static void schedule_server_update(GameState *game)
{
    if (!game) return;
    GApplication *app = g_application_get_default();
    if (!app) return;
    ServerWindow *win = (ServerWindow *)g_object_get_data(G_OBJECT(app), "server-window");
    if (!win) return;

    ServerUpdateArgs *args = g_new0(ServerUpdateArgs, 1);
    args->win = win;
    args->game = game;
    g_idle_add(idle_update_server, args);
}

static void broadcastGameState(int *clientSockets, int joinedPlayers, GameState *game)
{
    char buffer[4096];

    if (!clientSockets || joinedPlayers <= 0 || !game) {
        return;
    }

    if (formatFullGameState(buffer, sizeof(buffer), game)) {
        broadcastToAll(clientSockets, joinedPlayers, buffer);
    }
}

static void playBotTurns(GameState *game, int *clientSockets, int joinedPlayers)
{

    int safetyCounter = 0;

    if (!game) {
        return;
    }

    while (game->round != ROUND_SHOWDOWN &&
           game->currentPlayerIndex >= 0 &&
           game->currentPlayerIndex < game->numPlayers &&
           safetyCounter < SERVER_MAX_PLAYERS * 20) {
        int botIndex = game->currentPlayerIndex;
        Player_Info *player = game->players[botIndex];

        if (!player || player->playerType != BOT_PLAYER) {
            break;
        }

        broadcastGameState(clientSockets, joinedPlayers, game);
        sleep(BOT_TURN_DELAY_SECONDS);

        if (!player->canAct || player->chips <= 0) {
            if (isBettingPhaseComplete(game)) {
                advanceGameRound(game);
                broadcastGameState(clientSockets, joinedPlayers, game);
                safetyCounter++;
                continue;
            }
            break;
        }

        PlayerAction action = getBotAction(game, botIndex);
        GameActionResult result = handlePlayerAction(game, botIndex, action);

        if (result != GAME_ACTION_SUCCESS && result != GAME_ACTION_ROUND_COMPLETE) {
            PlayerAction fallback;
            int callCost = game->currentBet - player->currentBet;

            if (callCost <= 0) {
                fallback = (PlayerAction){CHECK, 0};
            } else if (player->chips > 0) {
                fallback = (PlayerAction){CALL, callCost};
            } else {
                fallback = (PlayerAction){FOLD, 0};
            }

            result = handlePlayerAction(game, botIndex, fallback);
            if (result != GAME_ACTION_SUCCESS && result != GAME_ACTION_ROUND_COMPLETE) {
                break;
            }
        }

        broadcastGameState(clientSockets, joinedPlayers, game);
        safetyCounter++;
    }
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

bool hasAcceptionError(int newClientSocket){ 

    if(newClientSocket < 0){ 
        printf("ERROR: accepting client failed\n"); 
    }

    return (newClientSocket < 0); 
}


bool portNoProvided(int args){ // checks if the number of command line arguments is less than 2 //aka not ./server portno
    if(args < 2) 
        fprintf(stderr,"ERROR, no port provided\n");
    return (args >= 2); 
}

int writeToClient(int* clientSockets, int joinedPlayers, int playerNumber, const char* message){
    if (playerNumber < 1 || playerNumber > joinedPlayers) {
        printf("ERROR: invalid player number %d (joined players: %d)\n", playerNumber, joinedPlayers);
        return -1;
    }

    int socketFd = clientSockets[playerNumber - 1];

    if (sendMessage(socketFd, message) < 0) {
        printf("ERROR: failed to write to player %d\n", playerNumber);
        return -1;
    }

    return 0;
}


int broadcastToAll(int* clientSockets, int joinedPlayers, const char* message){
    int result = 0;

    for (int i = 0; i < joinedPlayers; i++) {
        if(clientSockets[i] < 0) 
            continue; //skipping dead sockets 

        if (sendMessage(clientSockets[i], message) < 0) {
            printf("ERROR: failed to broadcast to player %d\n", i + 1);
            result = -1;
        }
    }

    return result;
}


int assignPlayerNumber(int joinedPlayers){
    // returns the player number for the next person who joins.
    // first joiner gets 1, second gets 2, etc. pass in the current
    // count of joined players (before this new person is added).
    return joinedPlayers + 1;
}


void acceptClient(int* clientSockets, int serverSocket, int* joinedPlayers, GameState* game){
    struct sockaddr_in cli_addr; 
    socklen_t clilen; //size of client's address structure 
    int newClientSocket; 

    clilen = sizeof(cli_addr);
    newClientSocket = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);

    if(!hasAcceptionError(newClientSocket)){ 
        if (*joinedPlayers >= SERVER_MAX_PLAYERS) {
            sendMessage(newClientSocket, "Lobby is full\n");
            close(newClientSocket);
            return;
        }

        int playerNumber = assignPlayerNumber(*joinedPlayers);
        char defaultName[sizeof(game->players[0]->name)];

        snprintf(defaultName, sizeof(defaultName), "Player%d", playerNumber);
        initPlayer(game->players[*joinedPlayers], defaultName, *joinedPlayers, STARTING_CHIPS, HUMAN_PLAYER);
        game->numPlayers = *joinedPlayers + 1;

        //added by queency -> send player number
        char welcomeMessage[64]; 
        snprintf(welcomeMessage, sizeof(welcomeMessage), "YOU_ARE %d\n", playerNumber);
        sendMessage(newClientSocket, welcomeMessage);

        printf("client has been accepted as player %d\n", playerNumber); 
        clientSockets[*joinedPlayers] = newClientSocket; 
        (*joinedPlayers)++; 

        /* update server GUI to reflect new player */
        schedule_server_update(game);
    }
} 


void readMessage(int* clientSockets, int joinedPlayers, int playerIndex, GameState* game){ 
    // LOBBY_WIRING: normal gameplay messages are handled here after lobby start.
    int clientSocket = clientSockets[playerIndex];

    
    if (clientSocket < 0) {
        return;
    }

    char buffer[GAME_STATE_MESSAGE_SIZE]; 
    char stateMessage[GAME_STATE_MESSAGE_SIZE];
    ssize_t n = 1;
    PokerActionMessage action;
    char playerName[20];

    bzero(buffer, GAME_STATE_MESSAGE_SIZE); 
    bzero(stateMessage, GAME_STATE_MESSAGE_SIZE); 

    printf("message being read\n"); 

    n = receiveMessage(clientSocket, buffer, sizeof(buffer));
    
    
    if (parsePlayerNameMessage(buffer, playerName, sizeof(playerName))) {
        strncpy(game->players[playerIndex]->name, playerName, sizeof(game->players[playerIndex]->name) - 1);
        game->players[playerIndex]->name[sizeof(game->players[playerIndex]->name) - 1] = '\0';
        printf("Player %d name set to %s\n", playerIndex + 1, game->players[playerIndex]->name); 

        if (formatFullGameState(stateMessage, sizeof(stateMessage), game)) {
            n = broadcastToAll(clientSockets, joinedPlayers, stateMessage);
        } else {
            n = sendMessage(clientSocket, "Name saved\n");
        } 
    } else if (parsePokerActionMessage(buffer, &action)) {
        printf("Player action: %s amount=%d\n", pokerActionTypeToString(action.type), action.amount);
        PlayerAction playerAction = {.actionType = action.type, .amount = action.amount}; 
        handlePlayerAction(game, playerIndex, playerAction); //added here
        playBotTurns(game, clientSockets, joinedPlayers);
        schedule_server_update(game);
    } else {
        printf("Unknown client message: %s\n", buffer);
        n = sendMessage(clientSocket, "Unknown message\n");
    }

    if (n < 0){
        perror("ERROR writing to socket");
        close(clientSockets[playerIndex]);
        clientSockets[playerIndex] = -1;
    }


    if (n == 0) {
        printf("client disconnected\n");
        close(clientSockets[playerIndex]);
        clientSockets[playerIndex] = -1;
        return;
    }

    printf("done reading msg\n");
}

void lobby(int serverSocket, int* clientSockets)
{ 
    // LOBBY_WIRING: keeps clients in the waiting room until Player 1 starts.
    int joinedPlayers = 0; 
    int max_fd = serverSocket; 
    int newClientSocket; 
    bool gameStarted = false;

    printf("trying to create lobby\n");

    GameState* game = malloc(sizeof(GameState));  

    if (!game) {
        perror("malloc game failed");
        exit(1);
    }

    initGameState(game);

    printf("LOBBY CREATED - MULTI CONNECTION VERSION\n");

    while (true) {
        fd_set socketList; 
        FD_ZERO(&socketList); 
        FD_SET(serverSocket, &socketList);

        max_fd = serverSocket;

        for (int i = 0; i < joinedPlayers; i++) { 
            if (clientSockets[i] < 0) {
                    continue; //SKIP DEAD SOCKETS
            }
            newClientSocket = clientSockets[i]; 
            FD_SET(newClientSocket, &socketList);

            if (clientSockets[i] > max_fd) { 
                max_fd = clientSockets[i];
            }
        }

        select(max_fd + 1, &socketList, NULL, NULL, NULL);

        if (FD_ISSET(serverSocket, &socketList)) {
            if (gameStarted) {
                struct sockaddr_in cli_addr;
                socklen_t clilen = sizeof(cli_addr);
                int lateClientSocket = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);

                if (!hasAcceptionError(lateClientSocket)) {
                    sendMessage(lateClientSocket, "Game already started\n");
                    close(lateClientSocket);
                }
                continue;
            }

            acceptClient(clientSockets, serverSocket, &joinedPlayers, game);

            if (!gameStarted) {
                broadcastLobbyStateToClients(clientSockets, joinedPlayers, game, SERVER_MAX_PLAYERS);
            }
        }

        for (int i = 0; i < joinedPlayers; i++) {
             if (clientSockets[i] < 0) {
                    continue; //SKIP DEAD SOCKETS
                }

            if (FD_ISSET(clientSockets[i], &socketList)) {
                if (!gameStarted) {
                    bool startedNow = handleLobbyClientMessage(clientSockets, joinedPlayers, i, game, &gameStarted);

                    if (startedNow) {
                        playBotTurns(game, clientSockets, joinedPlayers);
                        schedule_server_update(game);

                        broadcastGameState(clientSockets, joinedPlayers, game);
                    }

                    if (!startedNow) {
                        broadcastLobbyStateToClients(clientSockets, joinedPlayers, game, SERVER_MAX_PLAYERS);
                    }

                    continue;
                }

                readMessage(clientSockets, joinedPlayers, i, game);

                if(getRound(game) == ROUND_SHOWDOWN){
                    startNewRound(game);
                    schedule_server_update(game);
                }
                
                playBotTurns(game, clientSockets, joinedPlayers);
                schedule_server_update(game);

                broadcastGameState(clientSockets, joinedPlayers, game);
            }
        }
    }
}

static gpointer server_thread(gpointer data)
{
    int *fds = data;
    int serverSocket = fds[0];
    int *clientSockets = g_new0(int, SERVER_MAX_PLAYERS);
    lobby(serverSocket, clientSockets);
    g_free(clientSockets);
    return NULL;
}

static void on_activate(GApplication *app, gpointer user_data)
{
    int serverSocket = *(int *)user_data;
    GameState *gameState = g_new0(GameState, 1);
    initGameState(gameState);
    ServerWindow *window = createServerWindow(GTK_APPLICATION(app), gameState);
    /* populate label and initial draw from the current game state */
    updateServerWindow(window, NULL, gameState);
    g_object_set_data_full(G_OBJECT(app), "server-window", window, (GDestroyNotify)destroyServerWindow);
    
    int *fds = g_new0(int, 1);
    fds[0] = serverSocket;
    g_thread_new("server-thread", server_thread, fds);
}

int main(int argc, char *argv[])
{
    printf("WE ARE RUNNING THE NEW VERSION\n"); 
    signal(SIGPIPE, SIG_IGN); //to stop disconnection
                                    
    int serverSocket, portno;
    struct sockaddr_in serv_addr; 

    if(!portNoProvided(argc)) 
        exit(1); 

    serverSocket = socket(AF_INET, SOCK_STREAM, 0); //if obtained a port number --> create a socket

    if (serverSocket < 0) 
        error("ERROR opening socket");

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero((char *) &serv_addr, sizeof(serv_addr)); //clear possible garbage from serv_addr

    portno = atoi(argv[1]); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");

    listen(serverSocket, SERVER_MAX_PLAYERS); //we will allow only these many connections in our connection queue

    //launch server window
    GtkApplication *app = gtk_application_new("com.anteater.poker.server", G_APPLICATION_NON_UNIQUE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), &serverSocket);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    close(serverSocket);
    return status;
}
