#include <stdio.h>
#include "lexer.h"
#include "ast.h"

int main(int argc, char **argv)
{
    const char *source = "4 + 5 + 10";
    Token *tokens = tokenize(source);
    print_tokens(tokens);
    Parser parser;
    parser.current = tokens;
    Node *ast = parse_expression(&parser);
    print_ast(ast, 0);

    free_tokens(tokens);
    free_node(ast);
    return 0;
}