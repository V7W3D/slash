#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "split_string.h"
#include "double_star.h"

char STAR_PATH[PATH_MAX];

int double_star_aux(char * ref, char * target, char * chemin, char ** result, int *len_result){ 
  struct dirent * entry;
  char ref_tmp[PATH_MAX], ref_chemin[PATH_MAX];
  DIR * dirp1;
  DIR * dirp2;
  DIR * d;
  
  if((dirp1 = opendir(ref)) == NULL){
    perror("opendir");
    exit(1);
  }

  snprintf(ref_chemin, PATH_MAX, "%s/%s", ref, chemin);
  if((dirp2 = opendir(ref_chemin)) != NULL){
    while((entry = readdir(dirp2))){
      if(strcmp(entry->d_name, target) == 0){
        result[*len_result] = malloc(PATH_MAX);
        strcpy(result[*len_result], ref);
        strcat(result[*len_result], chemin);
        strcat(result[*len_result], target);
        *len_result = *len_result + 1;
        break;
      }
    }
  }
  closedir(dirp2);
  
  while((entry = readdir(dirp1))){
    if(entry->d_name[0] != '.'){
      snprintf(ref_tmp, PATH_MAX, "%s/%s", ref, entry->d_name);
      if((d = opendir(ref_tmp)) != NULL){
        double_star_aux(ref_tmp, target, chemin, result, len_result);
      }
      closedir(d);
    }
  }
  closedir(dirp1);
  return 0;
}

void double_star(char * target, char * chemin){
  int len_res = 0;
  char ** result = malloc(PATH_MAX * sizeof(char*));
  int i;
  for(i = 0; i < PATH_MAX; i++){
    result[i] = NULL;
  }
  char dir[PATH_MAX];
  strcpy(dir, ".");
  double_star_aux(dir, target, chemin, result, &len_res);
  i = 0;
  while(result[i]){
    printf("-> %s ",result[i]);
    i++;
  }
  printf("\n");
  i = 0;
  while(result[i]){
    free(result[i]);
    i++;
  }
  free(result);
}

void parse_ref(char * ref, char * target, char * chemin){  
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


int parse_args_main(char ** args, int len){
	int i = 1;
	while(i < len){
		if((args[i][0] == '-') && strlen(args[i]) > 1){
			i++;
		}
    else break;
	}
	return i;
}

int main(int argc, char ** argv){
  char target[256];
  char chemin[256];
  char ref[256] = "**/toto";

  parse_ref(ref, target, chemin);

  printf("%s\n", target);
  printf("%s\n", chemin);

  double_star(target, chemin);

}
