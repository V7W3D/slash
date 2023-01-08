#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

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
