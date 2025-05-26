#include <stdio.h>
#include "lexer.h"

int main(int argc, char **argv)
{
    Token *tokens = tokenize("123 456%1\n22");
    print_tokens(tokens);
    free_tokens(tokens);
    return 0;
}