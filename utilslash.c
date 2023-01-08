#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <linux/limits.h>
#include "utilslash.h"
#include "cd.h"
#include "mystring.h"

int is_intern(char *str){
	if (!strcmp(str, "pwd") || !strcmp(str, "cd") || !strcmp(str, "exit")) return 0;
	return 1;
}


int check_args(char **args, int len){
	for (int i=0;i<len;i++){
		if (strlen(args[i]) > MAX_ARGS_STRLEN) return 0;
	}
	return 1;
}

int is_number(char *str){
	for (int i=0;i<strlen(str);i++){
		if (!isdigit(str[i])) return 0;
	}
	return 1;
}

void free_2d_array(char **array){
	int i = 0;
	while (array[i] ){
		free(array[i]);
		i++;
	}
	free(array);
}

int contains(char*string, char c){
	int i=0;
	while (i < strlen(string)){
		if (string[i] == c) return 1;
		i++;
	}
	return 0;
}

void init(){
	maj_PWD_P();
	string_cpy(OLD_PATH, PWD);
}

