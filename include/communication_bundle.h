#ifndef COMMUNICATION_BUNDLE 
#define COMMUNICATION_BUNDLE


typedef struct {
    int socket; 
    int playerNum; 
    char stateMsg[4096];
} Communication_Bundle; 

#endif