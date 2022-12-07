#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <limits.h>
#include <wait.h>
#include "cd.h"
#include "pwd.h"
#include "split_string.h"
#include "mystring.h"

#define MAXLENPROMPT 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
char * INTERN_COMMAND[3] =  {"pwd","cd","exit"};

int is_intern(char *str){
	for(int i = 0; i<3; i++){
		if(strcmp(str,*(INTERN_COMMAND + i)) == 0){
			return 0;
		}
	}
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


int main(int argc, char **argv){
	int exit_code = 0;
	rl_outstream = stderr;
	PWD = string_new(PATH_MAX);
	OLD_PATH = string_new(PATH_MAX);
	struct string *PROMPT = string_new(MAXLENPROMPT + 1);
	char *args , **splited_args;
	maj_PWD_P();
	string_cpy(OLD_PATH, PWD);
	if (PROMPT == NULL){
		write(STDERR_FILENO, "erreur : string_new", 20);
		exit(EXIT_FAILURE);
	}
	while(1){
		init_string(PROMPT);
		int len_splited_args;
		args = NULL;
		if (PWD->length + 6 <= MAXLENPROMPT){
			string_append(PROMPT, "[0]");
			string_append(PROMPT, PWD->data);
			string_append(PROMPT, "$ ");
		}else{
			string_copy_from_end(PROMPT, PWD, MAXLENPROMPT-2);
			insert_prefixe(PROMPT, "[0]...", 6);
			string_append(PROMPT, "$ ");
		}
		args = readline(PROMPT->data);
		if (args == NULL){
			exit(EXIT_SUCCESS);
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
				if(is_intern(splited_args[0]) == 0){// Commande interne
					if (strcmp(splited_args[0], "cd") == 0){
						exit_code = slash_cd(splited_args+1, len_splited_args-1);
					}else if (strcmp(splited_args[0], "pwd") == 0){
						exit_code = slash_pwd(splited_args+1, len_splited_args-1);
					}else{
						if (len_splited_args == 2){
							if (is_number(splited_args[1])){
								int exitCode = atoi(splited_args[1]);
								free(args);
								string_delete(PROMPT);
								string_delete(PWD);
								string_delete(OLD_PATH);
								free_splited_string(splited_args);
								exit(exitCode);
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
				else{
					switch (fork())
					{
					case -1:
						write(STDERR_FILENO,"fork()", strlen("fork()"));
						break;
					case 0:
						execvp(splited_args[0], splited_args);
						break;
					
					default:
						wait(NULL);
						break;
					}
				}
			}
			free(args);
			free_splited_string(splited_args);
		}
	}
}
