/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include "structures.h"
#include "utils.h"

/* Lungimea maxima a unei adrese URL(exista in teste URL-uri cu 21 de caractere
   efective + 1 caracter NULL termator = 22 caractere). */

#define url_length 21

#ifndef _Browser_
#define _Browser_

/* Structura pentru pagina web */

typedef struct {
  char url[url_length + 1];
  unsigned int num_res;
  Resource *resources;
} TWebPage, *AWebPage;

/* Structura pentru tab */

typedef struct {
  AWebPage current_page;
  void *back_stack;
  void *forward_stack;
} TTab, *ATab;

/* Prototipurile functiilor folosite in implementarea browser-ului */

void SetBandwidth(unsigned int *Bandwidth, unsigned int NewValue);
void ChangeTab(unsigned int *CurrentTab, unsigned int NewValue);

void DelWebPage(void *elem);
void DistrTab(void *elem);
void DelTab(void *TList, unsigned int *CurrentTab);
void PrintOpenTabs(void *TList, FILE *Out);
void PrintResourcesFromCurrentPage(void *TList, int CurrentTab, FILE *Out);
void *NewWebPage(void *URL);
void *GetCurrentTab(void *TList, int CurrentTab);

int NewTab(void *TList, unsigned int *CurrentTab);
int Back(void *TList, int NoCurrentTab, TFElim ElimEl, FILE *Out);
int Forward(void *TList, int NoCurrentTab, TFElim ElimEl, FILE *Out);
int AddToGlobalHistory(void *HistoryQueue, void *URL);
int PrintGlobalHistory(void *HistoryQueue, TFElim ElimEl, FILE *Out);
int DelGlobalHistory(void *HistoryQueue, int entries, TFElim ElimEl);
int PrintDownloadHistory(void *DownloadHistoryQueue,
                         void *FinishedDownloadsList, TFElim ElimEl, FILE *Out);
int IntrPriorityQ(void *PriorityQ, void *ResourceToAdd, TFElim ElimEl);

int DownloadResource(void *TList, void *DownloadHistoryQueue,
                      int NoCurrentTab, int ResourceIndx, TFElim ElimEl);
int Wait(void *DownloadHistoryQueue, void *FinishedDownloadsList,
         unsigned int Seconds, unsigned int Bandwidth, TFElim ElimEl);
int GoTo(void *TList, char *URL, int CurrentTab);

#endif /* _Browser_ */
