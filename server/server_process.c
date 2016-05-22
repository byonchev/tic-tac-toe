#include "server_process.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
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

    // Prevents children from becoming zombies (¬º-°)¬
    signal(SIGCHLD, SIG_IGN);

    client_address_length = sizeof(client_address);

    while(1) {
        clientsockfd = accept(sockfd, (struct sockaddr*) &client_address, &client_address_length);

        print_client_status_message(client_address, 1);

        pid = fork();

        if (pid == 0) {
            client_process_handle_socket(clientsockfd, client_address);
            print_client_status_message(client_address, 0);
            exit(0);
        }
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
