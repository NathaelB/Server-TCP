#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"

void launch ();

int main () {
  launch();
  return 0;
}



void launch () {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  int res;

  printf("Veuillez indiquez le numéro de port de l'application\n");

  char resp[80];
  scanf("%s", resp);
  int port = atoi(resp);

  server_socket = socket(PF_INET, SOCK_STREAM, 0);

  printf("L'application a demarré sur le port %d\n", port);

  if (server_socket == -1) {
    printf("ERROR: socket\n");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  server_addr = create_server(port);

  /**
   * BIND
   */
  res = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));

  if (res == -1) {
    printf("ERROR: bind");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  /**
   * LISTEN
   */
  res = listen(server_socket, 7);

  if (res < 0) {
    printf("ERROR: listen");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  int client;
  socklen_t client_len;
  int i = 0;
  printf("Serveur en attente de connexions...\n");

  char msg[1024];
  char buffer[1024];

  socklen_t lg = sizeof(struct sockaddr_in);
  client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &lg);
  int port_client;

  get_client_info(client_socket, &port_client);
  client_addr = create_server(port_client);

  while (1) {
    strcpy(msg, "");
    strcpy(buffer, "");


    res = recv(client_socket, &buffer, sizeof (buffer), 0);

    if (res < 0) {
      close(client_socket);
      close(server_socket);
      printf("Erreur lors de la réception des données\n");
      exit(EXIT_FAILURE);
    }

    if (strcmp(buffer, "end\n") == 0) {
      printf("LE SERVER S'ETEINT VIA LA DEMANDE DU CLIENT\n");
      close(server_socket);
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    printf("Réponse du client: %s\n",buffer);

    printf("Entrée votre message\n");
    fgets(msg, sizeof (msg), stdin);

    res = send(client_socket,msg,sizeof(msg),0);
    printf("VOICI LE MESSAGE QUE TU ENNVOIES: %s\n", msg);

    if (res < 0) {
      printf("ERROR: send");
      close(server_socket);
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    if (strcmp(msg, "end\n") == 0) {
      printf("LE SERVER S'ETEINT VIA LA DEMANDE DU SERVER\n");
      close(server_socket);
      close(client_socket);
      exit(EXIT_FAILURE);
    }
  }

  close(server_socket);
  close(client_socket);
}