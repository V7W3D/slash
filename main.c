#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "cd.h"
#include "split_string.h"

#define MaxLenPrompt 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

static int checkArgs(char **args, int len){
	for (int i=0;i<len;i++){
		if (strlen(args[i]) > MAX_ARGS_STRLEN) return 0;
	}
	return 1;
}

int main(int argc, char **argv){
	struct string *PROMPT = string_new(MaxLenPrompt + 1);
	char *args , **splited_args;
	int len_Splited_args;
	splited_args = malloc(sizeof(char*));
	if (splited_args == NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	PWD = string_new(600);
	string_append(PWD, "/home/kali/Desktop/projetsys/sys5-slash");
	if (PROMPT == NULL){
		perror("string_new : ");
		exit(EXIT_FAILURE);
	}
	while(1){
		init_String(PROMPT);
		len_Splited_args = 0;
		args = NULL;
		if (PWD->length + 2 <= MaxLenPrompt){
			string_append(PROMPT, PWD->data);
			string_append(PROMPT, "$ ");
		}else{
			string_n_copy_from_end(PROMPT, PWD, MaxLenPrompt-2);
			insert_prefixe(PROMPT, "...", 3);
			string_append(PROMPT, "$ ");
		}
		write(STDERR_FILENO, PROMPT->data, PROMPT->length);
		args = readline("");
		if (args == NULL || strlen(args) == 0){
			write(STDERR_FILENO, "erreur readline\n", 16);
		}else{
			len_Splited_args = split_string(args, " ", splited_args);
			if (len_Splited_args > MAX_ARGS_NUMBER){
				write(STDERR_FILENO, "MAX_ARGS_NUMBER\n", 16);
			}else if(!checkArgs(splited_args, len_Splited_args)){
				write(STDERR_FILENO, "MAX_ARGS_STRLEN\n", 16);
			}
			else{
				if (strcmp(splited_args[0], "cd") == 0){
					
				}else if (strcmp(splited_args[0], "pwd") == 0){

				}else if (strcmp(splited_args[0], "exit") == 0){
					if (len_Splited_args == 2){ 
							int exitCode = atoi(splited_args[1]);
							free(args);
							string_delete(PROMPT);
							free_splited_string(splited_args, len_Splited_args);
							exit(exitCode);
						}else if (len_Splited_args == 1){
							free(args);
							string_delete(PROMPT);
							free_splited_string(splited_args, len_Splited_args);
							exit(EXIT_SUCCESS);
						}else{
							write(STDERR_FILENO, "too many args\n", 15);
						}
				}else{
					write(STDERR_FILENO, "error in the arguments\n", 23);
				}
			}
		}
	}
}