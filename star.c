#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include "star.h"
#include "utilslash.h"

//char * STAR_PATH = malloc(PATH_MAX);

//retourne le tableau des paths 
void star_aux(char *ref, char **result, int *current_pos , int contain_multiple_star){
	char *suffix = NULL;
	DIR *dir = NULL; 
	struct dirent *entry = NULL;
	struct stat buf;
	int last_index_star = 0;
	int contain_star = 0;

	for (int index=0; index < strlen(ref); index++){
		if (*(ref+index) == '/') last_index_star = index+1;
		else if (*(ref+index) == '*' && ((*(ref+index+1) == '\0') || *(ref+index+1) != '*')
			&& ((index == 0) || *(ref+index-1) == '/')){
				contain_star = 1;
				suffix = get_suffix_after_star(ref, &index);

				char *path = malloc(last_index_star+1);
				memmove(path, ref, last_index_star);
				*(path+last_index_star) = '\0';

				dir = opendir(path);

				int is_empty = 0;

				if (dir != NULL){
					while ((entry = readdir(dir)) != NULL){
						is_empty = 1;
						if (strlen(entry->d_name) != 0 && end_with_suffix(entry->d_name, suffix)
							&& entry->d_name[0] != '.'){
							contain_star = 1;
							char *entry_path = malloc(strlen(path)+strlen(entry->d_name)+1);
							memmove(entry_path, ref, strlen(path));
							memmove(entry_path+strlen(path), entry->d_name, strlen(entry->d_name));
							*(entry_path + strlen(path)+strlen(entry->d_name)) = '\0';
							if (stat(entry_path, &buf) != 0) return;
							if (S_ISDIR(buf.st_mode) || S_ISLNK(buf.st_mode)){
								char *updated_path = malloc(strlen(entry_path)+strlen(ref+index+1)+1);
								memmove(updated_path, entry_path, strlen(entry_path));
								memmove(updated_path + strlen(entry_path), ref+index+1, strlen(ref+index+1));
								*(updated_path + strlen(entry_path)+strlen(ref+index+1)) = '\0';
								star_aux(updated_path, result, current_pos, contain_multiple_star);
								free(updated_path);
							}else{
									insert_2d_array(result, entry->d_name, current_pos);
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
	if (!contain_multiple_star) ref=format_string(ref);
	if (!contain_star && !stat(ref, &buf)) insert_2d_array(result, ref, current_pos);
}

//ref must be absolute
char** star(char *ref, int *len_array){
	int nb=0;
	for (int i=0; i<strlen(ref); i++){
		if (ref[i] == '*') nb++;
	}
	char **result = malloc(PATH_MAX * sizeof(char*));
	for (int i=0;i<PATH_MAX;i++) result[i] = NULL;
	int pos = 0;
	star_aux(ref, result, &pos, nb>1);
	*len_array = pos;
	return result;
}

/*
void double_star_aux(char * dir, char * target, char * chemin, char ** result, int len_result){ 
    DIR * dirp = opendir(".");
    struct dirent * entry;
    char * dir_tmp = malloc(PATH_MAX);
    strcpy(dir_tmp, dir);
    if(dir[0] != ''){
      strcat(STAR_PATH, "/");
      strcat(dir);
    }
    if(chemin[0] != '' && chdir(chemin) != -1){
      while((entry = readdir(dirp))){
        if(strcmp(entry->d_name, target) == 0){
	  result[len_result] = malloc(PATH_MAX);
	  strcpy(result[len_resutl], dir);
	  strcpy(result[len_result], chemin);
	  strcpy(result[len_result], target);
	  len_result++;
	  break;
	}
      }
      chdir(STAR_PATH);
    }
    while((entry = readdir(dirp))){
      if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
        if(DT_DIR(entry)){
	  strcat(dir,entry->d_name);
	  strcat(dir, "/");
	  if(chdir(entry->d_name) != -1){
            double_star(dir, target, chemin, result, len_result);
	    strcpy(dir, dir_tmp);
	    chdir("..");
	  }	
        }
      }
    }
    free(dir_tmp);
    closedir(dirp)
}

void double_star(char * target, char * chemin){
  char ** result = malloc(PATH_MAX);
  int i;
  for(i = 0; i < PATH_MAX; i++){
    result[i] = NULL;
  }
  char * dir = malloc(PATH_MAX);
  strcpy(dir, "");
  double_star_aux(dir, target, chemin, 0);
  i = 0;
  while(result[i]){
    free(result[i]);
    i++;
  }
  free(result);
  free(dir);
}
*/