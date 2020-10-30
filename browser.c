/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include "browser.h"
#include <stdlib.h>

/* Functie pentru schimbarea valorii vitezei de transfer. */

void SetBandwidth(unsigned int *Bandwidth, unsigned int NewValue) {
  *Bandwidth = NewValue;
}

/* Functie pentru schimbarea tab-ului curent. */

void ChangeTab(unsigned int *CurrentTab, unsigned int NewValue) {
  *CurrentTab = NewValue;
}

/* Functie pentru eliberarea memoriei ocupata de o structura de tip 
   AWebPage. */

void DelWebPage(void *elem) {
  AWebPage WebPage = (AWebPage)elem;

  free(WebPage->resources);
  free(WebPage);
}

/* Functie pentru alocarea de memorie pentru o structura de tip AWebPage.

 * Sunt generate resursele corespunzatoare paginii web pe baza adresei URL a
   acesteia. */

void *NewWebPage(void *URL) {
  AWebPage elem = (AWebPage)calloc(1, sizeof(TWebPage));
  if (!elem) {
    return NULL;
  }

  strcpy(elem->url, URL);
  int NoResources = 0;
  elem->resources = get_page_resources(URL, &NoResources);
  elem->num_res = NoResources;

  return (void *)elem;
}

/* Functie pentru alocarea de memorie pentru o structura de tip Atab. 

 * Se seteaza pagina curenta ca NULL si se initializeaza stivele de back,
   respectiv forward. 

 * Apoi se insereaza noul tab in lista de taburi. */

int NewTab(void *TList, unsigned int *CurrentTab) {
  ATab elem = (ATab)calloc(1, sizeof(TTab));
  if (!elem) {
    return 1;
  }
  elem->current_page = NULL;

  elem->back_stack = InitS(sizeof(TWebPage));
  if (elem->back_stack == NULL) {
    free(elem);
  }

  elem->forward_stack = InitS(sizeof(TWebPage));
  if (elem->back_stack == NULL) {
    DistrS(elem->back_stack, free);
    free(elem);
  }

  int res = InsLG(TList, elem);

  if (res == 0) {
    ACell last = *((ACell *)TList);
    int NewTab = 0;
    while (last->urm != NULL) {
      last = last->urm;
      NewTab++;
    }
    ChangeTab(CurrentTab, NewTab);
  } else {
    DistrS(elem->back_stack, free);
    DistrS(elem->forward_stack, free);
    free(elem);
  }

  return res;
}

/* Functie pentru eliberarea memoriei ocupata de de un element de tip ATab. */

void DistrTab(void *elem) {
  ATab Tab = (ATab)(elem);
  if (Tab->current_page != NULL) {
    DelWebPage(Tab->current_page);
  }
  DistrS(Tab->forward_stack, DelWebPage);
  DistrS(Tab->back_stack, DelWebPage);
  free(elem);
}

/* Functie pentru eliminarea ultimului tab deschis din lista de taburi. 

 * Se cauta ultimul tab din lista de taburi.

 * Daca tab-ul care este inchis este cel curent, atunci tab-ul curent va 
   fi setat la ultimul tab deschis. */

void DelTab(void *TList, unsigned int *CurrentTab) {
  ACell *L = (ACell *)TList;
  ACell TabList = *L, AntTab = NULL;
  unsigned int NoTab = 0;

  while (TabList->urm != NULL) {
    AntTab = TabList;
    TabList = TabList->urm;
    NoTab++;
  }

  ATab Tab = (ATab)(TabList->info);

  DistrTab(Tab);
  free(TabList);

  if (NoTab == *CurrentTab) {
    ChangeTab(CurrentTab, NoTab - 1);
  }

  if (AntTab) {
    AntTab->urm = NULL;
  } else {
    *L = NULL;
  }
}

/* Functie pentru afisarea URL-urilor paginilor curente din taburile deschise.

 * In cazul in care intr-un tab nu este deschisa o pagina web se va afisa
   "empty". */

void PrintOpenTabs(void *TList, FILE *Out) {
  ACell TabList = (ACell)TList;
  int indx = 0;

  while (TabList) {
    ATab Tab = (ATab)(TabList->info);
    fprintf(Out, "(%d: ", indx);

    if (Tab->current_page != NULL) {
      fprintf(Out, "%s)\n", Tab->current_page->url);
    } else {
      fprintf(Out, "%s)\n", "empty");
    }

    indx++;
    TabList = TabList->urm;
  }
}

/* Functie pentru a gasi adresa tabului curent din lista de taburi. */

void *GetCurrentTab(void *TList, int CurrentTab){
  ACell TabList = (ACell)TList;
  int Indx = 0;

  while (TabList && Indx != CurrentTab) {
    Indx++;
    TabList = TabList->urm;
  }

  return (void *)TabList;
}

