#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mystring.h"
#include "split_string.h"
#include "cd.h"

struct string * PWD;
struct string * OLD_PATH;

int maj_PWD_L(char *ref){
  char **splited_ref = malloc(sizeof(char*) * 4096);
  int len = split_string(ref, "/", splited_ref);
  int nchars;
  for(int i=0; i<len; i++){
    if(strcmp(splited_ref[i], "..") == 0){
      if(PWD->length > 1){
        nchars = 0;
        for(int i = PWD->length; PWD->data[i] != '/'; i--) nchars++;
        string_truncate(PWD, nchars);
	if(PWD->length == 0) string_append(PWD, "/");
      }
      else return -1;
    }
    else{
      if(strcmp(splited_ref[i], ".") != 0){
        if(strcmp(PWD->data, "/") != 0) string_append(PWD, "/");
	string_append(PWD, splited_ref[i]);
      }
    }
  }
  free_splited_string(splited_ref, len);
  return 0;
}

void maj_PWD_P(){
  getcwd(PWD->data, PWD->capacity);
  PWD->length = strlen(PWD->data);
}

int slash_cd(char **args, int len){
  char mode = 'L';
  int opt;

/*  
  while((opt = getopt(len, args, "L:P")) != -1){
    switch(opt) {
      case 'L' : mode = 'L'; break;
      case 'P' : mode = 'P'; break;
      default : 
	fprintf(stderr, "cd : utilisation : cd [-L | -P] [ref | -]");
	return 1;
    }
  }
  
  printf("%c\n", mode); 
*/
  if(len > 1){
    perror("Too many arguments");
    return -1;
  }
  if(len > 0){
    if(strcmp(args[0], "-") == 0){
      chdir(OLD_PATH->data); 
      if(mode == 'P'){
	 string_cpy(OLD_PATH, PWD);
         maj_PWD_P();
      }
      else{
	char *ref = malloc(OLD_PATH->capacity);
	strcpy(ref, OLD_PATH->data);
        string_cpy(OLD_PATH, PWD);
        maj_PWD_L(ref);
	free(ref);
      }
    }
    else{
      if(chdir(args[0]) == -1){
        perror("cd");
        return -1;
      }
      if(mode == 'P'){
        string_cpy(OLD_PATH, PWD);
        maj_PWD_P();
      }
      else{
        string_cpy(OLD_PATH, PWD);
	if(args[0][0] == '/'){
	  string_truncate(PWD, PWD->length);
	  string_append(PWD, "/");
	}
        if(maj_PWD_L(args[0]) == -1) {
	  maj_PWD_P(); 
	}
      }
    }
  }
  else{
    chdir(getenv("HOME"));
  }

  return 0;
}

int main(){
  int len;

  PWD = string_new(PATH_MAX);
  OLD_PATH = string_new(PATH_MAX);
  maj_PWD_P();
  string_cpy(OLD_PATH, PWD);

  char *ligne = readline("monprompt$ ");
  while(strcmp(ligne, "exit") != 0){
    char **splited_ligne = malloc(sizeof(char*) * 4096);
    len = split_string(ligne, " ", splited_ligne);
    if(strcmp(splited_ligne[0], "cd") == 0){
      slash_cd(splited_ligne + 1, len-1);
      printf("%s\n", PWD->data);
    }
    free_splited_string(splited_ligne, len);
    ligne = readline("monprompt$ ");
  }
  return 0;
}
