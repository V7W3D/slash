#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "split_string.h"
#include "pwd.h"
#include "cd.h"

int pwd_L(){
    //write(STDERR_FILENO,PWD->data,sizeof(PWD->data));
    printf("%s\n",PWD->data);
    return 0;
}
int pwd_P(){
    char *BUF = malloc(sizeof(char)*PATH_MAX);
    getcwd(BUF,PATH_MAX);
    //write(STDERR_FILENO,BUF,PATH_MAX);
    printf("%s\n",BUF);
    free(BUF);
    return 0;
}

int parse_args(char ** args, int len, int *ind, char *c){
    int m = 'L';
    int i = 0;
    while(i < len){
        if(strcmp("-L",args[i]) == 0) m = 'L';
        else if (strcmp("-P",args[i]) == 0) m = 'P';
        else if((args[i][0] == '-') && strlen(args[i]) == 2){
          return -1;
        }
        else break;
        i++;
    }
    
    *ind = i;
    *c = m;
    return 0;
}

int slash_pwd(char **args, int len){
    int ind;
    char m;
    if(parse_args(args,len,&ind,&m) == -1){
      write(STDERR_FILENO,"Invalid argument\n",17);
      return -1;
    }
    else{
      if (m == 'L') pwd_L(); else pwd_P();
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
    if(strcmp(splited_ligne[0], "pwd") == 0){
        slash_pwd(splited_ligne + 1, len-1);
    }
    free_splited_string(splited_ligne, len);
    ligne = readline("monprompt$ ");
  }
  return 0;
}