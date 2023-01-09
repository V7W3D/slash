#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

void lecture(char ** cmd, char *fic);
void sans_ecrasement_stdout(char ** cmd, char *fic);
void avec_ecrasement_stdout(char ** cmd, char *fic);
void en_concat_stdout(char ** cmd, char * fic);
void en_concat_stderr(char ** cmd, char *fic);
void sans_ecrasement_stderr(char **cmd, char * fic);
void avec_ecrasement_stderr(char **cmd, char * fic);
void pipeline(char *args);

#endif