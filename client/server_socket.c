#include "server_socket.h"

#include "../common/helpers.h"
#include "../common/sockets.h"

int init_server_socket(int port)
{
    int sockfd;
    struct sockaddr_in address;

    sockfd = create_socket();

    print_message("Socket created\n");

    set_socket_options(sockfd);
    bind_address(sockfd, "127.0.0.1", port);

    print_message("Socket bind to address\n");

    listen(sockfd, MAX_PENDING_CONNECTIONS);

    print_message("Waiting for player 2...\n");

    return sockfd;
}
