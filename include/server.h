#ifndef SERVER_H 
#define SERVER_H 


void acceptClient(int* clientSockets, int serverSocket, int* joinedPlayers); 
//accepts  clients in connection queue and initializes clientSockets array with each accepted client's unique socket   
//returns the number of players that successfully joined



//ERROR FUNCTIONS 
bool portNoProvided(int args); 
// checks if the number of command line arguments is less than 2 --> returns true if portno was provided
// if number of command line arguments is less than 2 --> then port no was not probided 
//aka ./server not ./server portno

bool hasAcceptionError(int newClientSocket); 
//given the newly joined client socket. check if the client was able to sucessfully join 
//returns true if a client wasn't able to join . function also prints out error message 






#endif 