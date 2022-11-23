#include "mystring.h"
#include "split_string.h"
#include "cd.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>

struct string * PWD = NULL;
struct string * OLD_PATH = NULL;

int maj_PWD_L(char *ref){
  printf(" maj_PWD_L : %s\n", PWD->data);
  char **splited_ref = malloc(sizeof(char*) * 4096);
  int len = split_string(ref, "/", splited_ref);
  for(int i=0; i<len; i++){
    printf("%s\n", splited_ref[i]);
    if(strcmp(splited_ref[i], "..") == 0){
      int nchars = 0;
      for(int i = PWD->length; PWD->data[i] != '/'; i--) nchars++;
      if(nchars == 0){
        return -1;
      }
      string_truncate(PWD, nchars);
    }
    else{
      if(strcmp(splited_ref[i], ".") != 0){
        string_append(PWD, "/");
	string_append(PWD, splited_ref[i]);
	printf(" maj_PWD_L : %s\n", PWD->data);
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
        if(maj_PWD_L(args[0]) == -1) maj_PWD_P(); 
      }
    }
  }
  else{
    chdir(getenv("HOME"));
  }

  return 0;
}