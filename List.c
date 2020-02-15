#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Crea lista negozi
 * - lista negozi
 * - id negozi
 * Utilizzata da ServerM inizialmente per creare la lista
 **/
struct Negozio *creaListaNegozio(struct Negozio *testa, int id) {

  // Alloca memoria per il negozio
  struct Negozio *negozio = malloc(sizeof(struct Negozio));
  // Inserisce informazioni
  negozio->id = id;
  negozio->next = NULL;
  negozio->pnext = NULL;

  return negozio;
}

/** Crea lista di prodotti
 * - nome prodotto
 * - prezzo prodotto
 * - lista negozi
 * Primo inserimento del prodotto per creare la lista
 * Utilizzata dal ServerC primo inserimento di un prodotto
 **/
struct Prodotto *creaListaProdotti(char *nome, float prezzo,
                                   struct Prodotto *testa) {
  // Alloca memoria per un nuovo prodotto
  struct Prodotto *nuovo = malloc(sizeof(struct Prodotto));
  // Alloca memoria per stringa
  nuovo->nome = malloc(sizeof(char) * strlen(nome) + 1);
  // Inserisce informazioni
  strcpy(nuovo->nome, nome);
  nuovo->prezzo = prezzo;
  nuovo->pnext = NULL;

  return nuovo;
}

/**Crea lista negozi
 * - lista negozi
 * - id negozi
 * Utilizzata da ServerM inizialmente per creare la lista
 **/
struct Utente *creaListaUtenti(struct Utente *testa, char *username,
                               char *pin) {

  // Alloca memoria nuovo utente
  struct Utente *utente = malloc(sizeof(struct Utente));

  // Inserisce informazioni
  utente->username = malloc(sizeof(char) * strlen(username) + 1);
  strcpy(utente->username, username);

  utente->pin = malloc(sizeof(char) * strlen(pin) + 1);
  strcpy(utente->pin, pin);

  utente->unext = NULL;

  return utente;
}

/** Aggiunge un prodotto alla lista dei negozi
 * - nome prodotto
 * - prezzo prodoto
 * - testa lista negozi
 * - id negozio
 * Inserisce il prodotto nella la lista dei negozi
 * Ogni lista dei negozi avrà una sua la lista dei prodotti
 * Utilizzata dal ServerM
 **/
void *aggiungiProdotto(char *nome, float prezzo, struct Negozio *testa,
                       int id) {
  struct Prodotto *nuovo = malloc(sizeof(struct Prodotto));
  // Allocando memoria per un nuovo prodotto
  nuovo->nome = malloc(sizeof(char) * strlen(nome) + 1);
  // Inserisce informazioni
  strcpy(nuovo->nome, nome);
  nuovo->prezzo = prezzo;

  // Scorre lista negozi
  while (testa) {
    // Se il negozio corrisponde all'id, bisogna inserire il prodotto
    if (testa->id == id) {
      nuovo->pnext = testa->pnext;
      testa->pnext = nuovo;
    }
    // Prossimo negozio
    testa = testa->next;
  }
  return testa;
}

/** Aggiunge negozio alla lista di negozi
 * - lista negozi
 * - id negozio
 * Ritorna la testa della lista dei negozi
 * Utilizzata dal ServerM per aggiungere inizialmente dei negozi
 * Utilizzata dal ServerN
 **/
struct Negozio *aggiungiNegozio(struct Negozio *testa, int id) {

  // Alloca memoria per il prodotto
  struct Negozio *negozio = malloc(sizeof(struct Negozio));
  // Inserisce informazioni
  negozio->id = id;
  negozio->next = testa;
  negozio->pnext = NULL;

  return negozio;
}

/** Aggiunge prodotto alla lista dei prodotti
 * - lista prodotti
 * - nome prodotto
 * - prezzo prodotto
 * Utilizzata dal ServerC per creare la lista dei prodotti da mandare al Client
 **/
struct Prodotto *aggiungiProdottoLista(struct Prodotto *testa, char *nome,
                                       float prezzo) {

  // Alloca memoria nuovo prodotto
  struct Prodotto *nuovo = malloc(sizeof(struct Prodotto));

  nuovo->nome = malloc(sizeof(char) * strlen(nome) + 1);
  // Inserisce informazioni
  strcpy(nuovo->nome, nome);
  nuovo->prezzo = prezzo;

  nuovo->pnext = testa->pnext;
  testa->pnext = nuovo;

  return nuovo;
}

/** Aggiunge utente
 * - lista utenti
 * - username
 * - pin
 * Ritorna la testa della lista degli utenti
 * Utilizzata dal ServerN
 **/
struct Utente *aggiungiUtente(struct Utente *testa, char *username, char *pin) {
  // Alloca memoria per il prodotto
  struct Utente *utente = malloc(sizeof(struct Utente));

  // Inserisce informazioni
  utente->username = malloc(sizeof(char) * strlen(username) + 1);
  strcpy(utente->username, username);

  utente->pin = malloc(sizeof(char) * strlen(pin) + 1);
  strcpy(utente->pin, pin);

  utente->unext = testa;

  return utente;
}

/**Elimina un negozio dalla lista dei negozi
 * - lista negozi
 * - id negozi
 * Funzione utilizzata dal ServerM
 **/
