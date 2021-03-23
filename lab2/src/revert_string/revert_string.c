#include "revert_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void RevertString(char *str)
{
	char* old = malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(old, str);
    for(int i = 0; i < strlen(old); i++)
    {
        *(str + i) = *(old + (strlen(old) - 1 - i));
    }
    free(old);
}
