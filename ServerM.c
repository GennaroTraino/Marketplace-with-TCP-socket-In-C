/**
 * SERVER M
 * Mantiene la lista dei negozi virtuali e dei prodotti di ogni negozio
 * virtuale. Interagisce con ServerN e ServerC.
 **/

#include "IO.h"      //Contiene le operazioni IO
#include "List.h"    //Contiene i prototipi per creare liste
#include "Wrapped.h" // Contiene i prototipi che verificano le condizioni di uscita
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORTA 1024

int main(int argc, char *argv[]) {
  struct Negozio *negozi;    // Lista negozi
  int ascolto_fd;            // Socket ascolto
  int connessione_fd;        // Socket connessione
  struct sockaddr_in server; // Indirizzo server
  struct sockaddr_in client; // Indirizzo client
  socklen_t lunghezza;       // Lunghezza indirizzo client
  pid_t pid;                 // Process id

  // Lista negozi
  negozi = creaListaNegozio(negozi, 1);
  negozi = aggiungiNegozio(negozi, 2);
  negozi = aggiungiNegozio(negozi, 3);
  negozi = aggiungiNegozio(negozi, 4);
  negozi = aggiungiNegozio(negozi, 5);
  negozi = aggiungiNegozio(negozi, 6);
  assegnaProprietario("utente1", 1, negozi);
  assegnaProprietario("utente2", 2, negozi);
  assegnaProprietario("utente3", 3, negozi);
  assegnaProprietario("utente4", 4, negozi);
  assegnaProprietario("utente5", 5, negozi);
  assegnaProprietario("utente1", 6, negozi);

  struct Prodotto *prodotti = NULL; // Lista prodotti
  // Negozio 1
  prodotti = aggiungiProdotto("acqua", 1.5, negozi, 1);
  prodotti = aggiungiProdotto("sale", 2.0, negozi, 1);
  prodotti = aggiungiProdotto("batteria", 3.0, negozi, 1);
  // Negozio 2
  prodotti = aggiungiProdotto("casse", 13.52, negozi, 2);
  prodotti = aggiungiProdotto("hard disk", 24.90, negozi, 2);
  // Negozio 3
  prodotti = aggiungiProdotto("biscotti", 3.52, negozi, 3);
  prodotti = aggiungiProdotto("nutella", 2.90, negozi, 3);
  prodotti = aggiungiProdotto("armadio", 499.99, negozi, 3);
  // Negozio 4
  prodotti = aggiungiProdotto("cuffie", 20.0, negozi, 4);
  prodotti = aggiungiProdotto("PC", 1500.0, negozi, 4);
  // Negozio 5
  prodotti = aggiungiProdotto("Playstation 4", 399.99, negozi, 5);
  prodotti = aggiungiProdotto("Iphone X", 1390.90, negozi, 5);
  prodotti = aggiungiProdotto("Xbox One", 399.99, negozi, 5);
  // Negozio 6
  prodotti = aggiungiProdotto("divano", 1290.90, negozi, 6);
  prodotti = aggiungiProdotto("specchio", 39.99, negozi, 6);

  // Creazione socket per ServerC / ServerN
  ascolto_fd = Socket(AF_INET, SOCK_STREAM, 0);

  // Assegnazione famiglia, indirizzi, porta
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORTA);

  // Collegamento socket alla porta 1024
  Bind(ascolto_fd, (struct sockaddr *)&server, sizeof(server));

  // Stabilisce il numero massimo di connessioni client
  Listen(ascolto_fd, 100);

  printf("\nServerM in ascolto...\n");

  while (1) {
    int check;
    lunghezza = sizeof(client);

    // Accetta connessioni dal ServerC / ServerN
    connessione_fd = Accept(ascolto_fd, (struct sockaddr *)&client, &lunghezza);

    printf("\nRicezione connessione con il server");

    // Ricevi numero dal Server N / Server C
    int server;
    riceviCheck(connessione_fd, &server);

    // Aggiornamento lista ServerC
    if (server == 0) {
      invioListaNegozi(connessione_fd, negozi);
      inviaListaProdotti(connessione_fd, negozi);
    }

    // Inserire prodotto in lista ServerN
    if (server == 1) {
      printf("\n\tControllo prodotto in lista negozi");
      char nomep[25];
      float prezzop;
      int idp;

      riceviProdotto(connessione_fd, nomep, &prezzop, &idp);

      if (cercaProdotto(negozi, idp, nomep) == NULL) {
        printf("\n\tProdotto non in lista");
        printf("\n\tInserimento prodotto in lista\n");
        aggiungiProdotto(nomep, prezzop, negozi, idp);

        check = 1;
        inviaCheck(connessione_fd, &check);

      } else {
        printf("\n\tProdotto già in lista");
        printf("\n\tAggiornamento prezzo\n");
        eliminaProdotto(negozi, idp, nomep);
        aggiungiProdotto(nomep, prezzop, negozi, idp);
      }
    }

    // Elimina prodotto dalla lista ServerN
    if (server == 2) {
      printf("\n\tControllo prodotto in lista negozi");
      char nomep[25];
      int idp;

      riceviIdProdotto(connessione_fd, nomep, &idp);

      if (cercaProdotto(negozi, idp, nomep) != NULL) {
        printf("\n\tProdotto in lista");
        printf("\n\tEliminazione prodotto\n");
        eliminaProdotto(negozi, idp, nomep);
        check = 1;
        inviaCheck(connessione_fd, &check);
      } else {
        printf("\n\tProdotto non in lista\n");
        check = 0;
        inviaCheck(connessione_fd, &check);
      }
    }

    // Inserisce un negozio ServerC
    if (server == 3) {
      printf("\n\tRicezione utente");
      char username[25];
      int i = 0;

      riceviUsername(connessione_fd, username);

      // Calcola numero negozi
      struct Negozio *copia = negozi; // Copia puntatore

      while (copia) {
        if (copia->id > i)
          i = copia->id;
        copia = copia->next;
      }

      i++;

      // Inserisci negozi in lista
      printf("\n\tInserimento negozio in lista\n");
      negozi = aggiungiNegozio(negozi, i);
      assegnaProprietario(username, i, negozi);

      inviaCheck(connessione_fd, &i);
    }

    // Elimina un negozio ServerC
    if (server == 4) {
      printf("\n\tRicezione proprietario negozio");
      int id;
      char nome[25];

      riceviProprietario(connessione_fd, nome, &id);

      check = checkProprietario(nome, negozi, id);

      if (check) {
        printf("\n\tNegozio in lista");
        printf("\n\tEliminazione negozio\n");
        negozi = eliminaNegozio(negozi, id);
        inviaCheck(connessione_fd, &check);

      } else {
        printf("\n\tNegozio non in lista\n");
        check = 0;
        inviaCheck(connessione_fd, &check);
      }
    }

    // Controllo proprietario del negozio
    if (server == 5) {
      int id;
      char nome[25];

      riceviProprietario(connessione_fd, nome, &id);

      check = checkProprietario(nome, negozi, id);
      inviaCheck(connessione_fd, &check);
    }

    // Invio lista negozi gestiti dall'utente collegato
    if (server == 6) {
      char nome[25];
      int id;

      riceviUsername(connessione_fd, nome);
      inviaListaNegoziProprietario(connessione_fd, negozi, nome);
    }

    // Controlla negozio utente ServerN
    else if (server == 10) {
      printf("\n\tControllo proprietario negozio");
      int id;
      char nome[25];

      riceviProprietario(connessione_fd, nome, &id);

      check = checkProprietario(nome, negozi, id);
      inviaCheck(connessione_fd, &check);
    }

    printf("\nDisconnessione con il server...");
    close(connessione_fd);
  }

  exit(0);
}