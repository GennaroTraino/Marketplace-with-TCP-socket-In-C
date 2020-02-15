#include "Wrapped.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int Socket(int family, int type, int protocol) {
  int n;

  if ((n = socket(family, type, protocol)) < 0) {
    perror("socket");
    exit(1);
  }

  return n;
}

void Connect(int socket, struct sockaddr *address, socklen_t addresslen) {
  if ((connect(socket, &(*address), addresslen)) < 0) {
    perror("Connect");
    exit(1);
  }
}

void Bind(int socket, struct sockaddr *address, socklen_t addresslen) {
  if ((bind(socket, &(*address), addresslen)) < 0) {
    perror("bind");
    exit(1);
  }
}

void Listen(int socket, int n_max) {
  if ((listen(socket, n_max)) < 0) {
    perror("listen");
    exit(1);
  }
}

int Accept(int socket, struct sockaddr *address, socklen_t *addresslen) {
  int n;

  if ((n = accept(socket, &(*address), &(*addresslen))) < 0) {
    perror("accept");
    exit(1);
  }

  return n;
}

void Inet_pton(int family, const char *source, void *destination) {
  if (inet_pton(family, source, destination) <= 0) {
    perror("address creation error");
    exit(1);
  }
}

/**RICEVI**/
void riceviCheck(int socket, int *check) {
  if (recv(socket, &(*check), sizeof(int), 0) < 0) {
    perror("Errore lettura check");
    close(socket);
    exit(0);
  }
}

void riceviProdotto(int socket, char *nome, float *prezzo, int *id) {
  if (recv(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore lettura nome");
    exit(0);
  }

  if (recv(socket, &(*prezzo), sizeof(int), 0) < 0) {
    perror("Errore lettura prezzo");
    exit(0);
  }

  if (recv(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore lettura id negozio");
    close(socket);
    exit(0);
  }
}

void riceviIdProdotto(int socket, char *nome, int *id) {
  if (recv(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore lettura nome");
    exit(0);
  }

  if (recv(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore lettura id negozio");
    close(socket);
    exit(0);
  }
}

void riceviUtente(int socket, char *nome, char *pin) {
  if (recv(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore lettura nome");
    exit(0);
  }

  if (recv(socket, &*(pin), sizeof(char) * 5, 0) < 0) {
    perror("Errore lettura nome");
    exit(0);
  }
}

void riceviProprietario(int socket, char *nome, int *id) {
  if (recv(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore lettura nome");
    exit(0);
  }

  if (recv(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore lettura negozio");
    exit(0);
  }
}

void riceviPrezzo(int socket, float *prezzo) {
  if (recv(socket, &(*prezzo), sizeof(int), 0) < 0) {
    perror("Errore lettura prezzo");
    exit(0);
  }
}

void riceviIdNegozio(int socket, int *id) {
  if (recv(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore lettura id");
    exit(0);
  }
}

void riceviUsername(int socket, char *username) {
  if (recv(socket, &(*username), sizeof(char) * 25, 0) < 0) {
    perror("Errore lettura username");
    exit(0);
  }
}

/**INVIA**/
void inviaCheck(int socket, int *check) {
  if (send(socket, &(*check), sizeof(int), 0) < 0) {
    perror("Errore scrittura check");
    exit(0);
  }
}

void inviaUtente(int socket, char *nome, char *pin) {
  if (send(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore scrittura nome");
    exit(0);
  }

  if (send(socket, &*(pin), sizeof(char) * 5, 0) < 0) {
    perror("Errore scrittura nome");
    exit(0);
  }
}

void inviaProdotto(int socket, char *nome, float *prezzo, int *id) {
  if (send(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore scrittura nome");
    exit(0);
  }

  if (send(socket, &(*prezzo), sizeof(float), 0) < 0) {
    perror("Errore scrittura prezzo");
    exit(0);
  }

  if (send(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore scrittura id negozio");
    close(socket);
    exit(0);
  }
}

void inviaProprietario(int socket, char *nome, int *id) {
  if (send(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore scrittura nome");
    exit(0);
  }

  if (send(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore scrittura negozio");
    exit(0);
  }
}

void inviaIdProdotto(int socket, char *nome, int *id) {
  if (send(socket, &(*nome), sizeof(char) * 25, 0) < 0) {
    perror("Errore scrittura nome");
    exit(0);
  }

  if (send(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore scrittura id negozio");
    close(socket);
    exit(0);
  }
}

void inviaPrezzo(int socket, float *prezzo) {
  if (send(socket, &(*prezzo), sizeof(int), 0) < 0) {
    perror("Errore scrittura prezzo");
    exit(0);
  }
}

void inviaIdNegozio(int socket, int *id) {
  if (send(socket, &(*id), sizeof(int), 0) < 0) {
    perror("Errore scrittura id");
    exit(0);
  }
}

void inviaUsername(int socket, char *username) {
  if (send(socket, &(*username), sizeof(char) * 25, 0) < 0) {
    perror("Errore scrittura username");
    exit(0);
  }
}