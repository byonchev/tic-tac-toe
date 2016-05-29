#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "../common/sockets.h"

int init_client_socket(struct in_addr address, int port);

#endif
