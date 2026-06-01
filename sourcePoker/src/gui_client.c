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
    // LOBBY_WIRING: GUI client waits in the shared lobby before opening the table.
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
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
    
    

    Communication_Bundle *bundle = malloc(sizeof(Communication_Bundle));

    if (!bundle) {
        perror("malloc bundle failed");
        close(sockfd);
        return 1;
    }

    bundle->socket = sockfd;
    bundle->playerNum = 0;
    bundle->stateMsg[0] = '\0';


    GtkApplication *app;
    int status;

    printf("starting gui stuff\n"); 

    app = gtk_application_new("com.anteater.poker", G_APPLICATION_NON_UNIQUE);

    g_signal_connect(app, "activate", G_CALLBACK(createWaitingRoom), bundle);

    status = g_application_run(G_APPLICATION(app), 0, NULL);

    g_object_unref(app);
    free(bundle);
    close(sockfd);

    return status;
}
