/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


#define MAX_PLAYERS 3 //the max amount of clients the server will allow to connect

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int acceptClients(int* clientSockets, int socket){ //returns current number of players accepted 
    struct sockaddr_in cli_addr; 
    socklen_t clilen; //size of client's address structure 
    int newClientSocket, joinedPlayers = 0;  

    for(int i = 0; i < MAX_PLAYERS; i++){ 
        clilen = sizeof(cli_addr);
        newClientSocket = accept(socket, (struct sockaddr *) &cli_addr, &clilen); 

        if(newClientSocket < 0){ //error accepting client
            printf("ERROR on accept\n");  
        }
        else{
            clientSockets[joinedPlayers] = newClientSocket; 
            joinedPlayers++; 
        }
    }
    return joinedPlayers; 
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



int main(int argc, char *argv[])
{
    //array of client sockets --> aka players 

    int clientSockets[MAX_PLAYERS]; 
    int joinedPlayers = 0; 

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc < 2) { // checks if the number of command line arguments is less than 2 //aka not ./server portno
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0); //if obtained a port number --> create a socket

     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr)); //clear possible garbage from serv_addr

     portno = atoi(argv[1]); 

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");

     listen(sockfd, MAX_PLAYERS); //we will allow only these many connections in our connection queue

     joinedPlayers = acceptClients(clientSockets, sockfd); 

     readClientsMessages(clientSockets, joinedPlayers); 

     /*clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //accepts the client that connected and return the unique client socket
    
    if (newsockfd < 0) 
          error("ERROR on accept"); */

     //bzero(buffer,256);
     //n = read(newsockfd,buffer,255);  

     close(sockfd);
     return 0; 
}