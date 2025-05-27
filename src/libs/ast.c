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

    while (parser->current != NULL && parser->current->type == TOKEN_SPACE)
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

    raise_error("Unexpected token type", token->symbol);
    return NULL;
}

Node *parse_expression(Parser *parser)
{
    Node *left = parse_primary(parser);

    while (parser->current != NULL)
    {
        Token *token = parser->current;

        if (token->type == TOKEN_PLUS)
        {

            if (parser->current == NULL)
            {
                raise_error("Expected expression not found", "+");
            }
            advance(parser);
            Node *right = parse_primary(parser);

            Node *bin_op = malloc(sizeof(Node));
            bin_op->type = NODE_BINARY_OP;
            bin_op->binary_op.left = left;
            bin_op->binary_op.right = right;
            bin_op->binary_op.token = token;

            left = bin_op;
        }
        else if (token->type == TOKEN_BAD)
        {
            raise_error("Syntax error", token->symbol);
        }
        else
        {
            advance(parser);
        }
    }

    return left;
}

Node *parse(Parser *parser)
{
    return parse_expression(parser);
}

void free_node(Node *node)
{
    if (!node)
        return;

    if (node->type == NODE_BINARY_OP)
    {
        free_node(node->binary_op.left);
        free_node(node->binary_op.right);
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

    case NODE_BINARY_OP:
        printf("BinaryOp: %s\n",
               node->binary_op.token->type == TOKEN_PLUS ? "+" : node->binary_op.token->type == TOKEN_MINUS ? "-"
                                                                                                            : "UnknownOp");

        print_ast(node->binary_op.left, level + 1);
        print_ast(node->binary_op.right, level + 1);
        break;

    default:
        printf("Unknown node type\n");
    }
}
