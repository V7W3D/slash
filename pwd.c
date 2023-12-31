#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include "split_string.h"
#include "pwd.h"
#include "cd.h"
#include "mystring.h"

int pwd_P(){
    char *BUF = malloc(sizeof(char)*PATH_MAX);
    getcwd(BUF,PATH_MAX);
    write(STDOUT_FILENO, BUF, strlen(BUF));
    write(STDOUT_FILENO, "\n", 1);
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
          return 1;
        }
        else break;
        i++;
    }
    *ind = i;
    if (c!=NULL) *c = m;
    return 0;
}

int slash_pwd(char **args, int len){
    int ind;
    char m;
    if(parse_args(args,len,&ind,&m) == -1){
      write(STDERR_FILENO,"Invalid argument\n",17);
      return 1;
    }
    else{
      if (m == 'P') pwd_P();
      else{
        write(STDOUT_FILENO, PWD->data, PWD->length);
        write(STDOUT_FILENO, "\n", 1);
      } 
    }
    return 0;
}