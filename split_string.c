#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "split_string.h"
#include <linux/limits.h>

int split_string(char *string, const char *delimiters, char **result){
  int count = 0, i = 0, l = strlen(delimiters), ls = strlen(string), brk, k=0, j;
  char token[PATH_MAX];

  while(i+l-1 < ls){
    brk = 0;
    for(j = 0; j < l && brk == 0; j++){
      if(string[i+j] != delimiters[j]) brk = 1;
    }
    if(brk == 0){
      token[k] = '\0';
      if(k > 0){
        *(result + count) = malloc(strlen(token) + 1);
        strcpy(*(result + count), token);
        count++;
      }
      k=0;
      memset(token, 0, sizeof(token));
      i += l;
    }
    else{
      for(int u = 0; u < j; u++){
        token[k] = string[i];
        k++;
        i++;
      }
    }
  }

  while(i < ls){
    token[k] = string[i];
    k++;
    i++;
  }

  token[k] = '\0';
  *(result + count) = malloc(strlen(token) + 1);
  strcpy(*(result + count), token);
  count++;

  return count;
}

void free_splited_string(char **splited_string){
  int i = 0;
  while(splited_string[i] && i<PATH_MAX){
    free(splited_string[i]);
    i++;
  }
  free(splited_string);
}

char ** allocate_splited_string(){
  char **splited_string = malloc(sizeof(char*) * PATH_MAX);
  for(int i=0; i<PATH_MAX; i++){
    splited_string[i] = NULL;
  }
  return splited_string;
}