struct Negozio *eliminaNegozio(struct Negozio *testa, int id) {
  struct Negozio *precedente = testa;
  struct Negozio *copia = testa;

  // Se il negozio da eliminare è in testa
  if (testa->id == id) {
    testa = testa->next;
    return testa;
  }

  // Se il negozio da eliminare non è in testa
  // Scorri lista prodotti
  while (copia) {
    if (copia->id == id)
      precedente->next = copia->next;

    precedente = copia;
    copia = copia->next;
  }
  return testa;
}

/**Elimina un dato prodotto dalla lista dei negozi
 * - lista negozi
 * - id negozi
 * - nome prodotto
 * Funzione utilizzata dal ServerM
 **/
struct Prodotto *eliminaProdotto(struct Negozio *testa, int id, char *nome) {
  struct Prodotto *precedente;

  // Scorri lista negozi
  while (testa) {
    // Se l'id del negozio corrisponde con quello cercato
    if (testa->id == id) {
      // Prendi la testa della lista dei prodotti
      struct Prodotto *elimina = testa->pnext;

      // Se il prodotto da eliminare è in testa
      if (strcmp(elimina->nome, nome) == 0) {
        testa->pnext = elimina->pnext;
        return testa->pnext;
      }

      // Se il prodotto da eliminare non è in testa
      // Scorri lista prodotti
      while (elimina) {
        if (strcmp(elimina->nome, nome) == 0) {
          precedente->pnext = elimina->pnext;

          // Prossimo prodotto
          return testa->pnext;
        }
        precedente = elimina;
        elimina = elimina->pnext;
      }
    }

    testa = testa->next;
  }

  return NULL;
}

/** Cerca un prodotto in un negozio
 * - lista negozi
 * - id negozio
 * - nome prodotto
 * Ritorna il prodotto relativo alla lista di un negozio
 * Utilizzata dal ServerC / ServerM per cercare un prodotto e ritornarlo
 **/
struct Prodotto *cercaProdotto(struct Negozio *negozio, int id, char *nome) {
  // Scorri lista negozi
  while (negozio) {
    // Se il negozio corrisponde all'id
    if (negozio->id == id) {
      struct Prodotto *prodotto = negozio->pnext;
      // Scorri lista prodotti
      while (prodotto) {
        // Se il prodotto corrisponde al nome
        if (strcmp(prodotto->nome, nome) == 0)
          return prodotto;
        // Prodotto successivo
        prodotto = prodotto->pnext;
      }
    }
    // Negozio successivo
    negozio = negozio->next;
  }

  return NULL;
}

/**Stampa la lista dei negozi con i relativi prodotti
 * - lista negozi
 **/
void stampaLista(struct Negozio *negozio) {
  struct Prodotto *prodotto;
  // Scorre lista negozi
  while (negozio) {
    printf("Negozio n° %d\n", negozio->id);

    if (negozio->proprietario != NULL)
      printf("\tProprietario %s\n", negozio->proprietario);

    // Prende la lista dei prodotti
    prodotto = negozio->pnext;

    // Scorri lista prodotti
    while (prodotto) {
      printf("\tProdotto: %s\n\t\tPrezzo: %.2f\n", prodotto->nome,
             prodotto->prezzo);

      // Prossimo prodotto
      prodotto = prodotto->pnext;
    }
    // Prossimo negozio
    negozio = negozio->next;
  }
}

/**Stampa la lista degli utenti
 * - lista utenti
 **/
void stampaUtenti(struct Utente *utenti) {
  // Scorre lista utenti
  while (utenti) {
    printf("Username %s\n\tPin %s\n", utenti->username, utenti->pin);

    // Prossimo negozio
    utenti = utenti->unext;
  }
}

/**Stampa la lista degli utenti
 * - lista prodotti
 **/
void stampaProdotti(struct Prodotto *prodotti) {
  while (prodotti) {
    printf("%s\n", prodotti->nome);
    prodotti = prodotti->pnext;
  }
}

/** Assegna il proprietario ad un negozio:
 * - username
 * - id negozio
 * - lista negozi
 **/
void assegnaProprietario(char *proprietario, int id_negozio,
                         struct Negozio *lista) {
  while (lista) {
    if (lista->id == id_negozio) {
      lista->proprietario = malloc(sizeof(char) * strlen(proprietario) + 1);
      strcpy(lista->proprietario, proprietario);
    }
    lista = lista->next;
  }
}

/** Verifica identità proprietario
 * - username
 * - pin
 * - lista utenti
 **/
int checkUtente(char *nome, char *pin, struct Utente *lista) {
  while (lista) {
    if (strcmp(nome, lista->username) == 0 && strcmp(pin, lista->pin) == 0) {
      return 1;
    }
    lista = lista->unext;
  }
  return 0;
}

/** Verifica accesso utente al negozio
 * - username
 * - lista negozi
 * - id
 **/
int checkProprietario(char *nome, struct Negozio *lista, int id) {
  while (lista) {
    if (strcmp(nome, lista->proprietario) == 0 && lista->id == id)
      return 1;
    lista = lista->next;
  }
  return 0;
}