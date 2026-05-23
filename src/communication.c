#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "communication.h"

int sendMessage(int socketFd, const char *message)
{
    size_t totalSent = 0;
    size_t messageLength = strlen(message);

    while (totalSent < messageLength) {
        ssize_t bytesSent = write(socketFd, message + totalSent, messageLength - totalSent);

        if (bytesSent < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        if (bytesSent == 0) {
            return -1;
        }

        totalSent += bytesSent;
    }

    return 0;
}

ssize_t receiveMessage(int socketFd, char *buffer, size_t bufferSize)
{
    ssize_t bytesRead;

    if (bufferSize == 0) {
        return -1;
    }

    do {
        bytesRead = read(socketFd, buffer, bufferSize - 1);
    } while (bytesRead < 0 && errno == EINTR);

    if (bytesRead <= 0) {
        return bytesRead;
    }

    buffer[bytesRead] = '\0';
    return bytesRead;
}
