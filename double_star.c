#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include "star.h"
#include "double_star.h"
#include "mystring.h"



char * STAR_PATH[PATH_MAX];

void double_star_aux(char * dir, char * target, char * chemin, char ** result, int len_result){ 
  DIR * dirp = opendir(".");
  struct dirent * entry;
  char * dir_tmp = malloc(PATH_MAX);
  strcpy(dir_tmp, dir);
  if(srtlen(dir) > 0){
    strcat(STAR_PATH, "/");
    strcat(STAR_PATH, dir);
  }
  if(srtlen(chemin) > 0 && chdir(chemin) != -1){
    while((entry = readdir(dirp))){
      if(strcmp(entry->d_name, target) == 0){
        result[len_result] = malloc(PATH_MAX);
        strcpy(result[len_result], dir);
        strcat(result[len_result], chemin);
        strcat(result[len_result], target);
        len_result++;
        break;
      }
    }
    chdir(STAR_PATH);
  }
  else{
    while((entry = readdir(dirp))){  
      if(strcmp(entry->d_name, target) == 0){
        result[len_result] = malloc(PATH_MAX);
        strcpy(result[len_result], dir);
        strcat(result[len_result], target);
        len_result++;
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
          double_star_aux(dir, target, chemin, result, len_result);
          strcpy(dir, dir_tmp);
          chdir("..");
        }	
      }
    }
  }
  free(dir_tmp);
  closedir(dirp);
}

void double_star(char * target, char * chemin){
  char ** result = malloc(PATH_MAX);
  int i;
  for(i = 0; i < PATH_MAX; i++){
    result[i] = NULL;
  }
  char * dir = malloc(PATH_MAX);
  strcpy(dir, "");
  double_star_aux(dir, target, chemin, result, 0);
  i = 0;
  while(result[i]){
    free(result[i]);
    i++;
  }
  free(result);
  free(dir);
}

