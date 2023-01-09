#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "redirections.h"
#include "split_string.h"

char mes_symboles[7][4] = {"<", ">", ">|", ">>", "2>>", "2>", "2>|"};

int ind_sym(char * sym){
    for(int i = 0; i < 7; i++){
        if(strcmp(mes_symboles[i], sym) == 0){
            return i;
        }
    }
    return -1;
}

void parse_redirections(char **splited_args, int len){
    int i;
    for(i = 0; i < len; i++){    
        switch(ind_sym(splited_args[i])){
            case 0:
                splited_args[i] = NULL;
                lecture(splited_args, splited_args[i+1]);
                break;
            case 1:
                splited_args[i] = NULL;
                sans_ecrasement_stdout(splited_args, splited_args[i+1]);
                break;
            case 2:
                splited_args[i] = NULL;
                avec_ecrasement_stdout(splited_args, splited_args[i+1]);
                break;
            case 3:
                splited_args[i] = NULL;
                en_concat_stdout(splited_args, splited_args[i+1]);
                break;
            case 4:
                splited_args[i] = NULL;
                en_concat_stderr(splited_args, splited_args[i+1]);
                break;
            case 5:
                splited_args[i] = NULL;
                sans_ecrasement_stderr(splited_args, splited_args[i+1]);
                break;
            case 6:
                splited_args[i] = NULL;
                avec_ecrasement_stderr(splited_args, splited_args[i+1]);
                break;                
        }
    }
    if(i == len) execvp(splited_args[0], splited_args);
}

// cmd < fic 
void lecture(char ** cmd, char *fic){
    int fd = open(fic, O_RDONLY);
    dup2(0, fd);
    execvp(cmd[0], cmd);
}

// cmd > fic 
void sans_ecrasement_stdout(char ** cmd, char *fic){
    int fd = open(fic,O_CREAT | O_EXCL | O_WRONLY , 0666);
    if(fd == -1){
        if(errno == EEXIST){
            write(2, "le fichier existe déjà\n", strlen("le fichier existe déjà\n"));
        }
    }
    dup2(fd, 1);
    execvp(cmd[0], cmd);
} 

// cmd >| fic 
void avec_ecrasement_stdout(char ** cmd, char *fic){
    int fd = open(fic, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    dup2(fd, 1);
    execvp(cmd[0],cmd);
}

// cmd >> fic
void en_concat_stdout(char ** cmd, char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0666);
    dup2(fd, 1);
    execvp(cmd[0], cmd);
}

// cmd 2>> fic
void en_concat_stderr(char ** cmd, char *fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0666);
    dup2(fd, 2);
    execvp(cmd[0], cmd);
}

// cmd 2> fic
void sans_ecrasement_stderr(char **cmd, char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(fd < 0){
        if(errno == EEXIST) write(2, "Le fichier existe déjà\n", 23);
    }
    else{
        dup2(fd, 2);
        execvp(cmd[0], cmd);
    }
}

// cmd 2>| fic
void avec_ecrasement_stderr(char **cmd, char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(fd, 2);
    execvp(cmd[0], cmd);
}

void fork_tree(int * i, int n, char ** splited_args){
    char ** splited_cmd;
    int len_cmd;

    if(*i < n){
        pid_t pid;
        int pipefd[2];
        pipe(pipefd);
        if((pid = fork()) < 0) perror("fork");
        else{
            if(pid == 0){
                close(pipefd[1]);
                dup2(pipefd[0], 0);
                *i = *i + 1;
                fork_tree(i, n, splited_args);
            }
            else{
                close(pipefd[0]);
                dup2(pipefd[1], 1);
                splited_cmd = allocate_splited_string();
                len_cmd = split_string(splited_args[*i], " ", splited_cmd);
                parse_redirections(splited_cmd, len_cmd);
            }
        }
    }
    splited_cmd = allocate_splited_string();
    len_cmd = split_string(splited_args[n], " ", splited_cmd);
    parse_redirections(splited_cmd, len_cmd);
}

void pipeline(char *args){
    char **splited_args = allocate_splited_string();
    int len = split_string(args, "|", splited_args);
    int i = 0;
    fork_tree(&i, len-1, splited_args);
}

int main(){
    char p1[256] = "cat texte | head -n 15 | tail -n 14 | head -n 13 | tail -n 10";
    char p[256] = "echo \"izan\" > err";
    pipeline(p);
    return 0;
}