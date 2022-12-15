#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <linux/limits.h>
#include "utilslash.h"

char* get_suffix_after_star(char *ref, int *star_index){
	int len_result = -1, saved_star_pos = *star_index;
	while (ref[*star_index] != '\0' && ref[*star_index] != '/'){
		len_result++;
		*star_index = *star_index + 1;
	}
	char *result = malloc(len_result+1);
	memmove(result, ref+saved_star_pos+1, len_result);
	*(result + len_result) = '\0';
	*star_index = *star_index - 1;
	return result;
}

int end_with_suffix(char *string, char *suffix){
	if (strlen(suffix) == 0) return 1;
	char *end_string = string + strlen(string) - 1;
	char *end_suffix = suffix + strlen(suffix) - 1;
	while (end_suffix!=suffix && end_string != string){
		if (*end_string != *end_suffix) return 0;
		end_suffix--,
		end_string--;
	}
	if (*end_string != *end_suffix) return 0;
	return 1;
}


void insert_2d_array(char **result, char *data, int *current_pos){
	int pos = *current_pos;
	result[pos] = malloc(strlen(data)+1);
	memmove(result[pos], data, strlen(data));
	result[pos][strlen(data)] = '\0';
	*current_pos = *current_pos + 1;
}

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
}

char *absolute_path(char **args, int len_args, int *index){

	char *path = NULL;

	int i = 0;
	while(i < len_args){
		if (args[i][0] != '-'){
			*index = i;
			path = malloc(sizeof(char)*strlen(args[i]));
			memmove(path, args[i], strlen(args[i]));
			path[strlen(args[i])] = '\0';
			break;
		}
		i++;
	}

	if (path != NULL && strlen(path) > 0){
		char *newpath;
		if (path[0] == '/' || path[0] == '\\'){
			newpath = malloc(strlen(path));
			memmove(newpath, path, strlen(path));
			newpath[strlen(path)] = '\0';
		}else{
			char *abs_path = malloc(PATH_MAX);
			getcwd(abs_path, PATH_MAX);
			newpath = malloc(strlen(abs_path)+strlen(path)+2);
			memmove(newpath, abs_path, strlen(abs_path));
			memmove(newpath+strlen(abs_path), "/", 1);
			memmove(newpath+strlen(abs_path)+1, path, strlen(path));
			newpath[strlen(abs_path)+strlen(path)+1] = '\0';
			free(abs_path);
		}
		free(path);
		return newpath;
	}

	char *abs_path = malloc(PATH_MAX);
	getcwd(abs_path, PATH_MAX);
	return abs_path;
}

char *format_string(char *string){
	char *result;
	int len = strlen(string) - 1;
	while (len){
		if (string[len] == '/') break;
		len--;
	}
	result = malloc(sizeof(char) * (strlen(string)-len));
	memmove(result, string + len+1, strlen(string)-len);
	result[strlen(string)-len] = '\0';
	return result;
}

int contains(char*string, char c){
	return strchr(string, c) != NULL;
}