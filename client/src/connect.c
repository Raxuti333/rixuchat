#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>

int SocketConnect(char* addres, char* port, struct sockaddr* server)
{
    int fd;
    struct addrinfo HintsAdderInfo;
    struct addrinfo* ptrAddrInfo;

    const long CONNECT = -1L;

    memset(&HintsAdderInfo, 0, sizeof(struct addrinfo));
    HintsAdderInfo.ai_family = AF_INET;
    HintsAdderInfo.ai_socktype = SOCK_DGRAM;

    if(getaddrinfo(addres, port, &HintsAdderInfo, &ptrAddrInfo))
    {
        if(ptrAddrInfo != NULL) { freeaddrinfo(ptrAddrInfo); }

        return -1;
    }

    for(ptrAddrInfo; ptrAddrInfo != NULL; ptrAddrInfo = ptrAddrInfo->ai_next)
    {
        fd = socket(ptrAddrInfo->ai_family, ptrAddrInfo->ai_socktype, ptrAddrInfo->ai_protocol);

        if(fd == -1) { continue; }

        if(sendto(fd, &CONNECT, sizeof(CONNECT), MSG_CONFIRM, ptrAddrInfo->ai_addr, ptrAddrInfo->ai_addrlen) != -1)
        {
            memcpy(server, ptrAddrInfo->ai_addr, ptrAddrInfo->ai_addrlen);

            if(ptrAddrInfo != NULL) { freeaddrinfo(ptrAddrInfo); }

            return fd;
        }

        close(fd);
    }

    if(ptrAddrInfo != NULL) { freeaddrinfo(ptrAddrInfo); }

    return -1;
}