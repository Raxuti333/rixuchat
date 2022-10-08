#include <stdio.h>
#include "rhat.h"

int getargs(char* args, char** address, char** port, char** name)
{
    *name = args;

    unsigned long i = 0;
    while(args[i] != '@') { if(args[i] == '\0') { return 0; } ++i; }

    args[i] = '\0';

    *address = args + i + 1;

    i = 0;
    while((*address)[i] != ':') { if((*address)[i] == '\0') { return 0; } ++i; }

    (*address)[i] = '\0';

    *port = (*address) + i + 1;

    return 1;
}