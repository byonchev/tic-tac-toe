#ifndef CLIENT_PROCESS_H
#define CLIENT_PROCESS_H

#include <netinet/in.h>
#include "../common/room.h"

typedef struct {
    int sockfd;
    struct sockaddr_in address;
    int *rooms_count;
    room_t *rooms;
    pthread_mutex_t *mutex;
} thread_args_t;

void *client_process_handle_socket(void *data);

#endif
