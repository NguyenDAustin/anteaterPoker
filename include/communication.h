#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stddef.h>
#include <sys/types.h>

int sendMessage(int socketFd, const char *message);
ssize_t receiveMessage(int socketFd, char *buffer, size_t bufferSize);

#endif
