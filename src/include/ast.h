#pragma once
#include "lexer.h"

typedef struct Node Node;

typedef struct
{
    Node **statements;
    int count;
} NodeBlock;

typedef enum
{
    NODE_NUMBER,
    NODE_STRING,
    NODE_BINARY_OP,
    NODE_FUNCTION_CALL,
    NODE_IDENTIFIER,
    NODE_BLOCK,
    NODE_VARIABLE,
    NODE_NULL,
} NodeType;

typedef enum
{
    NODE_INTEGER,
    NODE_FLOAT

} NodeNumberType;
struct Node
{
    NodeType type;
    union
    {
        struct
        {
            char *value;
        } null;
        struct
        {
            int int_value;
            float float_value;
            NodeNumberType type;

        } number;

        struct
        {
            Node *left;
            Node *right;
            Token *token;
        } binary_op;

        struct
        {
            Node *left;
            Node *right;

        } func_call;

        struct
        {
            char *value;
        } identifier;

        struct
        {
            char *value;
        } string;

        struct
        {
            char *type;
            char *name;
            bool local;
            Node *value;
        } variable;

        NodeBlock block;
    };
};

typedef struct
{
    Token *current;
} Parser;

bool is_variable(Token *token);
Node *parse_expression(Parser *parser, int min_precedence);
NodeBlock *parse(Parser *parser);
Node *parse_variable(Parser *parser, Token *identifier_token);
Node *parse_function_call(Parser *parser, Token *identifier_token);
void free_node(Node *node);
void print_ast(Node *node, int level);
void print_ast_block(NodeBlock *block);