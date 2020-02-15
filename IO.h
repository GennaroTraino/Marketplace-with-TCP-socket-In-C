#include "List.h"
/* SCAMBIO DI LISTA NEGOZI E PRODOTTI TRA SERVER */
void inviaListaProdotti(int socket, struct Negozio *lista);
void invioListaNegozi(int socket, struct Negozio *lista);
void inviaListaNegoziProprietario(int socket, struct Negozio *lista,
                                  char *proprietario);
struct Negozio *ricezioneListaNegozi(int socket, struct Negozio *testa);
struct Negozio *ricezioneListaProdotti(int socket, struct Negozio *lista);

/* SCAMBIO LISTA DI SOLI PRODOTTI DA MOSTRARE AL CLIENT */
void *inviaprodotti_di(int socket, struct Negozio *lista, int id);
struct Prodotto *riceviprodotti_di(int socket, struct Prodotto *prodotti);
