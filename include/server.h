#ifndef SERVER_H 
#define SERVER_H 


void acceptClient(int* clientSockets, int serverSocket, int* joinedPlayers);
//accepts  clients in connection queue and initializes clientSockets array with each accepted client's unique socket
//returns the number of players that successfully joined

int assignPlayerNumber(int joinedPlayers);
//returns the player number for the next person who joins.
//first joiner returns 1, second returns 2, etc.
//pass the current joined-player count (BEFORE the new person is added).
//note: store the result in a separate playerNumbers[] array,
//      NOT in clientSockets[] (which must hold socket fds for select()).



//ERROR FUNCTIONS 
bool portNoProvided(int args); 
// checks if the number of command line arguments is less than 2 --> returns true if portno was provided
// if number of command line arguments is less than 2 --> then port no was not probided 
//aka ./server not ./server portno

bool hasAcceptionError(int newClientSocket);
//given the newly joined client socket. check if the client was able to sucessfully join
//returns true if a client wasn't able to join . function also prints out error message


//SERVER-TO-CLIENT WRITE FUNCTIONS
int writeToClient(int* clientSockets, int joinedPlayers, int playerNumber, const char* message);
//sends message to a specific player identified by playerNumber (1-indexed; first joiner is 1).
//returns 0 on success, -1 on failure (bad player number or socket write error).

int broadcastToAll(int* clientSockets, int joinedPlayers, const char* message);
//sends the same message to every connected client.
//returns 0 if all writes succeeded, -1 if any failed.


#endif