#include<string.h>
#include<stdlib.h>
#include "split_string.h"

int split_string(char *string, const char *delimiters, char **result){
  int count = 0;

  char *token = strtok(string, delimiters);
  while(token != NULL){
    result[count] = malloc(sizeof(char)*strlen(token));
    strcpy(result[count], token);
    count++;
    token = strtok(NULL, delimiters); 
  }
  return count;
}

void free_splited_string(char **splited_string, int len){
  for(int i=0; i<len; i++){
    free(splited_string[i]);
  }
  free(splited_string);
}