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
    NODE_BOOL,
    NODE_ARRAY,
} NodeType;

typedef enum
{
    NODE_INTEGER,
    NODE_FLOAT

} NodeNumberType;

typedef struct
{
    Node **elements;
    int capacity;
    int length;
    char *generic_type;
} NodeArray;

typedef struct
{
    bool value;
} NodeNull;
typedef struct
{
    bool value;
} NodeBoolean;

typedef struct
{
    int int_value;
    float float_value;
    NodeNumberType type;

} NodeNumber;

typedef struct
{
    Node *left;
    Node *right;
    Token *token;
} NodeBinaryOp;

typedef struct
{
    Node *left;
    Node *right;

} NodeFuncCall;

typedef struct
{
    char *value;
} NodeIdentifier;
typedef struct
{
    char *value;
} NodeString;

typedef struct
{
    char *type;
    char *name;
    char *generic_type;
    bool local;
    Node *value;
} NodeVariable;
struct Node
{
    NodeType type;
    union
    {
        NodeNull null;
        NodeBoolean boolean;
        NodeNumber number;
        NodeBinaryOp binary_op;
        NodeFuncCall func_call;
        NodeIdentifier identifier;
        NodeString string;
        NodeVariable variable;
        NodeArray array;
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
Node *create_node_array(char *generic_type);
void node_array_push(Node *array_node, Node *element);
Node *parse_variable(Parser *parser, Token *identifier_token);
Node *parse_function_call(Parser *parser, Token *identifier_token);
Node *parse_array_literal(Parser *parser, char *generic_type);
void free_node(Node *node);
void print_ast(Node *node, int level);
void print_ast_block(NodeBlock *block);