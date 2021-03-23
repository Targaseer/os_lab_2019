#include "swap.h"
#include <stdio.h>

void Swap(char *left, char *right)
{
	char tmp = *left;
    *left = *right;
    *right = tmp;
}
