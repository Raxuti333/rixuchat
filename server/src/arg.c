#include <stdlib.h>
#include <arpa/inet.h> 

int getargs(char* arg, struct sockaddr_in* server)
{
    unsigned long i = 0;
    while(arg[i] != ':') { if(arg[i] == '\0') { return 0; } ++i; }

    arg[i] = '\0';

    inet_pton(AF_INET, arg, &(server->sin_addr));
    server->sin_port = htons((uint16_t)strtol(arg + i + 1, NULL, 10));
    server->sin_family = AF_INET;
    
    return 1;
}