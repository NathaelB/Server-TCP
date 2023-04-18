//
// Created by Nathael Bonnal on 18/04/2023.
//

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "server.h"

/**
 * Create basic Server
 * @param port
 * @return
 */
struct sockaddr_in create_server (int port) {
  struct sockaddr_in Server;

  Server.sin_family = AF_INET;
  Server.sin_addr.s_addr = INADDR_ANY;
  Server.sin_port = htons(port);

  return Server;
}

/**
 * Get adress and port for a Client Server
 * @param sockfd
 * @param port
 */
void get_client_info (int sockfd, int* port) {
  int res;
  struct sockaddr_in client_addr;
  socklen_t len = sizeof (client_addr);

  res = getpeername(sockfd, (struct sockaddr*) &client_addr, &len);

  if (res == -1) {
    perror("getpeername() error");
    exit(EXIT_FAILURE);
  }

  char* client_ip = inet_ntoa(client_addr.sin_addr);
  int client_port = ntohs(client_addr.sin_port);

  *port = client_port;
}
