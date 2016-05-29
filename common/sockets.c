#include "sockets.h"

#include "helpers.h"

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        print_error("Error creating socket\n");
        exit(1);
    }

    return sockfd;
}

void bind_address(int sockfd, char *ip_address, int port)
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

void set_socket_options(int sockfd)
{
    int reuseaddr = 1;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));
}
