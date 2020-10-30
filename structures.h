/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _STRUCTURES_
#define _STRUCTURES_

/* Structura lista generica + definitii utile in lucrul cu aceasta */

#define VIDA(T) (T == NULL)

typedef struct cell {
  void *info;
  struct cell *urm;
} TCell, *ACell;

/* Structura stiva generica + definitii utile in lucrul cu aceasta */

#define VF(T) (((ASt)(T))->vf)
#define DIMES(T) (((ASt)(T))->dime)
#define VIDAS(T) (T == NULL)

typedef struct stiva {
  size_t dime;
  ACell vf;
} TStiva, *ASt;

/* Structura coada generica + definitii utile in lucrul cu aceasta */

#define VIDAQ(T) (((AQ)(T))->ic == NULL && ((AQ)(T))->sc == NULL)
#define DIMEQ(T) (((AQ)(T))->dime)
#define IC(T) (((AQ)(T))->ic)
#define SC(T) (((AQ)(T))->sc)

typedef struct coada {
  size_t dime;
  ACell ic, sc;
} TCoada, *AQ;

/* Prototip functie de eliminare */

typedef void (*TFElim)(void *);

/* Prototipurile functiilor pentru lista generica */

void *AllocGenericCell(void *elem);
void DistrLG(ACell *list, TFElim ElimEl);
int InsLG(ACell *List, void *elem);


/* Prototipurile functiilor pentru stiva generica */

void *InitS(unsigned int dim);
void ResetS(void *St, TFElim ElimEl);
void DistrS(void *St, TFElim ElimEl);
int Push(void *St, void *elem);
int Pop(void *St, void *elem, TFElim ElimEl);


/* Prototipurile functiilor pentru coada generica */

void *InitQ(unsigned int dim);
void ResetQ(void *Q, TFElim ElimEl);
void DistrQ(void *Q, TFElim ElimEl);
int ExtrQ(void *Q, void *elem, TFElim ElimEl);
int IntrQ(void *Q, void *elem);
int MutaQ(void *Dest_Q, void *Source_Q, TFElim ElimEl);

#endif /* _STRUCTURES_ */
