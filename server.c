#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

int main (int argc, char** argv) {
  if (argc != 3) {
    printf("ERROR FDP MET LE PORT STP");
    exit(0);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);

  printf("%s\n", ip);
  printf("%d\n", port);

  int MaSocket = socket(PF_INET, SOCK_STREAM, 0);
  int res;

  if (MaSocket == -1) {
    printf("ERRORRROROOROR MaSOCKET\n");
    close(MaSocket);
    exit(EXIT_FAILURE);
  }

  printf("MaSocket %d\n", MaSocket);

  struct sockaddr_in Server;

  Server.sin_family = AF_INET;
  Server.sin_addr.s_addr = INADDR_ANY;
  Server.sin_port = htons(port);

  int lgA = sizeof(struct sockaddr_in);
  printf("Ok");
  res = bind(MaSocket, (struct sockaddr*) &Server, sizeof(Server));

  if (res == -1) {
    printf("ERROR DANS LE RECU DE LA REPONSE");
    close(MaSocket);
    exit(0);
  }
  printf("Bind Ok");
  res = listen(MaSocket, 7);

  if (res < 0) {
    printf("ERROR CHEH");
    exit(EXIT_FAILURE);
  }

  printf("listen Ok");
  struct sockaddr_in client;
  socklen_t lg = sizeof(struct sockaddr_in);

  for (int i = 0; i< 3; i++) {
    int len = 0;
    char buffer[128];
    int SocketClient = accept(MaSocket, (struct sockaddr*) &client, &lg);

    len = recv(SocketClient, buffer, 128* sizeof(char), 0);

    if (len == -1) {
      close(SocketClient);
      close(MaSocket);
      printf("Erreur lors de la réception des données\n");
      exit(EXIT_FAILURE);
    }


    printf("Message reçu du client: %s\n", buffer);

    send(SocketClient, buffer, len * sizeof(char), 0);

    printf("Messag envoyé\n");

    close(SocketClient);
  }

  close(MaSocket);
}