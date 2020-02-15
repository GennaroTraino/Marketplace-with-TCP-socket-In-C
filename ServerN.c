/**
 * SERVERN
 * Consente ai ClientN di operare sul ServerM. In particolare
 * consente di creare un nuovo negozio virtuale, eliminare un negozio
 * virtuale ed aggiungere ed eliminare prodotti da un negozio virtuale.
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

#define PORTAM 1024
#define PORTA 1026

int main(int argc, char const *argv[]) {
  /*****Inserimento utenti*****/
  struct Utente *utenti = NULL; // Lista utenti

  utenti = creaListaUtenti(utenti, "utente1", "1234");
  utenti = aggiungiUtente(utenti, "utente2", "1234");
  utenti = aggiungiUtente(utenti, "utente3", "1234");
  utenti = aggiungiUtente(utenti, "utente4", "1234");
  utenti = aggiungiUtente(utenti, "utente5", "1234");

  /*****Connessione con il ServerM*****/
  int socketM;                   // Socket ServerM
  struct sockaddr_in indirizzoM; // Indirizzo ServerM

  // Assegnazione indirizzo
  indirizzoM.sin_family = AF_INET;
  indirizzoM.sin_port = htons(PORTAM);

  /*****Connessione con il Client*****/
  int ascolto_fd;            // Socket ascolto Client
  int connessione_fd;        // Socket connessione Client
  struct sockaddr_in server; // Indrizzo server
  struct sockaddr_in client; // Indrizzo client
  socklen_t lunghezza;       // Lunghezza indirizzo client
  pid_t pid;                 // Process id

  // Creazione socket Client
  ascolto_fd = Socket(AF_INET, SOCK_STREAM, 0);

  // Assegnazione famiglia, indirizzi, porta
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORTA);

  // Collegamento socket client alla porta 1025
  Bind(ascolto_fd, (struct sockaddr *)&server, sizeof(server));

  // Stabilisce il numero massimo di connessioni client
  Listen(ascolto_fd, 100);

  printf("\nServerN in ascolto...\n");
  while (1) {
    lunghezza = sizeof(client);

    // Accetta connessioni dal Client
    connessione_fd = Accept(ascolto_fd, (struct sockaddr *)&client, &lunghezza);

    printf("\n\tRicezione connessione con client...");

    // Crea figlio per assegnargli la gestione del client
    if ((pid = fork()) < 0) {
      perror(" fork error ");
      exit(-1);
    }

    // Figlio
    if (pid == 0) {
      close(ascolto_fd); // Chiudi ascolto
      char nome[25];     // Username utente
      char pin[5];       // Pin utente

      /** Verifica se l'utente è in lista **/
      printf("\n\tRicezione utente");
      // Ricevi Username e Pin dal client
      riceviUtente(connessione_fd, nome, pin);

      // Verifica login
      int check = checkUtente(nome, pin, utenti);

      if (check) {
        printf("\n\tUtente trovato nel database");
        inviaCheck(connessione_fd, &check);
      } else {
        printf("\n\tUtente non trovato");
        inviaCheck(connessione_fd, &check);
        close(connessione_fd);
        exit(0);
      }

      /** Verifica negozio proprietario **/
      int id_negozio;

      while (1) {
        printf("\n\tRicezione id negozio");

        riceviProprietario(connessione_fd, nome, &id_negozio);

        printf("\n\tConnessione con il server...");

        /**Connessione con il server M
         * per controllare se l'utente
         * proprietario del negozio scelto*/

        // Connessione con il serverM
        socketM = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(socketM, (struct sockaddr *)&indirizzoM, sizeof(indirizzoM));

        // Codice che aiuta il serverM per capire l'operazione da fare
        int server = 10;

        // Manda al serverM il nome del proprietario e id negozio
        inviaCheck(socketM, &server);
        inviaProprietario(socketM, nome, &id_negozio);

        // Ricevi da serverM il consenso con 1
        riceviCheck(socketM, &check);
        inviaCheck(connessione_fd, &check);

        if (check)
          break;
      }
      printf("\n\tDisconnessione con il server");
      close(socketM);

      /** Scelta operazione del client **/
      while (1) {
        int scelta;

        riceviCheck(connessione_fd, &scelta);

        printf("\n\tRicezione operazione del client");
        // Disconnessione client
        if (scelta == 0) {
          printf("\n\tClient disconnesso");
          break;
        }

        printf("\n\tConnessione con il server...");
        /** Connessione con il serverM**/
        socketM = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(socketM, (struct sockaddr *)&indirizzoM, sizeof(indirizzoM));

        /***Inserimento Client***/
        // Inserisce Prodotto in lista
        if (scelta == 1) {
          printf("\n\tRicezione operazione inserimento prodotto");
          char nomep[25];
          float prezzop;

          inviaCheck(socketM, &scelta);

          riceviProdotto(connessione_fd, nomep, &prezzop, &id_negozio);
          inviaProdotto(socketM, nomep, &prezzop, &id_negozio);

          riceviCheck(socketM, &check);
          inviaCheck(connessione_fd, &check);
        }

        // Elimina Prodotto in lista
        if (scelta == 2) {
          printf("\n\tRicezione operazione eliminazione prodotto");
          char nomep[25];
          int check;

          inviaCheck(socketM, &scelta);

          riceviProprietario(connessione_fd, nome, &id_negozio);
          inviaProprietario(socketM, nome, &id_negozio);

          riceviCheck(socketM, &check);
          inviaCheck(connessione_fd, &check);
        }

        // Inserisce negozio
        if (scelta == 3) {
          printf("\n\tRicezione operazione inserimento negozio");
          char username[25];
          int check;

          inviaCheck(socketM, &scelta);

          riceviUsername(connessione_fd, username);
          inviaUsername(socketM, username);

          riceviCheck(socketM, &check);
          inviaCheck(connessione_fd, &check);
        }

        // Elimina negozio
        if (scelta == 4) {
          printf("\n\tRicezione operazione eliminazione negozio");
          int negozio;
          char username[25];

          inviaCheck(socketM, &scelta);

          riceviProprietario(connessione_fd, username, &negozio);
          inviaProprietario(socketM, username, &negozio);

          int check;
          riceviCheck(socketM, &check);
          inviaCheck(connessione_fd, &check);
        }

        // Gestione altro negozio
        if (scelta == 5) {
          printf("\n\tRicezione operazione altro negozio");
          inviaCheck(socketM, &scelta);

          riceviProprietario(connessione_fd, nome, &id_negozio);
          inviaProprietario(socketM, nome, &id_negozio);

          riceviCheck(socketM, &check);
          inviaCheck(connessione_fd, &check);
        }

        // Mostra Negozi
        if (scelta == 6) {
          inviaCheck(socketM, &scelta);
          int id;
          char nome[25];

          riceviUsername(connessione_fd, nome);
          inviaUsername(socketM, nome);

          struct Negozio *temp;
          temp = ricezioneListaNegozi(socketM, temp);
          invioListaNegozi(connessione_fd, temp);
        }

        close(socketM);
      }

      printf("\n\tDisconnessione server e client...\n");
      close(socketM);
      close(connessione_fd);
      exit(0);
    }

    else { // PADRE
      close(connessione_fd);
    }
  }

  return 0;
}