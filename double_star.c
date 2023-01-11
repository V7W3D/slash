#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "split_string.h"
#include "double_star.h"
#include "star.h"

static int double_star_aux(char * ref, char * target, char * chemin, char ** result, int *len_result){ 
  struct dirent * entry;
  char *ref_tmp = malloc(PATH_MAX), *ref_chemin = malloc(PATH_MAX);
  DIR * dirp1;
  DIR * d;
  
  if((dirp1 = opendir(ref)) == NULL){
    perror("opendir");
    exit(1);
  }


  if (strlen(chemin) >1 && strlen(target)>1)
    snprintf(ref_chemin, PATH_MAX, "%s/%s/%s", ref, chemin, target);
  else if (strlen(chemin) >1)
    snprintf(ref_chemin, PATH_MAX, "%s/%s", ref, chemin);
  else if (strlen(target) > 1)
    snprintf(ref_chemin, PATH_MAX, "%s/%s", ref, target);
  else
    snprintf(ref_chemin, PATH_MAX, "%s", ref);

  char *formated_path = malloc(PATH_MAX);
  for (int i=0;i<PATH_MAX;i++) formated_path[i] = '\0';
  format_path(ref_chemin, 0, formated_path);
  free(ref_chemin);

  star_aux(formated_path, result, len_result, 2);

  free(formated_path);
  
  while((entry = readdir(dirp1))){
    if(entry->d_name[0] != '.'){
      snprintf(ref_tmp, PATH_MAX, "%s/%s", ref, entry->d_name);
      if((d = opendir(ref_tmp)) != NULL){
        double_star_aux(ref_tmp, target, chemin, result, len_result);
      }
      closedir(d);
    }
  }
  free(ref_tmp);
  closedir(dirp1);
  return 0;
}

static void parse_ref(char * ref, char * target, char * chemin){  
//  **/A/toto -> target: toto, chemin: A 
//  **/toto   -> target: toto, chemin: ø
//  **/*.c    -> target: (**/main.c, **/pwd.c, **/cd.c), chemin: ø
  int len = strlen(ref);
  int i = len - 1;
  while(ref[i] != '/' && i > -1){
    i--;
  }
  strcpy(target, ref + i + 1);
  ref[i] = '\0';
  strcpy(chemin, ref + 2);
  strcat(chemin, "/"); // Pour des raisons d'affichage
}

void double_star(char *ref, char **result, int *len){
  char *target = malloc(PATH_MAX), *chemin = malloc(PATH_MAX);
  parse_ref(ref, target, chemin);
  char *dir = malloc(PATH_MAX);
  strcpy(dir, ".");
  double_star_aux(dir, target, chemin, result, len);
  free(target);
  free(chemin);
  free(dir);
}
