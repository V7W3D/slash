#include <readline/readline.h>
#include <readline/history.h>
#include "cd.h"

#define MaxLenPrompt 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_CMDLINE_STRLEN 4096

struct string *CMDLINE = NULL;


int main(int argc, char **argv){
	CMDLINE = string_new(MAX_CMDLINE_STRLEN);
	string_append(CMDLINE, "/home/titi/pas/trop/long/mais/la/ca/depasse");
	string_format(CMDLINE, "...", MaxLenPrompt, 3);
	printf("%s", CMDLINE->data);
	exit(EXIT_SUCCESS);
}