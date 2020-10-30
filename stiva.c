/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include "structures.h"
#include <stdlib.h>

/* Functie pentru initializarea unei stive.
 
 * Se aloca memorie pentru structura de tip stiva.
 
 * Se seteaza dimensiunea elementelor din stiva.
 
 * Varful stivei este setat pe NULL(stiva vida). */

void *InitS(unsigned int dim) {
  ASt St = (ASt)calloc(1, sizeof(TStiva));

  if (!St) {
    return NULL;
  }

  St->dime = dim;
  St->vf = NULL;

  return (void *)St;
}

/* Functie pentru intrducerea unui element in stiva.
 
 * Se aloca memorie pentru noua celula introdusa in stiva si pentru 
   campul info al acesteia.
 
 * Se copiaza informatia dorita in campul info al celulei si se introduce
   acesta in stiva. */

int Push(void *St, void *elem) {
  ACell aux_cell = (ACell)calloc(1, sizeof(TCell));
  if (!aux_cell) {
    return 1;
  }

  aux_cell->info = calloc(1, DIMES(St));
  if (!aux_cell->info) {
    free(aux_cell);
    return 1;
  }

  memcpy(aux_cell->info, elem, DIMES(St));
  aux_cell->urm = VF(St);
  VF(St) = aux_cell;
  return 0;
}

/* Functie pentru extragerea unui element din stiva.
 
 * Se copiaza informatia din celula ce urmeaza a fi eliminata la adresa dorita.

 * Se elimina celula din stiva. */

int Pop(void *St, void *elem, TFElim ElimEl) {
  if (VIDAS(St)) {
    return 1;
  }

  memcpy(elem, (VF(St))->info, DIMES(St));

  ACell aux_cell = VF(St);
  VF(St) = aux_cell->urm;
  ElimEl(aux_cell->info);
  free(aux_cell);

  return 0;
}

/* Functie pentru resetarea unei stive.

 * Se seteaza varful stivei la NULL(stiva vida).

 * Se elibereaza memoria ocupata de toate elmentele din stiva. */

void ResetS(void *St, TFElim ElimEl) {
  ACell list = VF(St);
  DistrLG(&list, ElimEl);
  VF(St) = NULL;
}

/* Functie pentru distrugerea unei stive.

 * Se apeleaza functia ResetS.

 * Se elibereaza memoria ocupata de stiva. */

void DistrS(void *St, TFElim ElimEl) {
  ResetS(St, ElimEl);
  free(St);
}