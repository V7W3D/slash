#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <linux/limits.h>
#include <wait.h>
#include <errno.h>
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
				int i=0;
				int len_array = 0;
				char **all_paths = malloc(PATH_MAX * sizeof(char*));
				star(splited_args, len_splited_args, &len_array, all_paths);
				char **args_path = malloc(PATH_MAX*sizeof(char*));
					while (i < PATH_MAX){
					args_path[i] = NULL;
					i++;
				}
				concat(splited_args, all_paths,len_splited_args, len_array, args_path);
				char *valid_command = (contains(splited_args[0], '*')?all_paths[0]:splited_args[0]);
				if(is_intern(valid_command) == 0){// Commandes internes
						if (strcmp(valid_command, "cd") == 0){
							exit_code = slash_cd(args_path+1, len_splited_args-1);
						}else if (strcmp(valid_command, "pwd") == 0){
							exit_code = slash_pwd(args_path+1, len_splited_args-1);
						}else{
							if (len_splited_args == 2){
								if (is_number(args_path[1])){
									int exit_parameter = atoi(args_path[1]);
									free(args);
									string_delete(PROMPT);
									string_delete(PWD);
									string_delete(OLD_PATH);
									free_splited_string(splited_args);
									free_2d_array(all_paths);
									free_2d_array(args_path);
									exit(exit_parameter);
								}else{
									write(STDERR_FILENO, "exit : error must be an integer\n", 32);
								}
							}else if (len_splited_args == 1){
								free(args);
								free_2d_array(args_path);
								string_delete(PWD);
								string_delete(OLD_PATH);
								string_delete(PROMPT);
								free_2d_array(all_paths);
								free_splited_string(splited_args);
								exit(exit_code);
							}else{
								write(STDERR_FILENO, "exit: too many arguments\n", 25);
							}
						}
				}
				else{ // Commandes externes
					int status;
					switch (fork())
					{
					case -1:						
						write(STDERR_FILENO,"fork()", strlen("fork()"));
					break;
					case 0:
						if (execvp((contains(splited_args[0],'*')?all_paths[0]:splited_args[0])
							, args_path) < 0){
							free(args);
							free_2d_array(args_path);
							string_delete(PWD);
							string_delete(OLD_PATH);
							string_delete(PROMPT);
							free_2d_array(all_paths);
							free_splited_string(splited_args);
							exit(errno);
						} 
						break;
					default:
						wait(&status);
						if (WIFEXITED(status))
							exit_code = WEXITSTATUS(status);
						break;
					}
				}
				free_2d_array(args_path);
				free_2d_array(all_paths);
			}
			free(args);
			free_splited_string(splited_args);
		}
	}
}
