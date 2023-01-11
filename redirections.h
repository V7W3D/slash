#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

extern int exit_code;
extern int SIG;

int lecture(char *fic);
int sans_ecrasement_stdout(char *fic);
int avec_ecrasement_stdout(char *fic);
int en_concat_stdout(char * fic);
int en_concat_stderr(char *fic);
int sans_ecrasement_stderr(char * fic);
int avec_ecrasement_stderr(char * fic);
void pipeline(char *args);

#endif