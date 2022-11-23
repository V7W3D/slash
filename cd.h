#ifndef CD_H
#define CD_H

#include "mystring.h"
#include <stdlib.h>

extern struct string *OLD_PATH;
extern struct string *PWD;

int slash_cd(char **args, int len);

#endif
