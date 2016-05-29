#include "client_process.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common.h"

extern room_t* find_room(char *name, int rooms_count, room_t *rooms)
{
    int i;

    for (i = 0; i < rooms_count; i += 1)
    {
        if (strcmp(rooms[i].name, name) == 0)
        {
            return &rooms[i];
        }
    }

    return NULL;
}

extern void create_room(char *name, struct in_addr client_address, int *rooms_count, room_t *rooms)
{
    room_t new_room;
    struct in_addr address;

    new_room.name = name;
    new_room.player_address = client_address;
    new_room.taken = 0;

    rooms[(*rooms_count)++] = new_room;
}

void *client_process_handle_socket(void *data)
{
    thread_args_t *args = (thread_args_t*)data;

    int sockfd = args->sockfd;
    struct sockaddr_in address = args->address;
    int *rooms_count = args->rooms_count;
    room_t *rooms = args->rooms;

    int message_size;
    char *room_name;
    room_t *matching_room;
    char buffer[128];

    read(sockfd, &message_size, sizeof(int));

    room_name = (char*)malloc(message_size);

    read(sockfd, room_name, message_size);

    pthread_mutex_lock(args->mutex);

    matching_room = find_room(room_name, *rooms_count, rooms);

    if (matching_room == NULL)
    {
        sprintf(buffer, "Room '%s' not found. Creating...", room_name);
        print_message(buffer);

        buffer[0] = 0;

        create_room(room_name, address.sin_addr, rooms_count, rooms);
        write(sockfd, buffer, 1);
    }
    else if (mathing_room->taken)
    {
        sprintf(buffer, "Room '%s' already taken...", room_name);

        buffer[0] = 2;

        write(sockfd, buffer, 1);
    }
    else
    {
        sprintf(buffer, "Room '%s' found. Sending client info...", room_name);
        print_message(buffer);

        matching_room->taken = 1;

        buffer[0] = 1;

        write(sockfd, buffer, 1);
        write(sockfd, &matching_room->player_address, sizeof(struct in_addr));
    }

    pthread_mutex_unlock(args->mutex);

    shutdown(sockfd, 2);

    pthread_exit(NULL);
}
