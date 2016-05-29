#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include "server_socket.h"
#include "client_socket.h"
#include "game.h"
#include "../common/helpers.h"
#include "../common/sockets.h"

#define GAME_PORT 54321
#define MASTER_SERVER_PORT 12345

#define STATUS_ROOM_CREATED 0
#define STATUS_ROOM_FOUND 1

int connect_to_master_server()
{
    char buffer[128];
    char *hostname;
    int sockfd;
    struct sockaddr_in address;
    struct hostent *hostent;

    print_message("Enter master server address (127.0.0.1): ");

    fgets(buffer, 127, stdin);
    strtok(buffer, "\n");

    sockfd = create_socket();

    hostname = (strlen(buffer) <= 1) ? "127.0.0.1" : buffer;

    hostent = gethostbyname(hostname);

    address.sin_family = AF_INET;
    address.sin_port = htons(MASTER_SERVER_PORT);

    bcopy((char *)hostent->h_addr, (char *)&address.sin_addr.s_addr, hostent->h_length);

    if (connect(sockfd, (struct sockaddr*) &address, sizeof(address)) < 0)
    {
        return -1;
    }

    return sockfd;
}

void select_game_room(int sockfd)
{
    char buffer[256];
    uint8_t size;

    print_message("Enter game room: ");

    fgets(buffer, 255, stdin);
    strtok(buffer, "\n");

    size = (uint8_t)(strlen(buffer) + 1);

    write(sockfd, &size, 1);
    write(sockfd, buffer, size);
}

uint8_t read_game_room_status(int mastersockfd)
{
    uint8_t status;

    read(mastersockfd, &status, sizeof(uint8_t));

    return status;
}

int create_listen_socket()
{
    int listensockfd = init_server_socket(GAME_PORT);

    return accept(listensockfd, NULL, NULL);
}

int create_client_socket(int mastersockfd)
{
    struct in_addr address;

    read(mastersockfd, &address, sizeof(address));

    printf("Game room address: %s\n", inet_ntoa(address));

    return init_client_socket(address, GAME_PORT);
}

int main()
{
    int mastersockfd;
    int gamesockfd;
    uint8_t game_room_status;
    player_t player;

    mastersockfd = connect_to_master_server();

    if (mastersockfd < 0)
    {
        print_error("Error connecting to master server.\n");
        return -1;
    }

    select_game_room(mastersockfd);

    game_room_status = read_game_room_status(mastersockfd);

    switch (game_room_status)
    {
        case STATUS_ROOM_CREATED:
            gamesockfd = create_listen_socket();
            player = X_PLAYER;

            break;
        case STATUS_ROOM_FOUND:
            print_message("Joining game room...\n");

            gamesockfd = create_client_socket(mastersockfd);

            player = O_PLAYER;

            break;
        default:
            print_error("Invalid status returned from master server! Quitting...\n");
            return -1;
    }

    if (gamesockfd < 0)
    {
        print_error("Error connecting to other player...\n");
    }

    start_game(gamesockfd, player);

    return 0;
}
