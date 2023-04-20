<h1 align="center">Server TCP</h1>

---

<h3 align="center">Le server Perroquet en C</h3>
<p align="center">Projet open-source dans la création d'un server / client TCP en C ⚡</p>

<p align="center">
<img src="https://img.shields.io/badge/stability-work_in_progress-lightgrey.svg?style=flat-square" alt="work in progress badge">
</p>


Dans la réalisation d'un server TCP en TP de Réseau à la faculté de Montpellier. L'écriture de ce repo à pour objectif de s'initier au réseau
et de développer certaines fonctionnalités.


## 🚀 Getting Started
### Code CLIENT
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (int argc, char ** argv) {
  int res, client_socket;
  struct sockaddr_in client_addr, server_addr;
  printf("Début de la vie du client\n");

  if (argc != 3) {
    printf("ERROR: il faut renseigner deux paramètres <ip> <port>\n");
    exit(EXIT_FAILURE);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);

  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (client_socket < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(port);

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  server_addr.sin_port = htons(port);

  int lgA = sizeof (struct sockaddr_in);
  inet_pton(AF_INET, ip, &(client_addr.sin_addr));

  res = connect(client_socket, (struct sockaddr*) &server_addr, lgA);

  if (res != 0) {
    printf("Erreur lors de la connexion\n");
    exit(EXIT_FAILURE);
  }

  char msg[1024];
  char buffer[1024];

  while (1) {
    strcpy(msg, "");
    strcpy(buffer, "");

    printf("Entrez votre message\n");
    fgets(msg, sizeof (msg), stdin);

    res = send(client_socket, msg, sizeof (msg), 0);

    if (res < 0) {
      printf("Erreur de send\n");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    if (strcmp(msg, "end\n") == 0) {
      printf("LE SERVER S'ETEINT VIA LA DEMANDE DU CLIENT\n");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    res = recv(client_socket, &buffer, sizeof (buffer), 0);

    if (res < 0) {
      perror("Erreur de recv\n");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    if (strcmp(buffer, "end\n") == 0) {
      printf("LE SERVER S'ETEINT VIA LA DEMANDE DU SERVER\n");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    printf("BUFFER: %s\n", buffer);
  }
}
```

## ✨ Idées
- Possibilité d'écrire côté client un message custom pour le server
- Récupérer et afficher côté server les informations du client
