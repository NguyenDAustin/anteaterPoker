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

#include "communication.h"
#include "game.h"
#include "player.h"
#include "poker_protocol.h"
#include "server.h"
#include "state.h"


#define SERVER_MAX_PLAYERS 3 //the max amount of clients the server will allow to connect
#define STARTING_CHIPS 1000

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

        printf("client has been accepted as player %d\n", playerNumber); 
        clientSockets[*joinedPlayers] = newClientSocket; 
        (*joinedPlayers)++; 
    }
} 


void readMessage(int* clientSockets, int joinedPlayers, int playerIndex, GameState* game){ 
    int clientSocket = clientSockets[playerIndex];
    char buffer[256]; 
    char stateMessage[GAME_STATE_MESSAGE_SIZE];
    ssize_t n;
    PokerActionMessage action;
    char playerName[20];

    bzero(buffer, 256); 

    printf("message being read\n"); 

    n = receiveMessage(clientSocket, buffer, sizeof(buffer));

    if (n < 0)
        error("ERROR reading from socket");

    if (n == 0) {
        printf("client disconnected\n");
        return;
    }

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
        n = sendMessage(clientSocket, "I got your poker action\n");
    } else {
        printf("Unknown client message: %s\n", buffer);
        n = sendMessage(clientSocket, "Unknown message\n");
    }

    if (n < 0)
        error("ERROR writing to socket");

}


void lobby(int serverSocket, int* clientSockets){ 
    int joinedPlayers = 0; 
    int max_fd = serverSocket; 
    int newClientSocket; 

    printf("trying to create lobby\n");


    GameState* game = malloc(sizeof(GameState)); 
    initGameState(game);

    printf("LOBBY CREATED - MULTI CONNECTION VERSION\n");

    while(true){ //watch 
        fd_set socketList; 
        FD_ZERO(&socketList); 
        FD_SET(serverSocket, &socketList);

        //listen to current clients 
        for(int i = 0; i < joinedPlayers; i++){ 
            newClientSocket = clientSockets[i]; 
            
            FD_SET(newClientSocket, &socketList); //add client socket to be read from 

            if (clientSockets[i] > max_fd) { 
                max_fd = clientSockets[i];
            }
        }

        select(max_fd + 1, &socketList, NULL, NULL, NULL);

        //accept new clients 
        if(FD_ISSET(serverSocket, &socketList))
            acceptClient(clientSockets, serverSocket, &joinedPlayers, game); 

        //reading current messages
        for (int i = 0; i < joinedPlayers; i++) {
            if (FD_ISSET(clientSockets[i], &socketList))
                readMessage(clientSockets, joinedPlayers, i, game);
        }
    }
}



int main(int argc, char *argv[])
{
    //array of client sockets --> aka players 

    printf("WE ARE RUNNING THE NEW VERSION\n"); 

    int clientSockets[SERVER_MAX_PLAYERS]; // clientSockets[i] = socket fd of player (i + 1).
                                    // first joiner is player 1 at index 0, second is
                                    // player 2 at index 1, etc. use the index as the
                                    // player ID to look up or assign per-player data.

                                    
    int serverSocket, portno;
    struct sockaddr_in serv_addr; 


    if(!portNoProvided(argc)) 
        exit(1); 

    serverSocket = socket(AF_INET, SOCK_STREAM, 0); //if obtained a port number --> create a socket

    if (serverSocket < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr)); //clear possible garbage from serv_addr

    portno = atoi(argv[1]); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");

    listen(serverSocket, SERVER_MAX_PLAYERS); //we will allow only these many connections in our connection queue

    lobby(serverSocket, clientSockets); 

    close(serverSocket);
    return 0; 
}
