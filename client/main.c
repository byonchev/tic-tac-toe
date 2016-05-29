#include <string.h>
#include <stdio.h>
#include "server_socket.h"
#include "client_socket.h"
#include "game.h"
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

    char room_name[255];

    printf("Enter game room:");

    fgets(room_name, 255, stdin);
    strtok(room_name, "\n");

    uint8_t size = (uint8_t)(strlen(room_name) + 1);

    write(server_sockfd, &size, 1);
    write(server_sockfd, room_name, size);

    struct in_addr address;

    uint8_t status;

    read(server_sockfd, &status, sizeof(uint8_t));

    int sockfd;

    int listensockfd, client_address_length;

    struct sockaddr_in client_address;

    client_address_length = sizeof(client_address);

    player_t player;

    switch(status)
    {
        case 0:
            listensockfd = init_server_socket(54321);

            sockfd = accept(listensockfd, (struct sockaddr*) &client_address, &client_address_length);
            player = X_PLAYER;

            break;
        case 1:
            print_message("Joining game room...\n");

            read(server_sockfd, &address, sizeof(address));

            printf("Game room address: %s\n", inet_ntoa(address));

            sockfd = init_client_socket(address, 54321);
            player = O_PLAYER;
            break;
    }

    if (sockfd > 0)
    {
        start_game(sockfd, player);
    }

    return 0;
}
