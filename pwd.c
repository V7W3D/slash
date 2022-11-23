#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "split_string.h"
#include "pwd.h"
#include "cd.h"

int pwd_L(){
    write(STDERR_FILENO,PWD->data,sizeof(PWD->data));
    return 0;
}
int pwd_P(){
    char *BUF = malloc(sizeof(char)*PATH_MAX);
    getcwd(BUF,PATH_MAX);
    write(STDERR_FILENO,BUF,PATH_MAX);
    free(BUF);
    return 0;
}

char parse_args(char ** args, int len, int *ind){
    int m = 'L';
    int i = -1;
    if(len>=0) i = 0; 
    for(;i<len;i++){
        if(strcmp("-L",args[i]) == 0) m = 'L';
        else if (strcmp("-P",args[i]) == 0) m = 'P';
        else break;
    }
    
    ind = &i;
    return m;
}

void slash_pwd(char **args, int len){
    int ind;
    char mode = parse_args(args,len,&ind);
    printf("%c\n",mode);
    if(mode == 'L') pwd_L(); else pwd_P();
}

int main(){
    PWD = string_new(PATH_MAX);
    OLD_PATH = string_new(PATH_MAX);
    maj_PWD();
    string_cpy(OLD_PATH, PWD);

    return 0;
}