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

void commande_interne(char ** updated_args, int len_updated_args){
    if (strcmp(updated_args[0], "cd") == 0){
        exit_code = slash_cd(updated_args+1, len_updated_args-1);
    }else if (strcmp(updated_args[0], "pwd") == 0){
        exit_code = slash_pwd(updated_args+1, len_updated_args-1);
    }else{
        if (len_updated_args == 2){
            if (is_number(updated_args[1])){
                int exit_parameter = atoi(updated_args[1]);
                free_2d_array(updated_args);
                exit(exit_parameter);
            }else{
                write(STDERR_FILENO, "exit : error must be an integer\n", 32);
            }
        }else if (len_updated_args == 1){
            free_2d_array(updated_args);
            exit(exit_code);
        }else{
            write(STDERR_FILENO, "exit: too many arguments\n", 25);
        }
    }
}

void stdin_fd(char ** cmd, int fd){
    dup2(fd, 0);
    execvp(cmd[0], cmd);
}

void fd_stdout(char ** cmd, int fd){
    dup2(fd, 1);
    execvp(cmd[0], cmd);
}

void fd_stderr(char ** cmd, int fd){
    dup2(fd, 2);
    execvp(cmd[0], cmd);
}

int ind_sym(char * sym){
    for(int i = 0; i < 7; i++){
        if(strcmp(mes_symboles[i], sym) == 0){
            return i;
        }
    }
    return -1;
}

void parse_redirections(char **updated_args, int len){
    int i, err = 0;
    /*
    int j = 0;
    while(updated_args[j]){
        write(STDERR_FILENO, updated_args[j], strlen(updated_args[j]));
        j++;
    }
    write(STDERR_FILENO, "\n", 1);*/
    int fd_tmp0 = dup(0);
    int fd_tmp1 = dup(1);
    int fd_tmp2 = dup(2);

    for(i = 0; i < len; i++){  
        switch(ind_sym(updated_args[i])){
            case 0:
                updated_args[i] = NULL;
                err = lecture(updated_args[i+1]);
                break;
            case 1:
                updated_args[i] = NULL;
                err = sans_ecrasement_stdout(updated_args[i+1]);
                break;
            case 2:
                updated_args[i] = NULL;
                err = avec_ecrasement_stdout(updated_args[i+1]);
                break;
            case 3:
                updated_args[i] = NULL;
                err = en_concat_stdout(updated_args[i+1]);
                break;
            case 4:
                updated_args[i] = NULL;
                err = en_concat_stderr(updated_args[i+1]);
                break;
            case 5:
                updated_args[i] = NULL;
                err = sans_ecrasement_stderr(updated_args[i+1]);
                break;
            case 6:
                updated_args[i] = NULL;
                err = avec_ecrasement_stderr(updated_args[i+1]);
                break;                
        }
    }

    i = 0;
    while(updated_args[i]) i++;

    if(err != 1){
        if(is_intern(updated_args[0]) == 0) commande_interne(updated_args, i); else execvp(updated_args[0], updated_args);
    }
        
    dup2(fd_tmp0, 0);
    dup2(fd_tmp1, 1);
    dup2(fd_tmp2, 2);
}

// cmd < fic 
int lecture(char *fic){
    //for(int i = 0; i < len_cmd; i++) write(STDERR_FILENO, cmd[i], strlen(cmd[i]));
    int fd = open(fic, O_RDONLY);
    if(fd == -1){
        exit_code = 1;
        return 1;
    }
    dup2(fd, 0);
    return 0;
}

// cmd > fic 
int sans_ecrasement_stdout(char *fic){
    int fd = open(fic, O_CREAT | O_EXCL | O_WRONLY , 0666);
    if(fd == -1){
        if(errno == EEXIST){
            write(2, "le fichier existe déjà\n", strlen("le fichier existe déjà\n"));
        }
        exit_code = 1;
        return 1;
    }
    else{
        dup2(fd, 1);
        return 0;
    }
} 

