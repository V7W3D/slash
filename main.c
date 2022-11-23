#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "cd.h"

#define MaxLenPrompt 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

static int checkArgs(const char **args, const int len){
	for (int i=0;i<len;i++){
		if (strlen(args[i] > MAX_ARGS_STRLEN)) return 0;
	}
	return 1;
}

int main(int argc, char **argv){
	struct string *PROMPT = string_new(MaxLenPrompt + 1);
	char *args , **splited_args;
	int len_Splited_args;
	//------------
	PWD = string_new(600);
	string_append(PWD, "/home/kali/Desktop/projetsys/sys5-slash");
	//--------------
	if (PROMPT == NULL){
		perror("string_new : ");
		exit(EXIT_FAILURE);
	}
	while(1){
		init_String(PROMPT);
		len_Splited_args = 0;
		splited_args = NULL;
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
		if (args == NULL){
			perror("readline : ");
		}else{
			len_Splited_args = split_string(args, " ", splited_args);
			if (len_Splited_args > MAX_ARGS_NUMBER){
				perror("MAX_ARGS_NUMBER");
			}else if(!checkArgs(splited_args, len_Splited_args)){
				perror("MAX_ARGS_STRLEN");
			}
			else{
				switch (splited_args[0]){
					case "cd":

					break;
					case "pwd":

					break;
					case "exit":
						if (len_Splited_args == 2){ 
							int exitCode = atoi(splited_args[1]);
							exit(exitCode);
						}else if (len_Splited_args == 1){
							exit(EXIT_SUCCESS);
						}else{
							perror("exit: too many arguments");
						}
					break;
					default:
						perror("error in args");
					break;
				}
				free_splited_string(splited_args, len_Splited_args);
				free(args);
			}
		}
	}
	string_delete(PROMPT);
	exit(1);
}