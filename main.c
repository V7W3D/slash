#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <limits.h>
#include "cd.h"
#include "pwd.h"
#include "split_string.h"
#include "mystring.h"

#define MaxLenPrompt 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

static int checkArgs(char **args, int len){
	for (int i=0;i<len;i++){
		if (strlen(args[i]) > MAX_ARGS_STRLEN) return 0;
	}
	return 1;
}

static int isNumber(char *str){
	for (int i=0;i<strlen(str);i++){
		if (!isdigit(str[i])) return 0;
	}
	return 1;
}

static void addLigneToHistory(struct string *PROMPT,char *command){
	struct string *linecmd = string_new(PROMPT->length + strlen(command) + 1);
	string_append(linecmd, PROMPT->data);
	string_append(linecmd, command);
	add_history(linecmd->data);
	string_delete(linecmd);
}

int main(int argc, char **argv){
	dup2(2, 1);
	PWD = string_new(PATH_MAX);
	OLD_PATH = string_new(PATH_MAX);
	struct string *PROMPT = string_new(MaxLenPrompt + 1);
	char *args , **splited_args;
	int len_Splited_args;
	splited_args = malloc(sizeof(char*));
	if (splited_args == NULL){
		write(STDERR_FILENO, "erreur : malloc", 16);
		exit(EXIT_FAILURE);
	}
	maj_PWD_P();
	string_cpy(OLD_PATH, PWD);
	if (PROMPT == NULL){
		write(STDERR_FILENO, "erreur : string_new", 20);
		exit(EXIT_FAILURE);
	}
	while(1){
		init_String(PROMPT);
		len_Splited_args = 0;
		args = NULL;
		if (PWD->length + 2 <= MaxLenPrompt){
			string_append(PROMPT, PWD->data);
			string_append(PROMPT, "$");
		}else{
			string_n_copy_from_end(PROMPT, PWD, MaxLenPrompt-1);
			insert_prefixe(PROMPT, "...", 3);
			string_append(PROMPT, "$");
		}
		write(STDERR_FILENO, PROMPT->data, PROMPT->length);
		args = readline(" ");
		if (args == NULL){
			exit(EXIT_FAILURE);
		}else if (strlen(args) > 0){
			//add the command line to the history
			addLigneToHistory(PROMPT, args);
			//-----------------------------------
			len_Splited_args = split_string(args, " ", splited_args);
			if (len_Splited_args > MAX_ARGS_NUMBER){
				write(STDERR_FILENO, "\nMAX_ARGS_NUMBER\n", 17);
			}else if(!checkArgs(splited_args, len_Splited_args)){
				write(STDERR_FILENO, "\nMAX_ARGS_STRLEN\n", 17);
			}else{
				if (strcmp(splited_args[0], "cd") == 0){
					if (slash_cd(splited_args+1, len_Splited_args-1) == 0){
						write(STDERR_FILENO, PWD->data, PWD->length);
						write(STDERR_FILENO, "\n", 1);
					}
				}else if (strcmp(splited_args[0], "pwd") == 0){
					if (slash_pwd(splited_args+1, len_Splited_args-1) != -1){
						write(STDERR_FILENO, PWD->data, PWD->length);
						write(STDERR_FILENO, "\n", 1);
					}
				}else if (strcmp(splited_args[0], "exit") == 0){
					if (len_Splited_args == 2){
							if (isNumber(splited_args[1])){
								int exitCode = atoi(splited_args[1]);
								free(args);
								string_delete(PROMPT);
								free_splited_string(splited_args, len_Splited_args);
								exit(exitCode);
							}else{
								write(STDERR_FILENO, "exit : error must be an integer\n", 32);
							}
						}else if (len_Splited_args == 1){
							free(args);
							string_delete(PROMPT);
							free_splited_string(splited_args, len_Splited_args);
							exit(EXIT_SUCCESS);
						}else{
							write(STDERR_FILENO, "exit: too many arguments\n", 25);
						}
				}else{
					write(STDERR_FILENO, "command not found\n", 19);
				}
			}
		}
	}
}