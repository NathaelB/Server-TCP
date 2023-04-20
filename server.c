//
// Created by Nathael Bonnal on 18/04/2023.
//

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "server.h"

/**
 * Create basic Server
 * @param port
 * @return
 */
struct sockaddr_in create_server (int port) {
  struct sockaddr_in Server;

  memset(&Server, 0, sizeof (Server));
  Server.sin_family = AF_INET;
  Server.sin_addr.s_addr = inet_addr("127.0.0.1");
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

void *handle_client (void *arg) {
  int sockfd = *(int *)arg;
  char buffer[1024] = {0};

  printf("SOCKFD %d\n", sockfd);
  ssize_t res = read(sockfd, buffer, sizeof(buffer));

  if (res < 0) {
    perror("ERROR: socket read");
    exit(EXIT_FAILURE);
  }

  const char* hello = "Hello form server";

  res = send(sockfd, hello, strlen(hello), 0);

  if (res < 0) {
    perror("ERROR: send info to client");
    close(sockfd);
    pthread_exit(NULL);
    exit(EXIT_FAILURE);
  }

  read(sockfd, buffer, 1024);
  printf("Client message: %s\n", buffer);

  close(sockfd);
  pthread_exit(NULL);
}

void *connection_handler (void *socket_desc) {
  printf("CH TEST\n");
  int res;
  int client_socket = *(int *)socket_desc;
  char buffer[1024] = {0};
  ssize_t read_size;

  read_size = read(client_socket, buffer, 1024);
  while (read_size > 0) {
    printf("Message reçu : %s\n", buffer);

    res = write(client_socket, "Bonjour, je suis le serveur", strlen("Bonjour, je suis le serveur"));
    if (res == -1) {
      perror("Erreur lors de l'envoi de la réponse");
      break;
    }

    memset(buffer, 0, 1024);
  }
  printf("%lu\n", read_size);

  if (read_size == 0) {
    printf("Le client a fermé la connexion\n");
  } else if (read_size == -1) {
    perror("Erreur lors de la lecture de la socket");
  }

  res = close(client_socket);
  if (res == -1) {
    perror("Erreur lors de la fermeture de la socket");
  }
  free(socket_desc);

  return NULL;
}