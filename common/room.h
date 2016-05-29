#ifndef ROOM_H
#define ROOM_H

#include <netinet/in.h>

typedef struct
{
    char *name;
    struct in_addr player_address;
    int taken;
} room_t;

#endif
