#include <netinet/in.h>
#include <string.h>
#include "rhat.h"

long readMessage(int fd, Message* msg, long max, struct sockaddr_in* client, int* clilen)
{
    memset(msg, 0, max);
    long length = recvfrom(fd, msg, max, MSG_WAITALL, (struct sockaddr * restrict)client, clilen);

    if(msg->length == -1) { return -1; }

    if(length - sizeof(Message) != msg->length) { return 0; }
    if(msg->length > max - sizeof(Message)) { return 0; }

    return 1;
}