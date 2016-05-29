#include "server_process.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "common.h"
#include "client_process.h"

static int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        print_error("Error creating socket\n");
        exit(1);
    }

    return sockfd;
}

static void bind_address(int sockfd, char *ip_address, int port)
{
    int result;
    struct sockaddr_in address;

    address.sin_addr.s_addr = inet_addr(ip_address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    result = bind(sockfd, (struct sockaddr*) &address, sizeof(address));

    if (result < 0)
    {
        print_error("Error on binding address\n");
        exit(1);
    }
}

static void set_socket_options(int sockfd)
{
    int reuseaddr = 1;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));
}

static void print_client_status_message(struct sockaddr_in address, int connected)
{
    char buffer[40];

    sprintf(buffer, "Client %*s (%s)\n", -12, (connected ? "connected" : "disconnected"), inet_ntoa(address.sin_addr));

    print_message(buffer);
}

static void handle_clients(int sockfd)
{
    char buffer[128];

    struct sockaddr_in client_address;

    int clientsockfd, pid, client_address_length;

    client_address_length = sizeof(client_address);

    int rooms_count = 0;
    room_t rooms[MAX_ROOMS];

    pthread_t threads[MAX_PENDING_CONNECTIONS];
    thread_args_t thread_args[MAX_PENDING_CONNECTIONS];
    int threads_count = 0;
    int i;

    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);

    while(1) {
        clientsockfd = accept(sockfd, (struct sockaddr*) &client_address, &client_address_length);

        print_client_status_message(client_address, 1);

        if (threads_count >= MAX_PENDING_CONNECTIONS)
        {
            for (i = 0; i < threads_count; i += 1)
            {
                pthread_join(threads[i], NULL);
            }

            threads_count = 0;
        }

        thread_args[threads_count].sockfd = clientsockfd;
        thread_args[threads_count].address = client_address;
        thread_args[threads_count].rooms_count = &rooms_count;
        thread_args[threads_count].rooms = rooms;
        thread_args[threads_count].mutex = &mutex;

        pthread_create(&threads[threads_count], NULL, client_process_handle_socket, &thread_args[threads_count]);

        threads_count++;
    }
}

void listen_server_init(char *ip_address, int port)
{
    int sockfd;
    struct sockaddr_in address;

    sockfd = create_socket();

    print_message("Socket created\n");

    set_socket_options(sockfd);
    bind_address(sockfd, ip_address, port);

    print_message("Socket bind to address\n");

    listen(sockfd, MAX_PENDING_CONNECTIONS);

    print_message("Listening...\n");

    handle_clients(sockfd);

    shutdown(sockfd, 2);
}
