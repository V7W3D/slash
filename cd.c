#include "mystring.h"
#include "split_string.h"
#include "cd.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>

struct string * PWD = NULL;
struct string * OLD_PATH = NULL;

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
  if(len > 1){
    perror("Too many arguments");
    return -1;
  }
  if(len > 0){
    if(strcmp(args[0], "-") == 0){
        chdir(OLD_PATH->data); 
        if(mode == 'P'){
           init_String(OLD_PATH);
  	       string_append(OLD_PATH, PWD->data);
           maj_PWD_P();
        }
        else{
  	    char *ref = malloc(OLD_PATH->capacity);
	      strcpy(ref, OLD_PATH->data);
        init_String(OLD_PATH);
        string_append(OLD_PATH, PWD->data);
        init_String(PWD);
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
        init_String(OLD_PATH);
        string_append(OLD_PATH, PWD->data);
        maj_PWD_P();
      }
      else{
        init_String(OLD_PATH);
        string_append(OLD_PATH, PWD->data);
        if(maj_PWD_L(args[0]) == -1) {
	         maj_PWD_P(); 
	      }
      }
    }
  }
  else{
    chdir(getenv("HOME"));
    maj_PWD_P();
  }

  return 0;
}

