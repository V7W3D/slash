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
#include "double_star.h"
#include "redirections.h"

char *args;
struct string *PROMPT;

/*
static void free_all_ressources(){
	free(args);
	string_delete(PWD);
	string_delete(OLD_PATH);
	string_delete(PROMPT);
}
*/

int main(int argc, char **argv){
	
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
			pipeline(args);
			free(args);
		}
	}
}
