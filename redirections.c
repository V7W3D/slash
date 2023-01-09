#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <limits.h>
#include "redirections.h"
#include "split_string.h"
#include "utilslash.h"
#include "cd.h"
#include "pwd.h"
#include "star.h"

char mes_symboles[7][4] = {"<", ">", ">|", ">>", "2>>", "2>", "2>|"};
int exit_code = 0;

void commande_interne(char ** updated_args, int len_updated_args, int len_cmd){
    if (strcmp(updated_args[0], "cd") == 0){
        exit_code = slash_cd(updated_args+1, len_updated_args-1);
    }else if (strcmp(updated_args[0], "pwd") == 0){
        exit_code = slash_pwd(updated_args+1, len_updated_args-1);
    }else{
        if (len_cmd == 2){
            if (is_number(updated_args[1])){
                int exit_parameter = atoi(updated_args[1]);
                exit(exit_parameter);
            }else{
                write(STDERR_FILENO, "exit : error must be an integer\n", 32);
            }
        }else if (len_cmd == 1){
            exit(exit_code);
        }else{
            write(STDERR_FILENO, "exit: too many arguments\n", 25);
        }
    }
}

void stdin_fd(char ** cmd, int fd){
    int status;
    switch (fork())
    {
    case -1:						
        write(STDERR_FILENO,"fork", strlen("fork"));
    break;
    case 0:
        dup2(fd, 0);
        if (execvp(cmd[0], cmd) < 0){
            exit(errno);
        }
        break;
    default:
        wait(&status);
        if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
        break;
    }
}

void fd_stdout(char ** cmd, int fd){
    int status;
    switch (fork())
    {
    case -1:						
        write(STDERR_FILENO,"fork", strlen("fork"));
    break;
    case 0:
        dup2(fd, 1);
        if (execvp(cmd[0], cmd) < 0){
            exit(errno);
        }
        break;
    default:
        wait(&status);
        if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
        break;
    }
}

void fd_stderr(char ** cmd, int fd){
    int status;
    switch (fork())
    {
    case -1:						
        write(STDERR_FILENO,"fork", strlen("fork"));
    break;
    case 0:
        dup2(fd, 2);
        if (execvp(cmd[0], cmd) < 0){
            exit(errno);
        }
        break;
    default:
        wait(&status);
        if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
        break;
    }
}

int ind_sym(char * sym){
    for(int i = 0; i < 7; i++){
        if(strcmp(mes_symboles[i], sym) == 0){
            return i;
        }
    }
    return -1;
}

void parse_redirections(char **updated_args, int len, int len_cmd){
    int i;
    int brk = 0;
    for(i = 0; i < len && brk == 0; i++){    
        switch(ind_sym(updated_args[i])){
            case 0:
                updated_args[i] = NULL;
                lecture(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;
            case 1:
                updated_args[i] = NULL;
                sans_ecrasement_stdout(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;
            case 2:
                updated_args[i] = NULL;
                avec_ecrasement_stdout(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;
            case 3:
                updated_args[i] = NULL;
                en_concat_stdout(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;
            case 4:
                updated_args[i] = NULL;
                en_concat_stderr(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;
            case 5:
                updated_args[i] = NULL;
                sans_ecrasement_stderr(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;
            case 6:
                updated_args[i] = NULL;
                avec_ecrasement_stderr(updated_args, i, len_cmd, updated_args[i+1]);
                brk = 1;
                break;                
        }
    }
    if(i == len) {
        if(is_intern(updated_args[0]) == 0) commande_interne(updated_args, len, len_cmd);
        else{
            int status;
            switch (fork())
            {
            case -1:						
                write(STDERR_FILENO,"fork", strlen("fork"));
                break;
            case 0:
                if (execvp(updated_args[0], updated_args) < 0){
                    exit(errno);
                }
                break;
            default:
                wait(&status);
                if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
                break;
            }
        }
    }
}

// cmd < fic 
void lecture(char ** cmd, int len_cmd, int len, char *fic){
    //for(int i = 0; i < len_cmd; i++) write(STDERR_FILENO, cmd[i], strlen(cmd[i]));
    int fd = open(fic, O_RDONLY);
    if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else stdin_fd(cmd, fd);
}

// cmd > fic 
void sans_ecrasement_stdout(char ** cmd, int len_cmd, int len, char *fic){
    int fd = open(fic, O_CREAT | O_EXCL | O_WRONLY , 0666);
    if(fd == -1){
        if(errno == EEXIST){
            write(2, "le fichier existe déjà\n", strlen("le fichier existe déjà\n"));
        }
    }
    else{
        if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else fd_stdout(cmd, fd);
    }
} 

// cmd >| fic 
void avec_ecrasement_stdout(char ** cmd, int len_cmd, int len, char *fic){
    int fd = open(fic, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else fd_stdout(cmd, fd); 
}

// cmd >> fic
void en_concat_stdout(char ** cmd, int len_cmd, int len, char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else fd_stdout(cmd, fd);
}

// cmd 2>> fic
void en_concat_stderr(char ** cmd, int len_cmd, int len, char *fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else fd_stderr(cmd, fd);
}

// cmd 2> fic
void sans_ecrasement_stderr(char **cmd, int len_cmd, int len, char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(fd < 0){
        if(errno == EEXIST) write(2, "Le fichier existe déjà\n", 23);
    }
    else {
        if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else fd_stderr(cmd, fd);
    }
}

// cmd 2>| fic
void avec_ecrasement_stderr(char **cmd, int len_cmd, int len, char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(is_intern(cmd[0]) == 0) commande_interne(cmd, len_cmd, len); else fd_stderr(cmd, fd);
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
                int len_array = 0, len = 0;
                splited_cmd = allocate_splited_string();
                len_cmd = split_string(splited_args[*i], " ", splited_cmd);
				char ** star_path = malloc(10 * PATH_MAX * sizeof(char*));
				star(splited_cmd, len_cmd, &len_array, star_path);
				char ** updated_args = malloc(10 * PATH_MAX * sizeof(char*));
				for (int i=0;i<PATH_MAX;i++) updated_args[i] = NULL;
				concat(splited_cmd, star_path, len_cmd, len_array, updated_args, &len);
                parse_redirections(updated_args, len, len_cmd);
	            free_2d_array(updated_args);
                free_2d_array(star_path);
                free_splited_string(splited_cmd);
            }
        }
    }
    int len_array = 0, len = 0;
    splited_cmd = allocate_splited_string();
    len_cmd = split_string(splited_args[n], " ", splited_cmd);
    char ** star_path = malloc(10 * PATH_MAX * sizeof(char*));
    star(splited_cmd, len_cmd, &len_array, star_path);
    char ** updated_args = malloc(10 * PATH_MAX * sizeof(char*));
    for (int i=0;i<PATH_MAX;i++) updated_args[i] = NULL;
    concat(splited_cmd, star_path, len_cmd, len_array, updated_args, &len);
    parse_redirections(updated_args, len, len_cmd);
    free_2d_array(updated_args);
    free_2d_array(star_path);
    free_splited_string(splited_cmd);
    write(2, "Le fichier existe déjà\n", 24);
    
}

void pipeline(char *args){
    char **splited_args = allocate_splited_string();
    int len = split_string(args, "|", splited_args);
    int i = 0;
    fork_tree(&i, len-1, splited_args);
    free_splited_string(splited_args);
}