/* Functie pentru a permite accesarea paginii anterioare. 

 * Se cauta tab-ul curent, apoi se scoate elementul(pagina web) din varful
   stivei de back. 

 * In cazul in care stiva de back este goala se va afisa un mesaj de eroare,
   altfel pagina curenta din tab va in introdusa in stiva de forward, iar 
   pagina curenta va fi setata la elementul extras anterior. */

int Back(void *TList, int NoCurrentTab, TFElim ElimEl, FILE *Out) {
  ACell TabList = (ACell)GetCurrentTab(TList, NoCurrentTab);
  int res = 0;

  ATab Tab = (ATab)(TabList->info);

  if (VF(Tab->back_stack) == NULL) {
    fprintf(Out, "%s\n", "can't go back, no pages in stack");
  } else {
    AWebPage WebPage = (AWebPage)calloc(1, sizeof(TWebPage));
    if (!WebPage) {
      return 1;
    }

    res = Pop(Tab->back_stack, WebPage, ElimEl);
    if (res == 1) {
      return 1;
    }

    res = Push(Tab->forward_stack, Tab->current_page);
    if (res == 1) {
      return 1;
    }

    free(Tab->current_page);
    Tab->current_page = WebPage;
  }
  return 0;
}

/* Functie pentru a permite accesarea paginii urmatoare.

 * Se cauta ta-bul curent, apoi se scoate elementul(pagina web) din varful 
   stivei de forward. 

 * In cazul in care stiva de forward este goala se va afisa un mesaj de eroare,
   altfel pagina curenta din tab va in introdusa in stiva de back, iar pagina
   curenta va fi setata la elementul extras anterior. */

int Forward(void *TList, int NoCurrentTab, TFElim ElimEl, FILE *Out) {
  ACell TabList = (ACell)GetCurrentTab(TList, NoCurrentTab);
  int res = 0;

  ATab Tab = (ATab)(TabList->info);

  if (VF(Tab->forward_stack) == NULL) {
    fprintf(Out, "%s\n", "can't go forward, no pages in stack");
  } else {
    AWebPage WebPage = (AWebPage)calloc(1, sizeof(TWebPage));
    if (!WebPage) {
      return 1;
    }

    res = Pop(Tab->forward_stack, WebPage, ElimEl);
    if (res == 1) {
      return 1;
    }

    Push(Tab->back_stack, Tab->current_page);
    if (res == 1) {
      return 1;
    }

    free(Tab->current_page);
    Tab->current_page = WebPage;
  }
  return 0;
}

/* Functie pentru afisarea istoricului global al browser-ului. 

 * Se extrage pe rand cate un element din coada istoricului global, se 
   afiseaza continutul acestuia(o adresa URL), apoi acesta este introdus 
   intr-o coada auxiliara. 

 * In final sunt mutate elementele din coada auxiliara in coada istoricului
   global. */

int PrintGlobalHistory(void *HistoryQueue, TFElim ElimEl, FILE *Out) {
  int res = 0;

  AQ AuxQ = InitQ(DIMEQ(HistoryQueue));
  if (!AuxQ) {
    return 1;
  }

  void *AuxCell = calloc(1, DIMEQ(HistoryQueue));
  if (!AuxCell) {
    return 1;
  }

  while (!VIDAQ(HistoryQueue)) {
    if ((IC(HistoryQueue)->info) != NULL) {
      fprintf(Out, "%s\n", (char *)(IC(HistoryQueue)->info));
    }
    res = ExtrQ(HistoryQueue, AuxCell, ElimEl);

    if (res == 0) {
      IntrQ(AuxQ, AuxCell);
    }

    if (res == 1) {
      ElimEl(AuxCell);
      DistrQ(AuxQ, ElimEl);
    }
  }
  res = MutaQ(HistoryQueue, AuxQ, ElimEl);

  ElimEl(AuxCell);
  DistrQ(AuxQ, ElimEl);

  return res;
}

/* Functie pentru stergerea unui numar de "intrari" din istoricul global al
   browser-ului. 

 * Astfel, se extrage pe rand cate un element din coada istoricului global 
   si este eliberata memoria ocupata de acesta pana cand se atinge numarul de 
   eliminari dorit. */

int DelGlobalHistory(void *HistoryQueue, int entries, TFElim ElimEl) {
  if (entries == 0) {
    ResetQ(HistoryQueue, ElimEl);
  } else {
    while (entries) {
      void *AuxCell = calloc(1, DIMEQ(HistoryQueue));
      if (!AuxCell) {
        return 1;
      }
      ExtrQ(HistoryQueue, AuxCell, ElimEl);
      ElimEl(AuxCell);
      entries--;
    }
  }
  return 0;
}

