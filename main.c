#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <linux/limits.h>
#include <wait.h>
#include "cd.h"
#include "pwd.h"
#include "split_string.h"
#include "mystring.h"
#include "utilslash.h"
#include "star.h"

void init(){
	maj_PWD_P();
	string_cpy(OLD_PATH, PWD);
}


int main(int argc, char **argv){
	
	int exit_code = 0;
	rl_outstream = stderr;
	PWD = string_new(PATH_MAX);
	OLD_PATH = string_new(PATH_MAX);
	struct string *PROMPT = string_new(MAXLENPROMPT + 1);
	char *args , **splited_args;
	
	init();
	
	if (PROMPT == NULL){
		write(STDERR_FILENO, "erreur : string_new", 20);
		exit(EXIT_FAILURE);
	}
	while(1){
		init_string(PROMPT);
		int len_splited_args;
		args = NULL;
		char *str1 = malloc(7*sizeof(char));
		snprintf(str1,4, "[%d]", exit_code);
		if (PWD->length + 6 <= MAXLENPROMPT){
			string_append(PROMPT, str1);
			string_append(PROMPT, PWD->data);
			string_append(PROMPT, "$ ");
		}else{
			string_copy_from_end(PROMPT, PWD, MAXLENPROMPT-2);
			snprintf(str1,7,"[%d]...",exit_code);
			insert_prefixe(PROMPT, str1, 6);
			string_append(PROMPT, "$ ");
		}
		free(str1);
		args = readline(PROMPT->data);
		if (args == NULL){
			exit(exit_code);
		}else if (strlen(args) > 0){
			//add the command line to the history
			add_history(args);
			//-----------------------------------
			splited_args = allocate_splited_string();
			len_splited_args = split_string(args, " ", splited_args);
			if (len_splited_args > MAX_ARGS_NUMBER){
				write(STDERR_FILENO, "\nMAX_ARGS_NUMBER\n", 17);
			}else if(!check_args(splited_args, len_splited_args)){
				write(STDERR_FILENO, "\nMAX_ARGS_STRLEN\n", 17);
			}else{
				int index=-1, len_array = 0;
				char *path = absolute_path(splited_args+1, len_splited_args-1, &index);
				char **all_paths = NULL;
				if (contains(path, '*')) all_paths=star(path, &len_array);
				if(is_intern(splited_args[0]) == 0){// Commandes internes
					if (len_array > 1) write(STDERR_FILENO, "intern : too many arguments\n", strlen("intern : too many arguments\n"));
					else{
						if (strcmp(splited_args[0], "cd") == 0){
							exit_code = slash_cd(splited_args+1, len_splited_args-1);
						}else if (strcmp(splited_args[0], "pwd") == 0){
							exit_code = slash_pwd(splited_args+1, len_splited_args-1);
						}else{
							if (len_splited_args == 2){
								if (is_number(splited_args[1])){
									int exit_parameter = atoi(splited_args[1]);
									free(args);
									string_delete(PROMPT);
									string_delete(PWD);
									string_delete(OLD_PATH);
									free_splited_string(splited_args);
									exit(exit_parameter);
								}else{
									write(STDERR_FILENO, "exit : error must be an integer\n", 32);
								}
							}else if (len_splited_args == 1){
								free(args);
								string_delete(PWD);
								string_delete(OLD_PATH);
								string_delete(PROMPT);
								free_splited_string(splited_args);
								exit(exit_code);
							}else{
								write(STDERR_FILENO, "exit: too many arguments\n", 25);
							}
						}
					}
				}
				else{ // Commandes externes
					int status;
					int i=0;
					char **args_path = malloc(PATH_MAX*sizeof(char*));
					while (i < PATH_MAX){
						args_path[i] = NULL;
						i++;
					}
					int stoped_index = 0;
					for (int i=0;i<len_splited_args;i++){
						if (!contains(splited_args[i], '*')){
							args_path[i] = malloc(strlen(splited_args[i]));
							memmove(args_path[i], splited_args[i], strlen(splited_args[i]));
							args_path[i][strlen(splited_args[i])] = '\0';
							stoped_index++;
						}
					}
					if (all_paths){
						for (int i=stoped_index;i<len_array+stoped_index;i++){
							args_path[i] = malloc(strlen(all_paths[i-stoped_index]));
							memmove(args_path[i], all_paths[i-stoped_index], strlen(all_paths[i-stoped_index]));
							args_path[i][strlen(all_paths[i-stoped_index])] = '\0';
						}
					}
					switch (fork())
					{
					case -1:						
						write(STDERR_FILENO,"fork()", strlen("fork()"));
					break;
					case 0:
						execvp(splited_args[0], args_path);
						break;
					default:
						wait(&status);
						exit_code = WEXITSTATUS(status);
						free_2d_array(args_path);
						break;
					}
				}
				free(path);
				if (all_paths) free_2d_array(all_paths);
			}
			free(args);
			free_splited_string(splited_args);
		}
	}
}
