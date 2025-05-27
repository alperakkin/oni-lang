#include <stdio.h>
#include <stdlib.h>

void raise_error(char *msg, char *symbol)
{
    fprintf(stderr, "%s -> %s\n", msg, symbol);
    exit(1);
}