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

char *args, **splited_args, **star_path, **updated_args;
struct string *PROMPT;

static void free_all_ressources(){
	free(args);
	//free_2d_array(star_path);
	string_delete(PWD);
	string_delete(OLD_PATH);
	string_delete(PROMPT);
	//free_2d_array(updated_args);
	free_splited_string(splited_args);
}

int main(int argc, char **argv){
	
	int exit_code = 0;
	rl_outstream = stderr;
	PWD = string_new(PATH_MAX);
	OLD_PATH = string_new(PATH_MAX);
	PROMPT = string_new(MAXLENPROMPT + 1);
	
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
			add_history(args);
			splited_args = allocate_splited_string();
			len_splited_args = split_string(args, " ", splited_args);
			if (len_splited_args > MAX_ARGS_NUMBER){
				write(STDERR_FILENO, "\nMAX_ARGS_NUMBER\n", 17);
			}else if(!check_args(splited_args, len_splited_args)){
				write(STDERR_FILENO, "\nMAX_ARGS_STRLEN\n", 17);
			}else{
				int len_array = 0;
				
				/*star_path = malloc(PATH_MAX * sizeof(char*));
				star(splited_args, len_splited_args, &len_array, star_path);
				updated_args = malloc(PATH_MAX * sizeof(char*));
				for (int i=0;i<PATH_MAX;i++) updated_args[i] = NULL;
				concat(splited_args, star_path, len_splited_args, len_array, updated_args);*/

				updated_args = splited_args;
					

				if(is_intern(updated_args[0]) == 0){// Commandes internes
						if (strcmp(updated_args[0], "cd") == 0){
							exit_code = slash_cd(updated_args+1, len_splited_args+len_array-1);
						}else if (strcmp(updated_args[0], "pwd") == 0){
							exit_code = slash_pwd(updated_args+1, len_splited_args+len_array-1);
						}else{
							if (len_splited_args == 2){
								if (is_number(updated_args[1])){
									int exit_parameter = atoi(updated_args[1]);
									free_all_ressources();
									exit(exit_parameter);
								}else{
									write(STDERR_FILENO, "exit : error must be an integer\n", 32);
								}
							}else if (len_splited_args == 1){
								free_all_ressources();
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
						if (execvp(updated_args[0], updated_args) < 0){
							free_all_ressources();
							exit(errno);
						} 
						break;
					default:
						wait(&status);
						if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
						break;
					}
				}
				free_2d_array(star_path);
				free_2d_array(updated_args);
			}
			free(args);
			free_splited_string(splited_args);
		}
	}
}
