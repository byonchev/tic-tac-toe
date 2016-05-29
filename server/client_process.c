#include "client_process.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../common/helpers.h"

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

extern void create_room(char *name, struct in_addr client_address, int *rooms_count, room_t *rooms, room_t *existing_room)
{
    room_t new_room;
    struct in_addr address;

    new_room.name = name;
    new_room.player_address = client_address;
    new_room.taken = 0;

    if (existing_room != NULL)
    {
        free(existing_room->name);
        memcpy(existing_room, &new_room, sizeof(room_t));
    }
    else
    {
        rooms[(*rooms_count)++] = new_room;
    }
}

void *client_process_handle_socket(void *data)
{
    thread_args_t *args = (thread_args_t*)data;

    int sockfd = args->sockfd;
    struct sockaddr_in address = args->address;
    int *rooms_count = args->rooms_count;
    room_t *rooms = args->rooms;

    uint8_t message_size;
    char *room_name;
    room_t *matching_room;
    char buffer[128];

    if (read(sockfd, &message_size, 1) == 0) {
        shutdown(sockfd, 2);
        pthread_exit(NULL);
    }

    room_name = (char*)malloc(message_size);

    if (read(sockfd, room_name, message_size) < message_size)
    {
        shutdown(sockfd, 2);
        pthread_exit(NULL);
    }

    pthread_mutex_lock(args->mutex);

    matching_room = find_room(room_name, *rooms_count, rooms);

    if ((matching_room == NULL) || (matching_room->taken))
    {
        sprintf(buffer, "Room '%s' not found. Creating...\n", room_name);
        print_message(buffer);

        buffer[0] = 0;

        create_room(room_name, address.sin_addr, rooms_count, rooms, matching_room);
        write(sockfd, buffer, 1);
    }
    else
    {
        sprintf(buffer, "Room '%s' found. Sending client info...\n", room_name);
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
