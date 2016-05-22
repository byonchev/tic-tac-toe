#include "client_process.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

void client_process_handle_socket(int sockfd, struct sockaddr_in address)
{
    char buffer[100];

    write(sockfd,"I got your message\n",19);

    shutdown(sockfd, 2);
}
