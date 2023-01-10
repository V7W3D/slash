#ifndef STAR_H
#define STAR_H

void star(char **args,int len_args, int *len_array, char **result);
void star_aux(char *ref, char **result, int *current_pos, int len_abs_path);
void concat(char **buf1, char **buf2, int len1, int len2, char **result);
int len_concat(char ** concat_string);

#endif
