#ifndef SPLITSTRING_H
#define SPLITSTRING_H

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

int split_string(char *string, const char *delimiters, char **result);
void free_splited_string(char **splited_string, int len);

#endif 