// cmd >| fic 
int avec_ecrasement_stdout(char *fic){
    int fd = open(fic, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if(fd == -1){
        exit_code = 1;
        return 1;
    }
    dup2(fd, 1);
    return 0;
}

// cmd >> fic
int en_concat_stdout(char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if(fd == -1){
        exit_code = 1;
        return 1;
    }
    dup2(fd, 1);
    return 0;
}

// cmd 2>> fic
int en_concat_stderr(char *fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if(fd == -1){
        exit_code = 1;
        return 1;
    }
    dup2(fd, 2);
    return 0;
}

// cmd 2> fic
int sans_ecrasement_stderr(char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(fd < 0){
        if(errno == EEXIST) write(2, "Le fichier existe déjà\n", strlen("Le fichier existe déjà\n"));
        exit_code = 1;
        return 1;
    }
    else {
        dup2(fd, 2);
        return 0;
    }
}

// cmd 2>| fic
int avec_ecrasement_stderr(char * fic){
    int fd = open(fic, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd == -1){
        exit_code = 1;
        return 1;
    }
    dup2(fd, 2);
    return 0;
}

void fork_tree(int *n, char ** splited_args){
    char ** splited_cmd;
    int len_cmd;

    if( *n > 0){
        pid_t pid;
        int pipefd[2];
        pipe(pipefd);
        if((pid = fork()) < 0) perror("fork");
        else{
            if(pid == 0){
                close(pipefd[0]);
                dup2(pipefd[1], 1);
                *n = *n - 1;
                fork_tree(n, splited_args);
            }
            else{
                close(pipefd[1]);
                dup2(pipefd[0], 0);
                int len_array = 0;
                splited_cmd = allocate_splited_string();
                len_cmd = split_string(splited_args[*n], " ", splited_cmd);
				char ** star_path = malloc(10 * PATH_MAX * sizeof(char*));
                for (int i=0;i<PATH_MAX;i++) star_path[i] = NULL;
				star(splited_cmd, len_cmd, &len_array, star_path);
                parse_redirections(star_path, len_array);
                free_2d_array(star_path);
                free_splited_string(splited_cmd);
                exit(exit_code);
            }
        }
    }
    int len_array = 0;
    splited_cmd = allocate_splited_string();
    len_cmd = split_string(splited_args[0], " ", splited_cmd);
    char ** star_path = malloc(10 * PATH_MAX * sizeof(char*));
    for (int i=0;i<PATH_MAX;i++) star_path[i] = NULL;
    star(splited_cmd, len_cmd, &len_array, star_path);
    parse_redirections(star_path, len_array);
    free_2d_array(star_path);
    free_splited_string(splited_cmd);
    exit(exit_code);
}

void pipeline(char *args){
    char **splited_args = allocate_splited_string();
    int len = split_string(args, " | ", splited_args);
    //fprintf(stderr, "%d\n", len);
    //for(int j = 0; j<len; j++) write(STDERR_FILENO, splited_args[j], strlen(splited_args[j]));
    int status, n = len - 1;
    if(len > 1){
        switch (fork()){
        case -1:						
            write(STDERR_FILENO,"fork", strlen("fork"));
            break;
        case 0:
            fork_tree(&n, splited_args);
            break;
        default:
            wait(&status);
            if (WIFEXITED(status)){
                if (WEXITSTATUS(status) == 0) exit_code = 0; else exit_code = 1;
            }
        }
    }
    else{
        int len_array = 0;
        char ** splited_cmd = allocate_splited_string();
        int len_cmd = split_string(splited_args[0], " ", splited_cmd);
        char ** star_path = malloc(10 * PATH_MAX * sizeof(char*));
        for (int i=0;i<PATH_MAX;i++) star_path[i] = NULL;
        star(splited_cmd, len_cmd, &len_array, star_path);
        free_splited_string(splited_cmd);

        if(is_intern(star_path[0]) == 0) parse_redirections(star_path, len_array);
        else{
            switch (fork()){
            case -1:						
                write(STDERR_FILENO,"fork", strlen("fork"));
                break;
            case 0:
                parse_redirections(star_path, len_array);
                free_2d_array(star_path);
                free_splited_string(splited_args);
                exit(errno);
                break;
            default:
                wait(&status);
                if (WIFEXITED(status)){
                     if (WEXITSTATUS(status) == 0) exit_code = 0; else exit_code = 1;
                }
            }
        }
        free_2d_array(star_path);
    }
    free_splited_string(splited_args);
}