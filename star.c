#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static void get_suffix_after_star(char *ref, int *star_index, char *result){
	int len_result = 0, saved_star_pos = *star_index;
	while (*(*star_index+ref) != '\0' && *(*star_index+ref) != '/'){
		len_result++;
		*star_index++;
	}
	*result = malloc(len_result+1);
	memmove(result, ref+saved_star_pos+1, len_result);
	*(result + len_result) = '\0';
}

static int end_with_suffix(char *string, char *suffix){
	if (strlen(suffix) == 0) return 1;
	char *end_string = string + strlen(string) - 1;
	char *end_suffix = suffix + strlen(suffix) - 1;
	int is_equal = 1;
	while (end_suffix!=suffix && end_string != string){
		if (*end_string != *end_suffix) return 0;
		end_suffix--,
		end_string--;
	}
	if (*end_string != *end_suffix) return 0;
	return 1;
}


static void insert_2d_array(char **result, char *data, int *current_pos){
	*result+current_pos = malloc(strlen(data)+1);
	memmove(*result+current_pos, data, strlen(data));
	*(*result+current_pos) = '\0';
	*current_pos = *current_pos + 1;
}

//retourne le tableau des paths 
void star_aux(char *ref, char **result, int *current_pos){
	char *suffix = NULL;
	DIR *dir = NULL; 
	struct dirent *entry = NULL;
	struct stat buf;
	int last_index_star = 0;
	int contain_star = 0;

	for (int index=0; *(index+ref) != '\0'; index++){
		if (*(ref+index) == '/') last_index_star = index+1;
		else if (*(ref+index) == '*' && (*((ref+index+1) == '\0') || *(ref+index+1) != '*')
			&& ((index == 0) || *(ref+index-1) == '/')){
				contain_star = 1;
				get_suffix_after_star(ref, &index, &suffix);

				char *path = malloc(last_index_star+1);
				memmove(path, ref, last_index_star);
				*(path+last_index_star) = '\0';

				dir = opendir(path);
				if (dir == NULL) return;
				while ((entry = readdir(dir)) != NULL){
					if (strlen(entry->d_name) != 0 && end_with_suffix(entry->d_name, suffix)){
						char *entry_path = malloc(strlen(path)+strlen(entry->d_name)+1);
						memmove(entry_path, ref, strlen(path));
						memmove(entry_path+strlen(path), entry->d_name, strlen(entry->d_name));
						*(entry_path + strlen(path)+strlen(entry->d_name)) = '\0';
						if (!lstat(entry_path, &buf)) return;
						if (S_ISDIR(buf.mode) || S_ISLNK(buf.mode)){
							char *updated_path = malloc(strlen(entry_path)+strlen(ref+index)+1);
							memmove(updated_path, entry_path, strlen(entry_path));
							memmove(updated_path + strlen(entry_path), ref+index, strlen(ref)-index);
							*(updated_path + strlen(entry_path)+strlen(ref+index)) = '\0';
							star_aux(updated_path, result, current_pos);
							free(updated_path);
						}else{
							if (*(ref+index) != '\0') return;
							else insert_2d_array(result, entry_path, current_pos);
						}
						free(entry_path);
					}
				}

				closedir(dir);
				free(path);
				free(suffix);
		}
	}
	if (!contain_star) insert_2d_array(result, ref, current_pos);
}

char** star(char *ref){
	char **result = malloc(sizeof(char*) * PATH_MAX);
	for (int i=0;i<PATH_MAX;i++) *result+i = NULL;
	int pos = 0;
	star_aux(ref, result, &pos);
	return result;
}