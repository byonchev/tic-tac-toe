#ifndef SERVER_PROCESS_H
#define SERVER_PROCESS_H

#define MAX_PENDING_CONNECTIONS 10
#define MAX_ROOMS 64

void listen_server_init(char *ip_address, int port);

#endif
