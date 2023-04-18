//
// Created by Nathael Bonnal on 17/04/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (int argc, char ** argv) {
  if (argc != 3) {
    printf("ERROR: il faut renseigner deux paramètres <ip> <port>\n");
    exit(EXIT_FAILURE);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);

  int MaBR = socket(AF_INET, SOCK_STREAM, 0);
  int res;

  if (MaBR < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in Server;

  Server.sin_family = AF_INET;
  Server.sin_port = htons(port);

  int lgA = sizeof (struct sockaddr_in);
  inet_pton(AF_INET, ip, &(Server.sin_addr));

  res = connect(MaBR, (struct sockaddr*) &Server, lgA);
  if (res != 0) {
    printf("Erreur lors de la connexion\n");
    exit(EXIT_FAILURE);
  }

  char* m = "Hello nathael";
  char buffer[128];

  res = send(MaBR, m, 20, 0);
  if (res < 0) {
    printf("ERROR: impossible d'envoyer le message\n");
    exit(EXIT_FAILURE);
  }

  res = recv(MaBR, buffer, 128*sizeof (int), 0);

  if (res < 0) {
    printf("Error while receiving server's msg\n");
    exit(EXIT_FAILURE);
  }

  printf("Réponse du sever %s\n", buffer);

  close(MaBR);

  return 1;
}