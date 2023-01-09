#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include "star.h"
#include "utilslash.h"
#include "double_star.h"

void concat(char **buf1, char **buf2, int len1, int len2, char **result){
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

static char* get_suffix_after_star(char *ref, int *star_index){
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

static int end_with_suffix(char *string, char *suffix){
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

static void insert_2d_array(char **result, char *data, int *current_pos){
	int pos = *current_pos;
	result[pos] = malloc(strlen(data)+1);
	memmove(result[pos], data, strlen(data));
	result[pos][strlen(data)] = '\0';
	*current_pos = *current_pos + 1;
	free(data);
}

static void absolute_path(char *path, int *index_start, char *result){

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

static void format_path(char *string, int start_index, char *result){
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

static char* alloc_string(int len){
	char *result = malloc(len);
	for (int i=0;i<len;i++) result[i] = '\0';
	return result;
}	

void star_aux(char *ref, char **result, int *current_pos, int len_abs_path){
	
	struct stat buf;

	if (contains(ref, '*')){
		char *suffix = NULL;
		DIR *dir = NULL; 
		struct dirent *entry = NULL;
		int last_index_star = 0;

		for (int index=0; index < strlen(ref); index++){
			if (*(ref+index) == '/') last_index_star = index+1;
			else if (*(ref+index) == '*' && ((*(ref+index+1) == '\0') || *(ref+index+1) != '*')
				&& ((index == 0) || *(ref+index-1) == '/')){
					suffix = get_suffix_after_star(ref, &index);

					char *path = alloc_string(last_index_star+1); 
					memmove(path, ref, last_index_star);

					dir = opendir(path);

					if (dir != NULL){
						while ((entry = readdir(dir)) != NULL){
							if (strlen(entry->d_name) != 0 && end_with_suffix(entry->d_name, suffix)
								&& entry->d_name[0] != '.'){
								char *entry_path = alloc_string(strlen(path)+strlen(entry->d_name)+1);
								memmove(entry_path, ref, strlen(path));
								memmove(entry_path+strlen(path), entry->d_name, strlen(entry->d_name));
								if (stat(entry_path, &buf) != 0) return;
								if (S_ISDIR(buf.st_mode) || S_ISLNK(buf.st_mode)){
									char *updated_path=alloc_string(strlen(entry_path)+strlen(ref+index+1)+1);
									memmove(updated_path, entry_path, strlen(entry_path));
									memmove(updated_path + strlen(entry_path), ref+index+1, strlen(ref+index+1));
									star_aux(updated_path, result, current_pos, len_abs_path);
									free(updated_path);
								}else{
									if (index+strlen(suffix)+1 >= strlen(ref)) {
										char *result_formated = alloc_string(PATH_MAX);
										format_path(entry_path, len_abs_path, result_formated);
										insert_2d_array(result, result_formated, current_pos);
									}
								}
								free(entry_path);
							}
						}
						closedir(dir);
					}

					free(path);
					free(suffix);
			}
		}
	}else if (!stat(ref, &buf)){
		char *result_formated = alloc_string(PATH_MAX);
		format_path(ref, len_abs_path, result_formated);
		insert_2d_array(result, result_formated, current_pos);
	}
}

void star(char **args,int len_args, int *len_array, char **result){
	for (int i=0;i<PATH_MAX;i++) result[i] = NULL;
	int start_index = 0;
	int index_path = 0;
	int pos_befor = 0;
	while(args[index_path]){
		if (strlen(args[index_path]) > 1 && args[index_path][0] == '*' && args[index_path][1] == '*'){
			pos_befor = *len_array;
			double_star(args[index_path], result, len_array);
			if (pos_befor == *len_array && index_path == 0){
				char *cpy = alloc_string(PATH_MAX);
				memmove(cpy, args[index_path], strlen(args[index_path]));
				insert_2d_array(result, cpy, len_array);
			}
		}else{
			char *abs_path = alloc_string(PATH_MAX);
			absolute_path(args[index_path], &start_index, abs_path);
			if (abs_path && contains(args[index_path], '*')){
				pos_befor = *len_array;
				star_aux(abs_path, result, len_array, start_index);
				if (pos_befor == *len_array && index_path == 0){
					char *cpy = alloc_string(PATH_MAX);
					memmove(cpy, args[index_path], strlen(args[index_path]));
					insert_2d_array(result, cpy, len_array);
				}
			}
			free(abs_path);
		}
		index_path++;
	}
}