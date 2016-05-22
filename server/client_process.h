#ifndef CLIENT_PROCESS_H
#define CLIENT_PROCESS_H

#include <netinet/in.h>

void client_process_handle_socket(int sockfd, struct sockaddr_in address);

#endif
