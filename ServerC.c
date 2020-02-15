/**
 * SERVERC
 * Consente ai Client di operare sul ServerM. In particolare con-
 * sente di ricevere l’elenco dei negozi virtuali, ricevere l’elenco dei pro-
 * dotti di un negozio virtuale e ricercare un prodotto in un negozio
 * virtuale.
 **/

#include "IO.h"      //Contiene le operazioni IO
#include "List.h"    //Contiene prototipi per le liste
#include "Wrapped.h" //Contiene prototipi che verificano le condizioni di uscita
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORTAM 1024 // Porta per il serverM
#define PORTA 1025  // Porta per il client

int main(int argc, char *argv[]) {
  /**Connessione con il ServerM**/
  int socketM;                   // Socket connessione ServerM
  struct sockaddr_in indirizzoM; // Indirizzo ServerM
  struct Negozio *lista = NULL;  // Lista negozi

  // Assegnazione indirizzo
  indirizzoM.sin_family = AF_INET;
  indirizzoM.sin_port = htons(PORTAM);

  /**Connessione con il Client**/
  int ascolto_fd;            // Socket Client per l'ascolto
  int connessione_fd;        // Socket Client per la connessione
  struct sockaddr_in server; // Indirizzo
  struct sockaddr_in client; // Indirizzo client
  socklen_t lunghezza;       // Lungezza indirizzo client
  pid_t pid;                 // Process id

  // Creazione socket ascolto Client
  ascolto_fd = Socket(AF_INET, SOCK_STREAM, 0);

  // Assegnazione famiglia, indirizzi, porta
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORTA);

  // Collegamento socket client alla porta 1025
  Bind(ascolto_fd, (struct sockaddr *)&server, sizeof(server));

  // Stabilisce il numero massimo di connessioni client
  Listen(ascolto_fd, 100);

  printf("\nServerC in ascolto...\n");

  while (1) {
    lunghezza = sizeof(client);

    // Accetta connessioni dal Client
    connessione_fd = Accept(ascolto_fd, (struct sockaddr *)&client, &lunghezza);

    printf("\nRicezione connessione con il client...");

    /**Connessione al serverM
     * Per avere la lista aggiornata
     **/

    socketM = Socket(AF_INET, SOCK_STREAM, 0); // Crea socket serverM

    // Connessione al serverM
    Connect(socketM, (struct sockaddr *)&indirizzoM, sizeof(indirizzoM));

    printf("\n\tConnessione con il server...");

    // Richiede al serverM la lista aggiornata
    int campo = 0;
    inviaCheck(socketM, &campo);

    printf("\n\tRicezione lista negozi e prodotti");
    lista = NULL;
    lista = ricezioneListaNegozi(socketM, lista);
    lista = ricezioneListaProdotti(socketM, lista);

    printf("\n\tDisconnessione con il server...");
    close(socketM);

    // Creazione figlio
    if ((pid = fork()) < 0) {
      perror(" fork error ");
      exit(-1);
    }

    // Figlio
    if (pid == 0) {
      int scelta = 0; // Scelta client

      // Chiude ascolto
      close(ascolto_fd);

      printf("\n\tInvio lista al client...");
      invioListaNegozi(connessione_fd, lista); // Invia lista al client

      while (1) {
        // Riceve scelta dal client
        riceviCheck(connessione_fd, &scelta);

        /**Connessione con il server M
         * per avere la lista aggiornata**/
        // Crea e connetti al serverM
        socketM = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(socketM, (struct sockaddr *)&indirizzoM, sizeof(indirizzoM));

        printf("\n\tConnessione con il server...");

        // Richiede ad M la lista aggiornata
        int campo = 0;
        inviaCheck(socketM, &campo);

        printf("\n\tRicezione lista aggiornata");
        lista = ricezioneListaNegozi(socketM, lista);
        lista = ricezioneListaProdotti(socketM, lista);

        /**Inserimento Client**/
        // Disconnessione client
        if (scelta == 0) {
          printf("\nClient disconnesso");
          break;
          exit(0);
        }

        // Ricezione e invio prodotto
        if (scelta == 1) {
          printf("\n\tRicezione prodotto");
          int id_negozio; // Id negozio
          char nome[25];  // Nome prodotto
          float prezzo;   // Prezzo prodotto

          // Riceve id e nome prodotto
          riceviIdProdotto(connessione_fd, nome, &id_negozio);

          printf("\n\tRicerca prodotto");
          // Cerca prodotto dal nome inviato dal client
          struct Prodotto *prodotto = cercaProdotto(lista, id_negozio, nome);

          if (prodotto != NULL) {
            printf("\n\tProdotto in lista");
            prezzo = prodotto->prezzo;
            inviaPrezzo(connessione_fd, &prezzo);
          } else {
            printf("\n\tProdotto non in lista");
            prezzo = -1;
            inviaPrezzo(connessione_fd, &prezzo);
          }
        }

        // Invia lista prodotti di un negozio
        if (scelta == 3) {
          printf("\n\tRicezione id negozio");
          int id;       // Id negozio
          int flag = 0; // Controllo id

          struct Negozio *copia = lista; // Copia puntatore testa lista

          // Riceve id negozio e invia i prodotti del negozio
          riceviIdNegozio(connessione_fd, &id);

          // Controlla id negozio esistente
          while (copia) {
            if (copia->id == id) {
              flag = 1;
              break;
            }
            copia = copia->next;
          }

          if (flag) {
            inviaCheck(connessione_fd, &flag);
            inviaprodotti_di(connessione_fd, lista, id);
          } else
            inviaCheck(connessione_fd, &flag);
        }
        printf("\n\tDisconnessione con il server...");
        close(socketM);
      }

      printf("\nDisconnessione con il client...");
      // Chiudi connessione con il client
      close(connessione_fd);
      exit(0);

    } else { // Padre
      close(connessione_fd);
    }
  }
  exit(0);
}