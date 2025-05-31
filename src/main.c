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
    NodeBlock *ast = parse(&parser);
    print_ast_block(ast);
    for (int i = 0; i < ast->count; i++)
    {
        Value result = eval(ast->statements[i]);
        // print_value(result);
    }

    free_tokens(tokens);
    free_node(ast);
    return 0;
}