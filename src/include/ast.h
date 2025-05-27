#pragma once
#include "lexer.h"

typedef enum
{
    NODE_INTEGER,
    NODE_BINARY_OP
} NodeType;

typedef struct Node
{
    NodeType type;
    union
    {
        struct
        {
            int value;
        } integer;

        struct
        {
            struct Node *left;
            struct Node *right;
            struct Token *token;
        } binary_op;
    };
} Node;

typedef struct
{
    Token *current;
} Parser;

Node *parse_expression(Parser *parser);
Node *parse(Parser *parser);
void free_node(Node *node);
void print_ast(Node *node, int level);