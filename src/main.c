#include <stdio.h>
#include "lexer.h"
#include "ast.h"
#include "eval.h"
#include "utils.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        raise_error("Please provide a filename", "");
    const char *source = read_file(argv[1]);
    Token *tokens = tokenize(source);
    // print_tokens(tokens);
    Parser parser;
    parser.current = tokens;
    Node *ast = parse_expression(&parser);
    // print_ast(ast, 0);

    Value result = eval(ast);
    print_value(result);

    free_tokens(tokens);
    free_node(ast);
    return 0;
}