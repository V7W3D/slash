#include <string.h>
#include <stdlib.h>
#include "split_string.h"
#include <limits.h>
int split_string(char *string, const char *delimiters, char **result){
  int count = 0;

  char *token = strtok(string, delimiters);
  while(token != NULL){
    *(result + count) = malloc(strlen(token) + 1);
    strcpy(*(result + count), token);
    count++;
    token = strtok(NULL, delimiters); 
  }
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
