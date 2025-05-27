#include <stdio.h>
#include "lexer.h"
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv)
{
    const char *source = "4 + 5 +7";
    Token *tokens = tokenize(source);
    print_tokens(tokens);
    Parser parser;
    parser.current = tokens;
    Node *ast = parse_expression(&parser);
    print_ast(ast, 0);

    Value result = eval(ast);
    print_value(result);

    free_tokens(tokens);
    free_node(ast);
    return 0;
}