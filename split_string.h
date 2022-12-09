#ifndef SPLITSTRING_H
#define SPLITSTRING_H

int split_string(char *string, const char *delimiters, char **result);
void free_splited_string(char **splited_string);
char ** allocate_splited_string();
#endif 
