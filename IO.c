#include "IO.h"   //Contiene i prototipi IO
#include "List.h" //Contiene i prototipi delle liste
#include "Wrapped.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**Invia l'id della lista dei negozi al ServerC o ServerN
 * - socketdi connessione
 * - lista negozi
 **/
void invioListaNegozi(int socket, struct Negozio *lista) {

  // Scorri lista
  while (lista) {
    int id = lista->id;

    inviaIdNegozio(socket, &id);

    lista = lista->next;
  }

  // Condizione per fermare le recv del ServerC / ServerN
  int id = -1;
  inviaIdNegozio(socket, &id);
}

/**Invia l'id della lista di negozi con i proprietari al ClientN
 * - socket di connessione
 * - lista negozi
 * - proprietario negozio
 **/
void inviaListaNegoziProprietario(int socket, struct Negozio *lista,
                                  char *proprietario) {
  // Scorri lista
  while (lista) {
    int id = lista->id;

    // Controlla proprietario
    if (strcmp(lista->proprietario, proprietario) == 0 ||
        proprietario == NULL) {
      inviaIdNegozio(socket, &id);
    }

    lista = lista->next;
  }

  // Condizione per fermare le recv del ServerC / ServerN
  int id = -1;
  inviaIdNegozio(socket, &id);
}

/** Ricezione id lista di negozi dal ServerM
 * - socket di connessione
 * - lista negozi
 **/
struct Negozio *ricezioneListaNegozi(int socket, struct Negozio *lista) {
  int id = 0; // Id negozio

  // Crea testa lista negozi
  riceviIdNegozio(socket, &id);
  lista = creaListaNegozio(lista, id);

  // Inserisce negozio
  while (1) {
    riceviIdNegozio(socket, &id);

    // Condizione per fermare la ricezione di negozi
    if (id == -1)
      break;

    // Riceve id e costruisce lista
    lista = aggiungiNegozio(lista, id);
  }
  return lista;
}

/** Invia la lista dei prodotti di tutti i negozi al ServerC o ServerN
 * - socket di connessione
 * - lista negozi
 **/
void inviaListaProdotti(int socket, struct Negozio *lista) {

  struct Prodotto *prodotto;

  // Scorre lista negozi
  while (lista) {
    prodotto = lista->pnext;

    // Scorre lista prodotti
    // Invio prodotti al ServerC/ServerN
    while (prodotto) {
      char nome[25]; // Nome prodotto
      strcpy(nome, prodotto->nome);

      float prezzo = prodotto->prezzo; // Prezzo prodotto
      int id = lista->id;              // Id negozio

      // Invio prodotto
      inviaProdotto(socket, nome, &prezzo, &id);

      prodotto = prodotto->pnext;
    }

    lista = lista->next;
  }
  // Condizioni per terminare
  char fine[25] = "fine";
  float prezzo = -1;
  int id = -1;

  inviaProdotto(socket, fine, &prezzo, &id);
}

/* Riceve la lista dei prodotti di tutti i negozi dal ServerM
 * - socket di connessione
 * - lista negozi
 **/
struct Negozio *ricezioneListaProdotti(int socket, struct Negozio *lista) {

  struct Prodotto *prodotto = NULL; // Testa lista prodotto
  char nome[25];                    // Nome prodotto
  float prezzo;                     // Prezzo prodotto
  int id;                           // Id negozio

  // Riceve nome,prezzo e id
  while (1) {
    riceviProdotto(socket, nome, &prezzo, &id);

    // Condizioni per terminare
    if (prezzo == -1 && id == -1)
      return lista;

    // Riceve il prodotto e crea lista
    prodotto = aggiungiProdotto(nome, prezzo, lista, id);
  }

  return lista;
}

/* Invia prodotti di un negozio al Client
 * - socket di connessione
 * - lista negozi
 * - id negozio
 **/
void *inviaprodotti_di(int socket, struct Negozio *lista, int id) {

  // Scorre lista
  while (lista) {
    struct Prodotto *prodotto = NULL; // Testa lista prodotto
    char nome[25];                    // Nome prodotto
    float prezzo;                     // Prezzo prodotto

    // Se il prodotto è in testa
    if (lista->id == id) {
      prodotto = lista->pnext;

      // Scorre la lista dei prodotti
      while (prodotto) {
        strcpy(nome, prodotto->nome);
        prezzo = prodotto->prezzo;

        // Invio nome
        if (send(socket, &nome, 25 * sizeof(char), 0) < 0) {
          perror("Errore in scrittura del nome del prodotto");
          exit(0);
        }

        // Invio prezzo
        inviaPrezzo(socket, &prezzo);

        prodotto = prodotto->pnext;
      }

      // Condizioni per terminare
      strcpy(nome, "fine");
      prezzo = -1;
      if (send(socket, &nome, 25 * sizeof(char), 0) < 0) {
        perror("Errore in scrittura del nome del prodotto");
        exit(0);
      }
      inviaPrezzo(socket, &prezzo);
    }
    lista = lista->next;
  }
}

/* Riceve prodotti di un negozio al Client
 * - socket di connessione
 * - lista prodotti
 **/
struct Prodotto *riceviprodotti_di(int socket, struct Prodotto *prodotti) {
  struct Prodotto *prodotto = NULL; // Testa prodotto lista
  char nome[25];                    // Nome prodotto
  float prezzo;                     // Prezzo prodotto

  // Riceve nome e prezzo di un prodotto
  // Per la testa
  if (recv(socket, &nome, 25 * sizeof(char), 0) > 0 &&
      recv(socket, &prezzo, sizeof(prezzo), 0) > 0) {
    if (prezzo == -2)
      return prodotto;
    else if (prezzo == -1)
      return prodotto;
    else
      prodotto = creaListaProdotti(nome, prezzo, prodotto);
  }

  // Per il resto
  struct Prodotto *ptemp = prodotto; // Copia testa prodotto lista
  while (recv(socket, &nome, 25 * sizeof(char), 0) > 0 &&
         recv(socket, &prezzo, sizeof(prezzo), 0) > 0) {
    if (prezzo == -1)
      break;
    else if (prezzo == -2)
      return prodotto;
    else
      ptemp = aggiungiProdottoLista(prodotto, nome, prezzo);
  }

  return prodotto;
}
