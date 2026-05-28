#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "gui.h"
#include "communication.h"
#include "communication_bundle.h"


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


/*
static void sendPlayerName(Poker_Gui *pokerGui)
{
    char message[128];
    char *playerName = askPlayerName(GTK_WINDOW(pokerGui->Window));
    
    GameState* gameState = getGameState(pokerGui); 
    Player_Info* player = getPlayerInfo(gameState, 0); //1st player as default 
    setName(player, playerName); 

    if (!formatPlayerNameMessage(message, sizeof(message), playerName)) {
        printf("ERROR: was not able to format name message\n");
        g_free(playerName);
        return;
    }

    if (sendMessage(getSocket(pokerGui), message) < 0) {
        printf("ERROR: was not able to send name message\n");
    }

    g_free(playerName);
}
 */


//separate mains for 
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    
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
    
    

    printf("Waiting for server to start game...\n");

    
    int myPlayerIndex; 
    Communication_Bundle *bundle = malloc(sizeof(Communication_Bundle));

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes = receiveMessage(sockfd, buffer, sizeof(buffer) - 1);

        if (bytes <= 0) {
            printf("ERROR: server disconnected while waiting for game start\n");
            close(sockfd);
            return 1;
        }

        buffer[bytes] = '\0';

        printf("Server says: %s\n", buffer);

        if (sscanf(buffer, "YOU_ARE %d", &myPlayerIndex)) {
            printf("I am player index %d\n", myPlayerIndex); 
            bundle->playerNum = myPlayerIndex; 
            continue;
        }

        if (strncmp(buffer, "START_GAME", 10) == 0) {
            printf("Game starting! Waiting for initial game state...\n");
            break;
        }
    }

    printf("out of waiting lobby\n");

    char initialState[4096];

    memset(initialState, 0, sizeof(initialState));

    ssize_t stateBytes = receiveMessage(sockfd, initialState, sizeof(initialState) - 1);

    if (stateBytes <= 0) {
        printf("ERROR: no initial game state received\n");
        close(sockfd);
        return 1;
    }

    initialState[stateBytes] = '\0';

    printf("Initial game state: %s\n", initialState);


    if (!bundle) {
        perror("malloc bundle failed");
        close(sockfd);
        return 1;
    }

    printf("finished creating bundle\n");

    bundle->socket = sockfd;
    strncpy(bundle->stateMsg, initialState, sizeof(bundle->stateMsg) - 1);
    bundle->stateMsg[sizeof(bundle->stateMsg) - 1] = '\0';


    GtkApplication *app;
    int status;

    printf("starting gui stuff\n"); 

    app = gtk_application_new("com.anteater.poker", G_APPLICATION_NON_UNIQUE);

    g_signal_connect(app, "activate", G_CALLBACK(create_poker_gui), bundle);

    status = g_application_run(G_APPLICATION(app), 0, NULL);

    g_object_unref(app);
    free(bundle);
    close(sockfd);

    return status;
}
