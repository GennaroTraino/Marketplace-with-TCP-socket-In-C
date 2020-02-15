#include <sys/socket.h>
int Socket(int, int, int);
void Connect(int, struct sockaddr *, socklen_t);
void Bind(int, struct sockaddr *, socklen_t);
void Listen(int, int);
int Accept(int, struct sockaddr *, socklen_t *);
void Inet_pton(int, const char *, void *);

// Ricezione
void riceviCheck(int socket, int *check);
void riceviProdotto(int socket, char *nome, float *prezzo, int *id);
void riceviIdProdotto(int socket, char *nome, int *id);
void riceviUtente(int socket, char *nome, char *pin);
void riceviProprietario(int socket, char *nome, int *id);
void riceviPrezzo(int socket, float *prezzo);
void riceviIdNegozio(int socket, int *id);
void riceviUsername(int socket, char *username);

// Invio
void inviaCheck(int socket, int *check);
void inviaProdotto(int socket, char *nome, float *prezzo, int *id);
void inviaIdProdotto(int socket, char *nome, int *id);
void inviaProprietario(int socket, char *nome, int *id);
void inviaScelta(int socket, int *scelta);
void inviaUtente(int socket, char *nome, char *pin);
void inviaPrezzo(int socket, float *prezzo);
void inviaIdNegozio(int socket, int *id);
void inviaUsername(int socket, char *username);