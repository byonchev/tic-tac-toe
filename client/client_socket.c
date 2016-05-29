#include "client_socket.h"

#include "../common/helpers.h"

int init_client_socket(struct in_addr address, int port)
{
    int sockfd = create_socket();
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = address;
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        print_error("Error connecting to game server");
        return -1;
    }

    return sockfd;
}
