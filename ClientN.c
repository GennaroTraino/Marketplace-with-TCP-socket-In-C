/**
 * CLIENTN
 * Consente al negoziante di gestire i propri negozi virtuali (ed
 * i relativi prodotti) memorizzati sul ServerM, usando il ServerN come
 * tramite. Ogni negoziante può gestire più negozi virtuali.
 **/
#include "IO.h"      //Contiene i prototipi IO
#include "Wrapped.h" // COntiene i prototipi che verificano le condizioni di uscita
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORTA 1026

int main() {
  int socket;                   // Socket connessione serverN
  struct sockaddr_in indirizzo; // Indirizzo

  printf("\t\tBENVENUTO NEL MARKETPLACE\n");
  // Creazione socket
  socket = Socket(AF_INET, SOCK_STREAM, 0);

  // Assegnazione indirizzo, porta, famiglia
  indirizzo.sin_family = AF_INET;
  indirizzo.sin_port = htons(PORTA);
  indirizzo.sin_addr.s_addr = htonl(INADDR_ANY);

  // Connessione ServerN
  Connect(socket, (struct sockaddr *)&indirizzo, sizeof(indirizzo));

  char username[25]; // Username utente
  char pin[5];       // Pin utente
  int check;         // Flag di controllo
  int negozio;       // Id negozio

  // Inserimento utente
  printf("Inserisci username: ");
  scanf("%s", username);
  printf("\nInserisci pin: ");
  scanf("%s", pin);

  inviaUtente(socket, username, pin);

  // Riceve consenso dal Server N
  riceviCheck(socket, &check);
  if (!check) {
    printf("Credenziali sbagliate, disconnessione in corso...\n");
    close(socket);
    exit(0);
  }

  int scelta;

  while (1) {
    printf("\nSu quale negozio vuoi operare: ");
    scanf("%d", &negozio);

    inviaProprietario(socket, username, &negozio);
    riceviCheck(socket, &check);

    if (check)
      break;
    printf("\nQuesto negozio non è di tua proprietà\n");
  }

  int flag = 1;         // Flag controlla eliminazione negozio corrente
  struct Negozio *list; // Lista negozi gestiti da utente collegato

  while (1) {
    printf("\e[2J\e[H \nI tuoi Negozi:\n");

    // Forza scelta per ricevere la lista di negozi
    scelta = 6;
    inviaCheck(socket, &scelta);
    inviaUsername(socket, username);

    list = ricezioneListaNegozi(socket, list);
    stampaLista(list);

    printf("\n");

    printf("Scegli un operazione\n"
           "[0]Esci\n"
           "[1]Inserisci prodotto\n"
           "[2]Elimina prodotto\n"
           "[3]Inserisci negozio\n"
           "[4]Elimina negozio\n"
           "[5]Gestisci altro negozio\n");

    // Se il negozio corrente non è stato eliminato
    if (flag) {
      printf("-> ");
      scanf("%d", &scelta);
    }
    // Se il negozio corrente è stato eliminato forza la scelta 5
    else {
      scelta = 5;
      flag = 1;
    }

    inviaCheck(socket, &scelta);

    printf("\n");

    char prodotto[25];   // Nome prodotto
    float prezzo;        // Prezzo prodotto
    int elimina_negozio; // Variabile controllo per l'eliminazione negozio
                         // corrente

    // Menu
    switch (scelta) {

    // Esci
    case 0:
      printf("\e[2J\e[H");
      printf("Disconnessione...\n");

      // Chiudi connessione con il serverN
      close(socket);
      exit(0);
      break;

    // Inserimento prodotto
    case 1:
      printf("\e[2J\e[H");
      printf("Inserisci nome prodotto: ");
      scanf("%s", prodotto);
      printf("\nInserisci prezzo del prodotto: ");
      scanf("%f", &prezzo);

      inviaProdotto(socket, prodotto, &prezzo, &negozio);

      printf("\e[2J\e[H");
      // Riceve consenso
      riceviCheck(socket, &check);

      if (check) {
        printf("Prodotto aggiunto \n");
        sleep(3);
      } else {
        printf("Prodotto non aggiunto \n");
        sleep(3);
      }
      break;

    // Eliminazione prodotto
    case 2:
      printf("\e[2J\e[H");
      printf("Inserisci nome prodotto da eliminare: ");
      scanf("%s", prodotto);

      inviaIdProdotto(socket, prodotto, &negozio);

      printf("\e[2J\e[H");
      // Riceve consenso
      riceviCheck(socket, &check);

      if (check) {
        printf("Prodotto eliminato\n");
        sleep(3);
      } else {
        printf("Prodotto non trovato\n");
        sleep(3);
      }
      break;

    // Inserimento negozio
    case 3:
      inviaUsername(socket, username);
      printf("\e[2J\e[H");

      // Riceve consenso
      riceviCheck(socket, &check);
      if (check) {
        printf("Negozio aggiunto\n");
        sleep(3);
      } else {
        printf("Negozio non aggiunto\n");
        sleep(3);
      }
      printf("\e[2J\e[H");
      break;

    // Eliminazione negozio
    case 4:
      printf("\e[2J\e[H");

      printf("Inserisci negozio da eliminare: ");
      scanf("%d", &elimina_negozio);

      // Se il negozio che si sta eliminando è quello che si sta gestendo
      if (elimina_negozio == negozio)
        flag = 0; // Rifai la scelta

      inviaProprietario(socket, username, &elimina_negozio);

      printf("\e[2J\e[H");
      // Riceve consenso
      riceviCheck(socket, &check);

      if (check) {
        printf("Negozio eliminato\n");
        sleep(3);

      } else {
        printf("Negozio non trovato\n");
        sleep(3);
      }
      break;

    // Gestione altro negozio
    case 5:
      printf("\e[2J\e[H");
      printf("Su quale negozio vuoi operare -> ");
      scanf("%d", &negozio);

      inviaProprietario(socket, username, &negozio);

      // Riceve consenso
      riceviCheck(socket, &check);

      if (check) {
        printf("\nNegozio selezionato cambiato\n");
        sleep(3);
        break;
      } else {
        printf("\nQuesto negozio non è di tua proprietà\n");
        flag = 0; // Rifai la scelta
        sleep(3);
        break;
      }

    default:
      printf("\nRiprova\n\n");
      break;
    } // switch
  }   // while

  exit(0);
}