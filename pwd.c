#include "split_string.h"
#include "pwd.h"
#include "cd.h"
#include <limits.h>
#include <unistd.h>
#include <string.h>

int pwd_L(){
    return 0;
}
int pwd_P(){
    char *BUF = malloc(sizeof(char)*PATH_MAX);
    getcwd(BUF,PATH_MAX);
    init_String(PWD);
    string_append(PWD, BUF);
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
    if(mode == 'L') pwd_L(); else pwd_P();
}