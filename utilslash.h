#ifndef UTILSLASH_H
#define UTILSLASH_H

#define MAXLENPROMPT 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

int is_intern(char *str);
int check_args(char **args, int len);
int is_number(char *str);
void free_2d_array(char **array);
int contains(char*string, char c);
void init();

#endif
