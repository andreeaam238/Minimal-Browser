/* Blaga Ana-Maria-Andreea, 314CB, ACS */

#include "browser.h"
#include "structures.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxCmdLength 256
#define StandardBandwidth 1024
#define NoArguments 3


int main(int argc, char *argv[]) {
  /* Se verfica daca au fost introduse datele initiale: numele fisierului de
     intrare si cel al fisierului de iesire. */

  if (argc != NoArguments) {
    return 1;
  }

  /* Se initializeaza elementele componenete ale browser-ului. */

  unsigned int Bandwidth = StandardBandwidth, NoCurrentTab = 0, check = 0;
  char *cmd = calloc(MaxCmdLength, sizeof(char)), *ptr;

  if (!cmd) {
    return 1;
  }

  ACell Tab_List = NULL, FinishedDownloadsList = NULL;
  AQ GlobalHistory = InitQ(sizeof(char) * (url_length + 1));
  if (!GlobalHistory) {
    return 1;
  }
  AQ DownloadHistory = InitQ(sizeof(Resource));
  if (!DownloadHistory) {
    DistrQ(GlobalHistory, free);
    return 1;
  }

  check = NewTab(&Tab_List, &NoCurrentTab);
  if (check) {
    DistrQ(GlobalHistory, free);
    DistrQ(DownloadHistory, free);
    return 1;
  }

  /* Se deschide fisierul de intrare si se verifica daca aceasta a reusit. */

  FILE *InFile = fopen(argv[1], "rt");
  if (!InFile) {
    DistrQ(GlobalHistory, free);
    DistrQ(DownloadHistory, free);
    DistrLG(&Tab_List, DistrTab);
    return 1;
  }

  /* Se deschide fisierul de iesire si se verifica daca aceasta a reusit. */

  FILE *OutFile = fopen(argv[2], "wt");
  if (!OutFile) {
    DistrQ(GlobalHistory, free);
    DistrQ(DownloadHistory, free);
    DistrLG(&Tab_List, DistrTab);
    fclose(InFile);
    return 1;
  }

  /* Se executa comenzile existente in fisierul de intrare. */

  while (fgets(cmd, MaxCmdLength, InFile)) {
    cmd[strlen(cmd) - 1] = '\0';
    ptr = strtok(cmd, " ");

    if (strcmp(ptr, "newtab") == 0) {
      check = NewTab(&Tab_List, &NoCurrentTab);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "set_band") == 0) {
      ptr = strtok(NULL, " ");
      
      unsigned int NewBandwidth;
      sscanf(ptr, "%u", &NewBandwidth);
      
      SetBandwidth(&Bandwidth, NewBandwidth);
    } else if (strcmp(ptr, "deltab") == 0) {
      DelTab(&Tab_List, &NoCurrentTab);
    } else if (strcmp(ptr, "change_tab") == 0) {
      ptr = strtok(NULL, " ");
      
      unsigned int NewCurrentTab;
      sscanf(ptr, "%u", &NewCurrentTab);
      
      ChangeTab(&NoCurrentTab, NewCurrentTab);
    } else if (strcmp(ptr, "print_open_tabs") == 0) {
      PrintOpenTabs(Tab_List, OutFile);
    } else if (strcmp(ptr, "goto") == 0) {
      ptr = strtok(NULL, " ");
      char *url = calloc(url_length + 1, sizeof(char));
      if(!url){
        break;
      }
      strcpy(url, ptr);
      
      check = GoTo(Tab_List, url, NoCurrentTab);
      if(check == 1){
        free(url);
        break;
      }
      
      check = IntrQ(GlobalHistory, url);
      if(check == 1){
        free(url);
        break;
      }

      free(url);

      check = Wait(DownloadHistory, &FinishedDownloadsList, 1, Bandwidth, free);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "back") == 0) {
      check = Back(Tab_List, NoCurrentTab, free, OutFile);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "forward") == 0) {
      check = Forward(Tab_List, NoCurrentTab, free, OutFile);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "history") == 0) {
      check = PrintGlobalHistory(GlobalHistory, free, OutFile);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "list_dl") == 0) {
      PrintResourcesFromCurrentPage(Tab_List, NoCurrentTab, OutFile);
    } else if (strcmp(ptr, "del_history") == 0) {
      ptr = strtok(NULL, " ");
      
      unsigned int Entries;
      sscanf(ptr, "%u", &Entries);
      
      check = DelGlobalHistory(GlobalHistory, Entries, free);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "downloads") == 0) {
      check = PrintDownloadHistory(DownloadHistory, FinishedDownloadsList, free,
                           OutFile);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "download") == 0) {
      ptr = strtok(NULL, " ");
      
      unsigned int Resource;
      sscanf(ptr, "%u", &Resource);
      
      check = DownloadResource(Tab_List, DownloadHistory, NoCurrentTab, Resource, free);
      if(check == 1){
        break;
      }
    } else if (strcmp(ptr, "wait") == 0) {
      ptr = strtok(NULL, " ");
      
      unsigned int Seconds;
      sscanf(ptr, "%u", &Seconds);
      
      check = Wait(DownloadHistory, &FinishedDownloadsList, Seconds, Bandwidth, free);
      if(check == 1){
        break;
      }
    }
  }

  /* Se elibereaza memoria si se inchid fisierele de intrare si iesire. */

  DistrQ(DownloadHistory, free);
  DistrLG(&FinishedDownloadsList, free);
  DistrLG(&Tab_List, DistrTab);
  DistrQ(GlobalHistory, free);
  free(cmd);
  fclose(InFile);
  fclose(OutFile);

  return check;
}
