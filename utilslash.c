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
	free(data);
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
	free(array);
}

void absolute_path(char *path, int *index_start, char *result){

	if (path[0] == '-') return;

	if (path != NULL && strlen(path) > 0){
		if (path[0] == '/' || path[0] == '\\'){
			*index_start = 0;
			memmove(result, path, strlen(path));
			result[strlen(path)] = '\0';
		}else{
			char *abs_path = malloc(PATH_MAX);
			getcwd(abs_path, PATH_MAX);
			*index_start = strlen(abs_path)+1;
			memmove(result, abs_path, strlen(abs_path));
			memmove(result+strlen(abs_path), "/", 1);
			memmove(result+strlen(abs_path)+1, path, strlen(path));
			result[strlen(abs_path)+strlen(path)+1] = '\0';
			free(abs_path);
		}
		return;	
	}

	char *abs_path = malloc(PATH_MAX);
	getcwd(abs_path, PATH_MAX);
	memmove(result, abs_path, strlen(abs_path)+1);
	*index_start = strlen(abs_path)+1;
	free(abs_path);
}

void format_path(char *string, int start_index, char *result){
	int i = 0;
	int inserted = 0;
	while (start_index < strlen(string)){
		result[i] = string[start_index];
		start_index++;
		i++;
		inserted = 0;
		while(string[start_index] == '/'){
			if (!inserted){
				result[i] = string[start_index];
				inserted = 1;
				i++;
			}
			start_index++;
		}
		
	}
}

int contains(char*string, char c){
	int i=0;
	while (i < strlen(string)){
		if (string[i] == c) return 1;
		i++;
	}
	return 0;
}

void concat(char **buf1, char **buf2,int len1, int len2, char **result){
	int stoped_index = 0;
	if (!contains(buf1[0], '*')){
		for (int i=0;i<len1;i++){
			if (!contains(buf1[i], '*')){
				result[stoped_index] = malloc(strlen(buf1[i])+1);
				memmove(result[stoped_index], buf1[i], strlen(buf1[i]));
				result[stoped_index][strlen(buf1[i])] = '\0';
				stoped_index++;
			}
		}
		for (int i=stoped_index;i<len2+stoped_index;i++){
		result[i] = malloc(strlen(buf2[i-stoped_index])+1);
		memmove(result[i], buf2[i-stoped_index], strlen(buf2[i-stoped_index]));
		result[i][strlen(buf2[i-stoped_index])] = '\0';
		}
	}else{
		for (int i=0;i<len2;i++){
			result[i] = malloc(strlen(buf2[i])+1);
			memmove(result[i], buf2[i], strlen(buf2[i]));
			result[i][strlen(buf2[i])] = '\0';stoped_index++;
		}
		for (int i=0;i<len1;i++){
			if (!contains(buf1[i], '*')){
				result[stoped_index] = malloc(strlen(buf1[i])+1);
				memmove(result[stoped_index], buf1[i], strlen(buf1[i]));
				result[stoped_index][strlen(buf1[i])] = '\0';
				stoped_index++;
			}
		}
	}
}