#include <readline/readline.h>
#include <readline/history.h>
#include "cd.h"

#define MaxLenPrompt 30
#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

int main(int argc, char **argv){
	while(1){
		readline("monprompt");
	}
	exit(EXIT_SUCCESS);
}