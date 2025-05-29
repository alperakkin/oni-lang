#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "utils.h"

static Token *advance(Parser *parser);
static Node *parse_primary(Parser *parser);

static Token *advance(Parser *parser)
{
    if (parser->current != NULL)
        parser->current = parser->current->next;

    while (parser->current != NULL && (parser->current->type == TOKEN_SPACE || parser->current->type == TOKEN_NEW_LINE))
    {
        parser->current = parser->current->next;
    }

    return parser->current;
}

Node *parse_primary(Parser *parser)
{
    Token *token = parser->current;

    if (token == NULL)
    {
        raise_error("No token found", "");
    }

    if (token->type == TOKEN_INTEGER)
    {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_INTEGER;
        node->integer.value = token->value.int_val;

        advance(parser);
        return node;
    }

    if (token->type == TOKEN_IDENTIFIER)
    {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_IDENTIFIER;
        node->identifier.value = token->value.identifier;

        advance(parser);
        return node;
    }

    raise_error("Unexpected token type", token->symbol);
    return NULL;
}

Node *parse_expression(Parser *parser)
{
    if (parser->current->type == TOKEN_BAD)
        raise_error("Syntax Error: Bad Token", parser->current->symbol);
    if (parser->current &&
        parser->current->type == TOKEN_IDENTIFIER)

    {
        Token *identifier_token = parser->current;
        advance(parser);
        if (parser->current->type == TOKEN_FUNCTION_CALL)
            return parse_function_call(parser, identifier_token);
        else
            raise_error("Not Implemented", "Parser Identifier");
    }

    Node *left = parse_primary(parser);

    while (parser->current != NULL)
    {
        Token *token = parser->current;

        if (token->type == TOKEN_PLUS)
        {
            advance(parser);
            Node *right = parse_primary(parser);

            Node *bin_op = malloc(sizeof(Node));
            bin_op->type = NODE_BINARY_OP;
            bin_op->binary_op.left = left;
            bin_op->binary_op.right = right;
            bin_op->binary_op.token = token;

            left = bin_op;
        }
        else
        {
            break;
        }
    }

    return left;
}

Node *parse_function_call(Parser *parser, Token *identifier_token)
{

    Token *arrow_token = parser->current;
    if (arrow_token->type != TOKEN_FUNCTION_CALL)
    {
        raise_error("Expected ->", arrow_token->symbol);
    }
    advance(parser);

    Node *arg_node = parse_expression(parser);

    Node *identifier_node = malloc(sizeof(Node));
    identifier_node->type = NODE_IDENTIFIER;
    identifier_node->identifier.value = identifier_token->value.identifier;

    Node *func_call = malloc(sizeof(Node));
    func_call->type = NODE_FUNCTION_CALL;
    func_call->func_call.left = identifier_node;
    func_call->func_call.right = arg_node;
    func_call->func_call.token = arrow_token;

    return func_call;
}

NodeBlock *parse(Parser *parser)
{
    NodeBlock *block = malloc(sizeof(NodeBlock));
    block->statements = NULL;
    block->count = 0;

    while (parser->current != NULL)
    {
        Node *stmt = parse_expression(parser);
        if (stmt != NULL)
        {
            block->count++;
            block->statements = realloc(block->statements, sizeof(Node *) * block->count);
            block->statements[block->count - 1] = stmt;
        }
    }

    return block;
}

void free_node(Node *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_BINARY_OP:
        free_node(node->binary_op.left);
        free_node(node->binary_op.right);
        break;
    case NODE_FUNCTION_CALL:
        free_node(node->func_call.left);
        free_node(node->func_call.right);
        break;
    default:
        break;
    }

    free(node);
}

void print_ast(Node *node, int level)
{
    if (!node)
        return;

    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }

    switch (node->type)
    {
    case NODE_INTEGER:
        printf("Integer: %d\n", node->integer.value);
        break;

    case NODE_IDENTIFIER:
        printf("Identifier: %s\n", node->identifier.value);
        break;

    case NODE_BINARY_OP:
        printf("BinaryOp: %s\n",
               node->binary_op.token->type == TOKEN_PLUS ? "+" : "UnknownOp");

        print_ast(node->binary_op.left, level + 1);
        print_ast(node->binary_op.right, level + 1);
        break;

    case NODE_FUNCTION_CALL:
        printf("FunctionCall: %s\n", "->");

        print_ast(node->func_call.left, level + 1);
        print_ast(node->func_call.right, level + 1);
        break;

    default:
        printf("Unknown node type\n");
    }
}

void print_ast_block(NodeBlock *block)
{
    for (int i = 0; i < block->count; i++)
    {
        print_ast(block->statements[i], 0);
    }
}
