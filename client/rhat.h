#ifndef RHAT_H
#define RHAT_H
#include <sys/socket.h>

typedef struct Message { long length; char msg[]; } Message;

int SocketConnect(char* addres, char* port, struct sockaddr* server);
int getargs(char* args, char** address, char** port, char** name);

#endif