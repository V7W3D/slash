#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

extern int exit_code;

void lecture(char ** cmd, int len_cmd, int len, char *fic);
void sans_ecrasement_stdout(char ** cmd, int len_cmd, int len, char *fic);
void avec_ecrasement_stdout(char ** cmd, int len_cmd, int len, char *fic);
void en_concat_stdout(char ** cmd, int len_cmd, int len, char * fic);
void en_concat_stderr(char ** cmd, int len_cmd, int len, char *fic);
void sans_ecrasement_stderr(char **cmd, int len_cmd, int len, char * fic);
void avec_ecrasement_stderr(char **cmd, int len_cmd, int len, char * fic);
void pipeline(char *args);

#endif