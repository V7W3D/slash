#ifndef UTILSLASH_H
#define UTILSLASH_H

#define MAXLENPROMPT 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char* get_suffix_after_star(char *ref, int *star_index);
int end_with_suffix(char *string, char *suffix);
void insert_2d_array(char **result, char *data, int *current_pos);
int is_intern(char *str);
int check_args(char **args, int len);
int is_number(char *str);
void free_2d_array(char **array);
void absolute_path(char *path, int *index_start, char *result);
void format_path(char *string, int start_index, char *result);
int contains(char*string, char c);
void concat(char **buf1, char **buf2,int len1, int len2, char **result);

#endif