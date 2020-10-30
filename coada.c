/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include "structures.h"
#include <stdlib.h>

/* Functie pentru initializarea unei cozi.
 
 * Se aloca memorie pentru structura de tip coada.
 
 * Se seteaza dimensiunea elementelor din coada.
 
 * Inceputul si sfarsitul cozii sunt setate pe NULL(coada vida). */

void *InitQ(unsigned int dim) {
  AQ Q = (AQ)calloc(1, sizeof(TCoada));

  if (!Q) {
    return NULL;
  }

  Q->dime = dim;
  Q->ic = NULL;
  Q->sc = NULL;

  return (void *)Q;
}

/* Functie pentru intrducerea unui element in coada.
 
 * Se aloca memorie pentru noua celula introdusa in coada si pentru 
   campul info al acesteia.
 
 * Se copiaza informatia dorita in campul info al celulei si se introduce
   acesta in coada. */

int IntrQ(void *Q, void *elem) {
  ACell aux_cell = (ACell)calloc(1, sizeof(TCell));
  if (!aux_cell) {
    return 1;
  }

  aux_cell->info = calloc(1, DIMEQ(Q));
  if (!aux_cell->info) {
    free(aux_cell);
    return 1;
  }

  memcpy(aux_cell->info, elem, DIMEQ(Q));
  aux_cell->urm = NULL;

  if (VIDAQ(Q)) {
    SC(Q) = aux_cell;
    IC(Q) = aux_cell;
  } else {
    SC(Q)->urm = aux_cell;
    SC(Q) = aux_cell;
  }

  return 0;
}

/* Functie pentru extragerea unui element din coada.
 
 * Se copiaza informatia din celula ce urmeaza a fi eliminata la adresa dorita.

 * Se elimina celula din coada. */

int ExtrQ(void *Q, void *elem, TFElim ElimEl) {
  if (VIDAQ(Q)) {
    return 1;
  }

  memcpy(elem, IC(Q)->info, DIMEQ(Q));

  ACell aux_cell = IC(Q);
  IC(Q) = aux_cell->urm;
  ElimEl(aux_cell->info);
  free(aux_cell);
  if (IC(Q) == NULL) {
    SC(Q) = NULL;
  }

  return 0;
}

/* Functie pentru resetarea unei cozi.

 * Se seteaza inceputul si sfarsitul cozii la NULL(coada vida).

 * Se elibereaza memoria ocupata de toate elmentele din coada. */

void ResetQ(void *Q, TFElim ElimEl) {
  ACell list = IC(Q);
  IC(Q) = NULL;
  SC(Q) = NULL;
  DistrLG(&list, ElimEl);
}

/* Functie pentru distrugerea unei cozi.

 * Se apeleaza functia ResetQ.

 * Se elibereaza memoria ocupata de coada. */

void DistrQ(void *Q, TFElim ElimEl) {
  ResetQ(Q, ElimEl);
  free(Q);
}

/* Functie pentru mutarea elementelor dintr-o coada in alta.

 * Se extrag elemente din coada sursa si se introduc in coada destinatie pana
   cand coada sursa devine vida. */

int MutaQ(void *Dest_Q, void *Source_Q, TFElim ElimEl) {
  while (!VIDAQ(Source_Q)) {
    void *AuxCell = calloc(1, DIMEQ(Source_Q));
    if (!AuxCell) {
      return 1;
    }
    ExtrQ(Source_Q, AuxCell, ElimEl);
    IntrQ(Dest_Q, AuxCell);
    ElimEl(AuxCell);
  }
  return 0;
}
