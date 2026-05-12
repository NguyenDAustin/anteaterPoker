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


#define MAX_PLAYERS 3 //the max amount of clients the server will allow to connect

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


int acceptClients(int* clientSockets, int socket){ //returns current number of players accepted 
    struct sockaddr_in cli_addr; 
    socklen_t clilen; //size of client's address structure 
    int newClientSocket, joinedPlayers = 0;  

    for(int i = 0; i < MAX_PLAYERS; i++){ //right now its forcing it so 3 people must join 
        clilen = sizeof(cli_addr);
        newClientSocket = accept(socket, (struct sockaddr *) &cli_addr, &clilen); 

        if(!hasAcceptionError(newClientSocket)){
            printf("client accepted\n"); 
            clientSockets[joinedPlayers] = newClientSocket; 
            joinedPlayers++; 
        } 

    }
    return joinedPlayers; 
} 

void readMessage(int clientSocket){ 
    char buffer[256]; 
    int n; 

    printf("message being read\n"); 

    n = read(clientSocket, buffer, 255);  

    if (n < 0) error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);

    n = write(clientSocket, "I got your message", 18);  

     
    if (n < 0) error("ERROR writing to socket");
        close(clientSocket);

}

//just a local func for now
void readClientsMessages(int * clientSockets, int joinedClients){  
    int currClientSocket; 
    char buffer[256];
    int n; 

    for(int i = 0; i < joinedClients; i++){ 
        bzero(buffer,256);
        currClientSocket = clientSockets[i]; 

        n = read(currClientSocket,buffer,255); 

        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        
        n = write(currClientSocket,"I got your message",18);
        
        if (n < 0) error("ERROR writing to socket");
            close(currClientSocket);

    }
}


void lobby(int serverSocket, int* clientSockets){ 
    int joinedPlayers = 0; 
    int max_fd = serverSocket; 
    int newClientSocket; 
    struct sockaddr_in cli_addr; 
    socklen_t clilen; 

    printf("LOBBY CREATED 2\n");
    printf("NEWWWW\n"); 

    while(true){ //watch 
        fd_set socketList; 
        FD_ZERO(&socketList); 
        FD_SET(serverSocket, &socketList);

        //listen to current clients 
        for(int i = 0; i < joinedPlayers; i++){ 
            printf("clients being listened to\n"); 
            newClientSocket = clientSockets[i]; 
            FD_SET(newClientSocket, &socketList); //add client socket to be read from 

            if (clientSockets[i] > max_fd) { 
                max_fd = clientSockets[i];
            }
        }

        select(max_fd + 1, &socketList, NULL, NULL, NULL);

        printf("out of select\n"); 

        //accepting of new clients 
        if(FD_ISSET(serverSocket, &socketList)){ 
            clilen = sizeof(cli_addr);
            newClientSocket = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);

            if(!hasAcceptionError(newClientSocket)){ 
                printf("client has been accepted\n"); 
                clientSockets[joinedPlayers] = newClientSocket; 
                joinedPlayers++; 
            }
        }


        //reading current messages
        for (int i = 0; i < joinedPlayers; i++) {
        if (FD_ISSET(clientSockets[i], &socketList)) {
            readMessage(clientSockets[i]);
        }
    }

    }
}



int main(int argc, char *argv[])
{
    //array of client sockets --> aka players 

    printf("WE ARE RUNNING THE NEW VERSION\n"); 

    int clientSockets[MAX_PLAYERS]; 
    int joinedPlayers = 0; 

    int serverSocket, portno;
    struct sockaddr_in serv_addr; 

    if (argc < 2) { // checks if the number of command line arguments is less than 2 //aka not ./server portno
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0); //if obtained a port number --> create a socket

    if (serverSocket < 0) 
    error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr)); //clear possible garbage from serv_addr

    portno = atoi(argv[1]); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");

    listen(serverSocket, MAX_PLAYERS); //we will allow only these many connections in our connection queue

    lobby(serverSocket, clientSockets); 

    close(serverSocket);
    return 0; 
}