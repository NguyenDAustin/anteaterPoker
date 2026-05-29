#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "communication.h"
#include "lobby.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


//separate mains for 
int main(int argc, char *argv[])
{
    // LOBBY_WIRING: terminal client waits in lobby and lets Player 1 start.
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[LOBBY_MESSAGE_SIZE];
    char initialState[4096];
    int myPlayerNumber = 0;
    
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    memset(initialState, 0, sizeof(initialState));

    if (waitInLobby(sockfd, &myPlayerNumber, initialState, sizeof(initialState)) < 0) {
        close(sockfd);
        return 1;
    }

    if (initialState[0] == '\0') {
        ssize_t stateBytes = receiveMessage(sockfd, initialState, sizeof(initialState) - 1);

        if (stateBytes <= 0) {
            printf("ERROR: no initial game state received\n");
            close(sockfd);
            return 1;
        }

        initialState[stateBytes] = '\0';
    }

    printf("You are Player %d.\n", myPlayerNumber);
    printf("Initial game state: %s\n", initialState);

    while(true){
        printf("Please enter the message: ");
            bzero(buffer, sizeof(buffer));
            fgets(buffer, sizeof(buffer) - 1, stdin); 
        
        if (sendMessage(sockfd, buffer) < 0) {
            perror("ERROR writing to socket");
            break;
        }

        bzero(buffer, sizeof(buffer));
        ssize_t bytesRead = receiveMessage(sockfd, buffer, sizeof(buffer) - 1);

        if (bytesRead < 0) 
            error("ERROR reading from socket");

        if (bytesRead == 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("%s\n",buffer);

    }


    close(sockfd);
    return 0;
}
