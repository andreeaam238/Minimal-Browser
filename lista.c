/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include "structures.h"
#include <stdlib.h>

/* Functie pentru alocarea unei celule generice.
 
 * Se aloca memorie pentru celula generica si se seteaza campul info al 
   acesteia la adresa dorita. */

void *AllocGenericCell(void *elem){
  ACell aux_cell = (ACell)calloc(1, sizeof(TCell));
  if (!aux_cell) {
    return NULL;
  }
  aux_cell->info = elem;
  aux_cell->urm = NULL;
  return (void *)aux_cell;
}

/* Functie pentru distrugerea unei liste generice.

 * Se elimina toate celulele din lista pana cand lista devine vida. */

void DistrLG(ACell *List, TFElim ElimEl) {
  ACell aux_cell;
  while (*List) {
    aux_cell = *List;
    *List = aux_cell->urm;
    ElimEl(aux_cell->info);
    free(aux_cell);
  }
}

/* Functie pentru inserarea unei celule la sfarsitul unei liste generice.

 * Se aloca memorie pentru noua celula apoi se insereaza aceasta la sfarsitul
   listei. */

int InsLG(ACell *List, void *elem) {
  ACell aux_cell = (ACell)AllocGenericCell(elem);
  if (!aux_cell) {
    return 1;
  }

  if (*List == NULL) {
    *List = aux_cell;
  } else {
    ACell p = *List, ant = NULL;
    while (p) {
      ant = p;
      p = p->urm;
    }
    ant->urm = aux_cell;
  }
  return 0;
}
