#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <termios.h>
#include "rhat.h"

#define MAXLEN 4096

int fd;
static struct termios oldt, newt; 

void terminationCatch(int sig) 
{ 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    close(fd);
    printf("\033[0G\033[KExiting\n");
    exit(0);
}

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

int main(int argc, char** argv)
{
    struct sockaddr server;

    char* address;
    char* port;
    char* name;

    if(argc != 2) { puts(TARGET" <name>@<address>:<port>"); return 1; }

    if(!getargs(argv[1], &address, &port, &name)) { puts(TARGET" <name>@<address>:<port>"); return 1; }
    
    if((fd = SocketConnect(address, port, &server)) == -1) { puts("Failed creating socket"); return 1; }

    printf("Running:\e[1;33m"TARGET"\e[0m Version:\e[1;95m"VERSION"\e[0m\nName:\e[1;36m%s\e[0m\nAddress:\e[1;31m%s\e[0m:\e[1;92m%s\e[0m\n", name, address, port);

    Message* sent = malloc(MAXLEN);
    memcpy(sent->msg, name, strlen(name));
    sent->msg[strlen(name)] = ':';
    sent->length = strlen(sent->msg);
    printf("\n%s",sent->msg);
    fflush(stdout);

    Message* recived = malloc(MAXLEN);


    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    struct sigaction new_action, old_action;

    new_action.sa_handler = terminationCatch;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGINT, NULL, &old_action);
    if(old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGINT, &new_action, NULL);
    }

    sigaction(SIGTERM, NULL, &old_action);
    if(old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGTERM, &new_action, NULL);
    }

    sigaction(SIGHUP, NULL, &old_action);
    if(old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGHUP, &new_action, NULL);
    }

    while(1)
    {
        if(kbhit())
        {
            sent->msg[sent->length] = fgetc(stdin);
            switch (sent->msg[sent->length])
            {
            case 8:
            case 127:
                if (sent->length > strlen(name) + 1)
                {
                    if (sent->length > 3 + strlen(name) + 1 && (sent->msg[sent->length - 4] & 0b11111000) == 0b11110000) sent->length -= 4;
                    else if (sent->length > 2 + strlen(name) + 1 && (sent->msg[sent->length - 3] & 0b11110000) == 0b11100000) sent->length -= 3;
                    else if (sent->length > 1 + strlen(name) + 1 && (sent->msg[sent->length - 2] & 0b11100000) == 0b11000000) sent->length -= 2;
                    else sent->length--;
                    printf("\033[0G\033[K");
                    for (unsigned short i = 0; i < sent->length; i++)
                    {
                        putchar(sent->msg[i]);
                    }
                    fflush(stdout);
                }
            break;
            case '\n':
                sendto(fd, sent, sent->length + sizeof(Message), MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(struct sockaddr_in));
                memset(sent, 0, MAXLEN);
                memcpy(sent->msg, name, strlen(name));
                sent->msg[strlen(name)] = ':';
                sent->length = strlen(sent->msg);
                printf("\n%s",sent->msg);
                fflush(stdout);
            break;
            default:
                printf("%c", sent->msg[sent->length]);
                sent->length++;
                fflush(stdout);
                break;
            }

        }

        if(recv(fd, recived, MAXLEN, MSG_DONTWAIT) > 0)
        {
            long response = -1;
            sendto(fd, &response, sizeof(response), MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(struct sockaddr_in));

            printf("\033[0G\033[K");
            printf("%s",recived->msg);
            memset(recived, 0, MAXLEN);
            printf("\n%s",sent->msg);
            fflush(stdout);
        }
    }

    return 0;
}