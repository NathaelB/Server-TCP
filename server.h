//
// Created by Nathael Bonnal on 18/04/2023.
//

#ifndef SERVER_TCP_SERVER_H
#define SERVER_TCP_SERVER_H

struct sockaddr_in create_server (int port);
void get_client_info (int sockfd, int* port);

#endif //SERVER_TCP_SERVER_H
