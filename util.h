#ifndef UTIL_H
#define UTIL_H

#define MAXLENPROMPT 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char * INTERN_COMMAND[3] =  {"pwd","cd","exit"};

static char* get_suffix_after_star(char *ref, int *star_index);
static int end_with_suffix(char *string, char *suffix);
static void insert_2d_array(char **result, char *data, int *current_pos);
int is_intern(char *str);
int check_args(char **args, int len);
int is_number(char *str);

#endif