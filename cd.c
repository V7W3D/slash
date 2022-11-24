#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "mystring.h"
#include "split_string.h"
#include "cd.h"
#include "pwd.h"

struct string * PWD = NULL;
struct string * OLD_PATH = NULL;

void maj_PWD_L(char *ref){
  char **splited_ref = allocate_splited_string();
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
    }
    else{
      if(strcmp(splited_ref[i], ".") != 0){
        if(strcmp(PWD->data, "/") != 0) string_append(PWD, "/");
	      string_append(PWD, splited_ref[i]);
      }
    }
  }
  free_splited_string(splited_ref);
}

void maj_PWD_P(){
  getcwd(PWD->data, PWD->capacity);
  PWD->length = strlen(PWD->data);
}

int slash_cd(char **args, int len){
  char mode;
  int opt;
  
  if(parse_args(args, len, &opt, &mode) == -1){
    write(STDERR_FILENO, "slash: cd: invalid option\n", strlen("slash: cd: invalid option\n"));
    return 1;
  }

  if(len - opt > 1){
    write(STDERR_FILENO, "cd : Too many arguments\n", strlen("cd : Too many arguments\n"));
    return 1;
  }
  if(len - opt > 0){
    if(strcmp(args[opt], "-") == 0){
      chdir(OLD_PATH->data); 
      if(mode == 'P'){
        string_cpy(OLD_PATH, PWD);
        maj_PWD_P();
      }
      else{
	      char *ref = malloc(OLD_PATH->length + 1);
        strcpy(ref, OLD_PATH->data);
        string_cpy(OLD_PATH, PWD);
        init_string(PWD);
        string_append(PWD, ref);
	      free(ref);
      }
    }
    else{
      if (chdir(args[opt]) == -1){
        write(STDERR_FILENO, "cd: no such file or directory\n", strlen("cd: no such file or directory\n"));
        return 1;
      }
      if(mode == 'P'){
        string_cpy(OLD_PATH, PWD);
        maj_PWD_P();
      }
      else{
        string_cpy(OLD_PATH, PWD);
        if(args[opt][0] == '/'){
          string_truncate(PWD, PWD->length);
          string_append(PWD, "/");
        }
        maj_PWD_L(args[opt]);
        if(chdir(PWD->data) == -1){
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
