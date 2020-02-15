#ifndef LIST_H_
#define LIST_H_

/**
 * Contiene informazioni di:
 * - id
 * - un puntatore che punta al prossimo negozio
 * - un puntatore che punta al prossimo prodotto
 **/
struct Negozio {
  int id;
  struct Negozio *next;
  struct Prodotto *pnext;
  char *proprietario;
};

/**
 * Contiene informazioni di:
 * - nome
 * - prezzo
 * - un puntatore che punta al prossimo prodotto
 **/
struct Prodotto {
  char *nome;
  float prezzo;
  struct Prodotto *pnext;
};

/**
 * Contiene informazioni di:
 * - username
 * - pin
 * - un puntatore che punta al prossimo utente
 **/
struct Utente {
  char *username;
  char *pin;
  struct Utente *unext;
};

struct Negozio *creaListaNegozio(struct Negozio *testa, int id);
struct Utente *creaListaUtenti(struct Utente *testa, char *nome, char *pin);
struct Prodotto *creaListaProdotti(char *nome, float prezzo,
                                   struct Prodotto *testa);

struct Negozio *aggiungiNegozio(struct Negozio *testa, int id);
struct Utente *aggiungiUtente(struct Utente *testa, char *username, char *pin);
void *aggiungiProdotto(char *nome, float prezzo, struct Negozio *testa, int id);
struct Prodotto *aggiungiProdottoLista(struct Prodotto *testa, char *nome,
                                       float prezzo);

struct Negozio *eliminaNegozio(struct Negozio *negozio, int id);
struct Prodotto *cercaProdotto(struct Negozio *negozio, int id, char *nome);
struct Prodotto *eliminaProdotto(struct Negozio *negozio, int id, char *nome);

void stampaLista(struct Negozio *negozio);
void stampaProdotti(struct Prodotto *prodotti);
void stampaUtenti(struct Utente *utenti);

void assegnaProprietario(char *proprietario, int id_negozio,
                         struct Negozio *lista);

int checkUtente(char *nome, char *pin, struct Utente *lista);
int checkProprietario(char *nome, struct Negozio *lista, int id);
#endif
