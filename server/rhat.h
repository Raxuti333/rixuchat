#ifndef RHAT_H
#define RHAT_H

typedef struct Message { long length; char msg[]; } Message;

typedef struct cliinfo { int life; struct sockaddr_in addr; } cliinfo;

int getargs(char* arg, struct sockaddr_in* server);
long readMessage(int fd, Message* msg, long max, struct sockaddr_in* client, int* clilen);

#endif