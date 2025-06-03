#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"
#include "interpreter.h"
#include "utils.h"
#include "variable.h"

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
    // print_ast_block(ast);

    GlobalScope *globals = init_globals();

    for (int i = 0; i < ast->count; i++)
    {
        Value result = interpret(ast->statements[i], globals);
        // print_value(result);
    }
    print_globals(globals);

    free_tokens(tokens);
    free_node(ast);
    // TODO: free globals
    return 0;
}