/* Functie pentru afisarea resurselor disponibile in pagina curenta. 

 * Se cauta tab-ul curent in lista de taburi, iar in cazul in care pagina 
   curenta din tab-ul respectiva exista si are resurse sunt afisate acestea. */

void PrintResourcesFromCurrentPage(void *TList, int CurrentTab, FILE *Out) {
  unsigned int i;
  ACell TabList = (ACell)GetCurrentTab(TList, CurrentTab);
  AWebPage CurrentPage = ((ATab)(TabList->info))->current_page;

  if (CurrentPage == NULL) {
    return;
  }

  for (i = 0; i < CurrentPage->num_res; i++) {
    fprintf(Out, "[%d - \"%s\" : %ld]\n", i, CurrentPage->resources[i].name,
            CurrentPage->resources[i].dimension);
  }
}

/* Functie pentru afisarea istoricului de descarcari al browserului. 
 * Mai intai sunt afisare descarcarile incomplete(stocate in coada de 
   descarcari). 

 * Astfel,se extrage cate un element din coada, sunt afisate numele 
   descarcarii, cati bytes mai sunt de descarcat si cati bytes are resursa
   respectiva, apoi acest element este introdus intr-o coada auxiliara.

 * Dupa ce coada de descarcari incomplete este golita elementele din coada 
   auxiliara sunt mutate in aceasta.

 * Apoi sunt afisate elementele din lista de descarcari complete, astfel: numele
   resursei si starea in care se afla, adica "completed". */

int PrintDownloadHistory(void *DownloadHistoryQueue,
                         void *FinishedDownloadsList, TFElim ElimEl,
                         FILE *Out) {

  AQ UnfinishedDownloads = (AQ)DownloadHistoryQueue;
  ACell FinishedDownloads = (ACell)FinishedDownloadsList;

  AQ AuxQ = InitQ(DIMEQ(DownloadHistoryQueue));
  if (!AuxQ) {
    return 1;
  }

  void *AuxCell = calloc(1, DIMEQ(DownloadHistoryQueue));
  if (!AuxCell) {
    DistrQ(AuxQ, ElimEl);
    return 1;
  }

  while (!VIDAQ(UnfinishedDownloads)) {
    ACell QStart = IC(UnfinishedDownloads);
    Resource *CurrentResource = (Resource *)(QStart->info);

    fprintf(Out, "[\"%s\" : %ld/%ld]\n", CurrentResource->name,
            CurrentResource->dimension - CurrentResource->currently_downloaded,
            CurrentResource->dimension);

    int res = ExtrQ(UnfinishedDownloads, AuxCell, ElimEl);

    if (res == 0) {
      res = IntrQ(AuxQ, AuxCell);
    }

    if (res == 1) {
      ElimEl(AuxCell);
      DistrQ(AuxQ, ElimEl);
      return 1;
    }
  }

  int res = MutaQ(DownloadHistoryQueue, AuxQ, ElimEl);

  DistrQ(AuxQ, ElimEl);

  ElimEl(AuxCell);

  while (FinishedDownloads) {
    Resource *CurrentResource = (Resource *)(FinishedDownloads->info);
    fprintf(Out, "[\"%s\" : completed]\n", CurrentResource->name);
    FinishedDownloads = FinishedDownloads->urm;
  }

  return res;
}

/* Functie pentru introducerea unui element(a unei resurse) in coada de 
   prioritati a descarcarilor. 

 * Coada este structurata astfel: elementul cu cei mai putini bytes ramasi
   de descarcat vai fi primul din coada etc.

 * Astfel pentru a introduce un nou element in acesata coada sunt extrase mai 
   intai din coada elementele care au valoarea ramasa de descarcat mai mica 
   decat acesta si introduse intr-o coada auxiliara.

 * Cand regula de mai sus nu mai este respectata se insereaza elementul in 
   coada auxiliara si se continua extragerea din coada initiala si introducerea
   in cea auxiliara. 
   
 * In final elementele din coada auxiliara sunt mutate in 
   coada initiala. */

