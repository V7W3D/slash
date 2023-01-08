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

void double_star_aux(char * dir, char * target, char * chemin, char ** result, int *len_result){ 
  int cpt = *len_result;
  printf("%d,\n",*len_result);
  DIR * dirp = opendir(".");
  struct dirent * entry;
  char dir_tmp[PATH_MAX];
  strcpy(dir_tmp, dir);
  if(strlen(dir) > 0){
    strcat(STAR_PATH, "/");
    strcat(STAR_PATH, dir);
  }
  if((strlen(chemin) > 0) && (chdir(chemin) != -1)){
    while((entry = readdir(dirp))){
      if(strcmp(entry->d_name, target) == 0){
        result[cpt] = malloc(PATH_MAX);
        strcpy(result[cpt], dir);
        strcat(result[cpt], chemin);
        strcat(result[cpt], target);
        cpt++;
        break;
      }
    }
    chdir(STAR_PATH);
  }
  else{
    while((entry = readdir(dirp))){  
      if(strcmp(entry->d_name, target) == 0){
        result[cpt] = malloc(PATH_MAX);
        strcpy(result[cpt], dir);
        strcat(result[cpt], target);
        cpt++;
        break;
      }
    }   
  }
  while((entry = readdir(dirp))){
    if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
      if(DT_DIR == entry->d_type){
        strcat(dir,entry->d_name);
        strcat(dir, "/");
        if(chdir(entry->d_name) != -1){
          *len_result = cpt;
          double_star_aux(dir, target, chemin, result, len_result);
          strcpy(dir, dir_tmp);
          chdir("..");
        }	
      }
    }
  }
  closedir(dirp);
}

void double_star(char * target, char * chemin){
  int len_res = 0;
  char ** result = malloc(PATH_MAX * sizeof(char*));
  int i;
  for(i = 0; i < PATH_MAX; i++){
    result[i] = NULL;
  }
  char dir[PATH_MAX];
  strcpy(dir, "");
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
  double_star("toto",".");
}
