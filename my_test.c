#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>


int main(void){
	char *opt[3] = {"echo","5", NULL};
	execvp("*cho", opt);
	perror("");
}