int IntrPriorityQ(void *PriorityQ, void *ResourceToAdd, TFElim ElimEl) {
  int Added = 1, res = 0;
  AQ DownloadHistoryQueue = (AQ)PriorityQ;
  Resource *Res = (Resource *)ResourceToAdd;
  unsigned long ToDownload = Res->dimension - Res->currently_downloaded;

  AQ AuxQ = InitQ(DIMEQ(DownloadHistoryQueue));
  if (!AuxQ) {
    return 1;
  }

  void *AuxCell = calloc(1, DIMEQ(DownloadHistoryQueue));
  if (!AuxCell) {
    return 1;
  }

  while (!VIDAQ(DownloadHistoryQueue)) {
    ACell QStart = IC(DownloadHistoryQueue);
    Resource *CurrentResource = (Resource *)(QStart->info);
    unsigned long ToCompare =
        CurrentResource->dimension - CurrentResource->currently_downloaded;

    if (ToDownload <= ToCompare && Added == 1) {
      res = IntrQ(AuxQ, ResourceToAdd);
      Added = 0;
    }

    if (res == 0) {
      res = ExtrQ(DownloadHistoryQueue, AuxCell, ElimEl);
    }

    if (res == 0) {
      res = IntrQ(AuxQ, AuxCell);
    }

    if (res == 1) {
      DistrQ(AuxQ, ElimEl);
      ElimEl(AuxCell);
      return 1;
    }
  }

  if (Added == 1) {
    res = IntrQ(AuxQ, ResourceToAdd);
  }

  if (res == 0) {
    res = MutaQ(PriorityQ, AuxQ, ElimEl);
  }

  DistrQ(AuxQ, ElimEl);
  ElimEl(AuxCell);

  return res;
}

/* Functie pentru descarcarea unei resurse dintr-o pagina.
 
 * Se cauta tab-ul curent in lista de taburi.
 
 * In cazul in care in tab-ul respectiv exista o pagina deschisa resursele din
   aceasta vor fi introduse in coada de descarcari a browserului. */

int DownloadResource(void *TList, void *DownloadHistoryQueue,
                     int NoCurrentTab, int ResourceIndx, TFElim ElimEl) {
  ACell TabList = (ACell)GetCurrentTab(TList, NoCurrentTab);
  ATab Tab = (ATab)(TabList->info);

  AWebPage CurrentPage = (AWebPage)(Tab->current_page);
  if (CurrentPage == NULL) {
    return 0;
  }

  return IntrPriorityQ(DownloadHistoryQueue,
                       &(CurrentPage->resources[ResourceIndx]), ElimEl);
}

/* Functie pentru a realiza o trecere fictiva a timpului, permitand 
   descarcarilor sa inainteze.
 
 * Capacitatea de descarcare este calculata ca produs dintre viteza de transfer
   si timp.
 
 * Cat timp exista elemente in coada de descarcari a browserului si capacitate
   de descarcare se vor descarca resurse.
 
 * In cazul in care o resursa s-a terminat de descarcat aceasta este scoasa din
   coada si introdusa in lista de descarcari finalizate. */

int Wait(void *DownloadHistoryQueue, void *FinishedDownloadsList,
         unsigned int Seconds, unsigned int Bandwidth, TFElim ElimEl) {
  unsigned long Download = Bandwidth * Seconds;
  int res = 0;

  while (Download && !VIDAQ(DownloadHistoryQueue)) {
    ACell QStart = IC(DownloadHistoryQueue);
    Resource *CurrentResource = (Resource *)(QStart->info);
    unsigned long ToDownload =
        CurrentResource->dimension - CurrentResource->currently_downloaded;

    if (ToDownload >= Download) {
      CurrentResource->currently_downloaded += Download;
      Download = 0;
    } else {
      Download -= ToDownload;
      CurrentResource->currently_downloaded += ToDownload;
    }

    ToDownload =
        CurrentResource->dimension - CurrentResource->currently_downloaded;

    if (ToDownload == 0) {
      void *AuxCell = calloc(1, DIMEQ(DownloadHistoryQueue));
      if (!AuxCell) {
        return 1;
      }

      res = ExtrQ(DownloadHistoryQueue, AuxCell, ElimEl);
      if (res == 0) {
        InsLG(FinishedDownloadsList, AuxCell);
      }
      if (res == 1) {
        ElimEl(AuxCell);
        return 1;
      }
    }
  }
  return 0;
}

/* Functie pentru accesarea unei pagini web in tab-ul curent.
 
 * Se cauta tab-ul curent in lista de taburi.
 
 * Daca exista deja o pagina deschisa in tab-ul curent acesta este introdusa
   in stiva de back, iar stiva de forward este golita.
 
 * Se elibereaza memoria ocupata de pagina curenta si se creeaza o noua pagina
   curenta pe baza URL-ului acesteia. */

int GoTo(void *TList, char *URL, int CurrentTab) {
  ACell TabList = (ACell)GetCurrentTab(TList, CurrentTab);
  ATab Tab = (ATab)(TabList->info);

  if (Tab->current_page != NULL) {
    int res = Push(Tab->back_stack, Tab->current_page);
    if (res == 1) {
      return 1;
    }
    if (VF(Tab->forward_stack) != NULL) {
      ResetS(Tab->forward_stack, DelWebPage);
    }

    free(Tab->current_page);
  }

  Tab->current_page = (AWebPage)NewWebPage(URL);
  if (Tab->current_page == NULL) {
    return 1;
  }

  return 0;
}