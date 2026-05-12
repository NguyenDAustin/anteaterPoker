#ifnfdef SERVER_H 
#define SERVER_H 


int acceptClients(int* clientSockets, int socket); 
//accepts all clients in connection queue and initializes clientSockets array with each accepted client's unique socket   
//returns the number of players that successfully joined



#endif 