/**
 * CLIENT
 * Consente all’utente di interagire con i negozi virtuali memo-
 * rizzati sul ServerM usando il ServerC come tramite. In particolare,
 * consente all’utente di inserire i prodotti contenuti in diversi negozi
 * virtuali in una lista di acquisti e di visualizzare la lista di acquisti.
 */

#include "IO.h"      // Contiene le operazioni IO
#include "Wrapped.h" // Contiene i prototipi che verificano le condizioni di uscita
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORTA 1025

void main() {
  int socket;                   // Socket connessione serverC
  struct sockaddr_in indirizzo; // Indirizzo

  printf("\e[2J\e[H\t\tBENVENUTO NEL MARKETPLACE");
  // Creazione socket
  socket = Socket(AF_INET, SOCK_STREAM, 0);

  // Assegnazione indirizzo, porta, indirizzo
  indirizzo.sin_family = AF_INET;
  indirizzo.sin_port = htons(PORTA);
  indirizzo.sin_addr.s_addr = htonl(INADDR_ANY);

  // Connessione al serverC
  Connect(socket, (struct sockaddr *)&indirizzo, sizeof(indirizzo));

  struct Negozio *carrello = NULL; // Carello della spesa

  carrello = ricezioneListaNegozi(socket, carrello); // Ricevi lista dei negozi
  putchar('\n');

  int scelta; // Scelta client
  int flag;   // Flag controllo

  while (1) {
    printf("Scegli un operazione\n[0]Esci\n"
           "[1]Inserisci prodotto nel carello\n"
           "[2]Visualizza carello\n"
           "[3]Visualizza prodotti Negozio\n->");

    scanf("%d", &scelta);

    // Invia scelta al ServerC
    inviaCheck(socket, &scelta);

    printf("\n");
    int id_negozio;                  // Id negozio
    char nome[25];                   // Nome prodotto
    float prezzo;                    // Prezzo prodotto
    struct Prodotto *lista_prodotti; // Lista prodotti

    switch (scelta) {
    // Esci
    case 0:
      printf("\e[2J\e[H");
      printf("Disconnessione...\n");
      exit(0);
      break;

    // Inserisci prodotto nel carello
    case 1:
      printf("\e[2J\e[H");

      printf("Inserisci negozio da cui vuoi acquistare: ");
      scanf("%d", &id_negozio);
      printf("Inserisci il nome del prodotto: ");
      scanf("%s", nome);

      // Invio id negozio e nome al ServerC
      inviaIdProdotto(socket, nome, &id_negozio);

      // Ricevi prezzo prodotto dal ServerC
      riceviPrezzo(socket, &prezzo);

      // Inserimento nel carello
      if (prezzo >= 0)
        aggiungiProdotto(nome, prezzo, carrello, id_negozio);
      else {
        fflush(stdin);
        printf("Il prodotto inserito non esiste\n");
        sleep(3);
      }
      printf("\e[2J\e[H");
      break;

    // Visualizza carello
    case 2:
      printf("\e[2J\e[H");
      stampaLista(carrello);
      break;

    // Visualizza prodotti di un negozio
    case 3:
      printf("\e[2J\e[H");
      printf("Scegli il Negozio -> ");
      scanf("%d", &id_negozio);

      // Invia id al ServerC
      inviaIdNegozio(socket, &id_negozio);

      // Ricevi prodotti di un negozio dal ServerC
      riceviCheck(socket, &flag);

      if (flag) {
        lista_prodotti = riceviprodotti_di(socket, lista_prodotti);
        stampaProdotti(lista_prodotti);
      } else {
        printf("\nNegozio non disponibile\n");
        sleep(3);
      }

      putchar('\n');
      break;

    default:
      printf("Riprova\n\n");
      break;
    }
  }

  close(socket);
  exit(0);
}
