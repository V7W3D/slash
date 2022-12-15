#ifndef UTIL_H
#define UTIL_H

char* get_suffix_after_star(char *ref, int *star_index);
int end_with_suffix(char *string, char *suffix);
void insert_2d_array(char **result, char *data, int *current_pos);
int is_intern(char *str);
int check_args(char **args, int len);
int is_number(char *str);

#endif