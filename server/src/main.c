#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "rhat.h"

#define MAXLEN 4096

int main(int argc, char** argv)
{
    struct sockaddr_in server;
    int fd;

    unsigned int count = 0;
    cliinfo* clients = NULL;

    if(argc != 2) { puts("rhat-server <address>:<port>"); return 1; }

    if(!getargs(argv[1], &server)) { puts("rhat-server <address>:<port>"); return 1; }

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) { puts("failed creating socket"); return 1; }

    if(bind(fd, (const struct sockaddr *)&server, sizeof(server)) == -1) { puts("failed binding socket"); return 1; }

    Message* msg = malloc(MAXLEN);

    while(1)
    {
        struct sockaddr_in client;
        int clilen = sizeof(client);

        long length = readMessage(fd, msg, MAXLEN, &client, &clilen);

        if(length > 0)
        { 
            printf("%s", msg->msg);

            for(unsigned int i = 0; i < count; ++i)
            {
                clients[i].life--;
                if(!memcmp(&client, &clients[i].addr, sizeof(struct sockaddr_in))) { printf(" %i\n", clients[i].life); clients[i].life = 100; continue; }
                if(sendto(fd, msg, msg->length + sizeof(Message), MSG_CONFIRM, (const struct sockaddr *)&clients[i].addr, sizeof(struct sockaddr_in)) == -1) { puts("faild to send message"); }
                if(clients[i].life == 10) { long question = -1L; sendto(fd, &question, sizeof(long), MSG_CONFIRM, (const struct sockaddr *)&clients[i].addr, sizeof(struct sockaddr_in)); }
                /*TODO FIX REMOVING CONNECTIONS*/
                if(clients[i].life < 0) { for(unsigned int j = i + 1; j < count; ++j) { clients[j - 1] = clients[j]; } --count; clients = realloc(clients, sizeof(cliinfo) * count); }
            }
        }
        if(length == -1)
        {
            for(unsigned int i = 0; i < count; ++i)
            {
                if(!memcmp(&client, &clients[i].addr, sizeof(struct sockaddr_in))) { clients[i].life = 100; goto END; }
            }

            ++count;
            clients = realloc(clients, sizeof(cliinfo) * count);

            memcpy(&clients[count-1].addr, &client, sizeof(struct sockaddr_in));
            clients[count-1].life = 100;

            END:;
        }
    }
}