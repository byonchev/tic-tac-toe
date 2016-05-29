#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int create_socket();
void bind_address(int sockfd, char *ip_address, int port);
void set_socket_options(int sockfd);

#endif
