#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "communication.h"

int main(void)
{
    int sockets[2];
    char buffer[64];
    const char *message = "Anteater Poker communication test";
    ssize_t bytesRead;

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        perror("socketpair");
        return 1;
    }

    if (sendMessage(sockets[0], message) < 0) {
        perror("sendMessage");
        close(sockets[0]);
        close(sockets[1]);
        return 1;
    }

    bytesRead = receiveMessage(sockets[1], buffer, sizeof(buffer));
    close(sockets[0]);
    close(sockets[1]);

    if (bytesRead < 0) {
        perror("receiveMessage");
        return 1;
    }

    if (strcmp(buffer, message) != 0) {
        fprintf(stderr, "communication test failed: received \"%s\"\n", buffer);
        return 1;
    }

    printf("communication test passed\n");
    return 0;
}
