#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>



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
            write(2, "le fichier existe déjà", strlen("le fichier existe déjà"));
        }stdout_cmd_fic_avec_ecrasement
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
