#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"

int main () {
  printf("Veuillez indiquez le numéro de port de l'application\n");
  char resp[80];
  scanf("%s", resp);

  int port = atoi(resp);

  printf("L'application a demarré sur le port %d\n", port);

  int MaSocket = socket(PF_INET, SOCK_STREAM, 0);
  int res;

  if (MaSocket == -1) {
    printf("ERROR: socket\n");
    close(MaSocket);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in Server = create_server(port);

  int lgA = sizeof(struct sockaddr_in);

  res = bind(MaSocket, (struct sockaddr*) &Server, sizeof(Server));

  if (res == -1) {
    printf("ERROR: bind");
    close(MaSocket);
    exit(EXIT_FAILURE);
  }

  res = listen(MaSocket, 7);

  if (res < 0) {
    printf("ERROR: listen");
    close(MaSocket);
    exit(EXIT_FAILURE);
  }


  struct sockaddr_in client;
  socklen_t lg = sizeof(struct sockaddr_in);

  while (1) {
    int len = 0;
    char buffer[256];
    char msg[256];
    int SocketClient = accept(MaSocket, (struct sockaddr*) &client, &lg);
    int portClient;

    get_client_info(SocketClient, &portClient);

    printf("Client port: %d\n", portClient);
    len = recv(SocketClient, buffer, sizeof(buffer), 0);

    if (len == -1) {
      close(SocketClient);
      close(MaSocket);
      printf("Erreur lors de la réception des données\n");
      exit(EXIT_FAILURE);
    }

    printf("Message reçu du client: %s\n", buffer);
    printf("Écrivez un message que vous voulez envoyer au client\n");
    //fgets(msg, sizeof (msg), stdin);
    scanf("%s", msg);

    if (strcmp(msg, "end") == 0) {
      char* msg_resp = "Le serveur a décidé de prendre sa retraite";
      printf("%lu", sizeof (msg_resp));
      send(SocketClient, msg_resp, sizeof (msg_resp), 0);

      close(SocketClient);
      break;
    }

    printf("REPONSE ENVOYE: %s\n", msg);

    send(SocketClient, msg, sizeof (msg), 0);

    close(SocketClient);
  }

  close(MaSocket);

  return 0;
}
