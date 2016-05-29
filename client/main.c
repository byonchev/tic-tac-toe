#include <string.h>
#include <stdio.h>
#include "server_socket.h"
#include "client_socket.h"
#include "../common/helpers.h"
#include "../common/sockets.h"

int main()
{
    int server_sockfd = create_socket();
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(12345);

    if (connect(server_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        print_error("Error connecting to master server");
        return 1;
    }

    char *test = "testroom";
    uint8_t size = (uint8_t)(strlen(test) + 1);

    write(server_sockfd, &size, 1);
    write(server_sockfd, test, size);

    struct in_addr address;

    uint8_t status;

    read(server_sockfd, &status, sizeof(uint8_t));

    int sockfd;

    int clientsockfd, client_address_length;

    struct sockaddr_in client_address;

    client_address_length = sizeof(client_address);

    switch(status)
    {
        case 0:
            sockfd = init_server_socket(54321);

            clientsockfd = accept(sockfd, (struct sockaddr*) &client_address, &client_address_length);

            break;
        case 1:
            print_message("Joining game room...");

            read(server_sockfd, &address, sizeof(address));

            printf("Address: %s\n", inet_ntoa(address));

            sockfd = init_client_socket(address, 54321);

            break;
        case 2:
            print_message("Room already taken");
            exit(0);

            break;
    }



    return 0;
}
