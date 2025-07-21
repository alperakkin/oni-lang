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
    NODE_NUMBER = 1000, // 1000
    NODE_STRING,        // 1001
    NODE_BINARY_OP,     // 1002
    NODE_UNARY_OP,      // 1003
    NODE_FUNCTION_CALL, // 1004
    NODE_IDENTIFIER,    // 1005
    NODE_BLOCK,         // 1006
    NODE_VARIABLE,      // 1007
    NODE_NULL,          // 1008
    NODE_BOOL,          // 1009
    NODE_ARRAY,         // 1010
    NODE_IF,            // 1011
    NODE_WHILE,         // 1012
    NODE_FOR,           // 1013
    NODE_CONTINUE,      // 1014
    NODE_BREAK,         // 1015
    NODE_FUNCTION_DEF,  // 1016
    NODE_RETURN,        // 1017
    NODE_CLASS,         // 1018
    NODE_THIS,          // 1019
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
    Node *operand;
    Token *token;
} NodeUnaryOp;

typedef struct
{
    char *name;
    Node **args;
    int args_count;
    Node **kwargs;
    int kwargs_count;

    NodeBlock *func_block;
    Node *return_type;

} NodeFuncDef;

typedef struct
{
    Node *expression;
} NodeReturn;

typedef struct
{
    char *name;
    Node **args;
    Node **kwargs;
    int args_count;
    int kwargs_count;

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

typedef struct
{
    Node *condition;
    NodeBlock *if_block;
    NodeBlock *else_block;
} NodeIf;

typedef struct
{
    Node *condition;
    NodeBlock *if_block;
} NodeWhile;

typedef struct
{
    Node *iterator;
    Node *iterable;
    Token *token;
    NodeBlock *for_block;
} NodeFor;

typedef struct
{
    char *name;
    NodeBlock *methods;

} NodeClass;

struct Node
{
    NodeType type;
    bool is_iterable;
    union
    {
        NodeNull null;
        NodeBoolean boolean;
        NodeNumber number;
        NodeBinaryOp binary_op;
        NodeUnaryOp unary_op;
        NodeFuncDef func_def;
        NodeFuncCall func_call;
        NodeIdentifier identifier;
        NodeString string;
        NodeVariable variable;
        NodeArray array;
        NodeBlock block;
        NodeIf node_if;
        NodeWhile node_while;
        NodeFor node_for;
        NodeReturn node_return;
        NodeClass node_class;
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
Node *parse_function_definition(Parser *parser);
Node *parse_function_call(Parser *parser, Token *identifier_token);
Node *parse_array_literal(Parser *parser, char *generic_type);
Node *parse_if_block(Parser *parser);
Node *parse_while_block(Parser *parser);
Node *parse_for_block(Parser *parser);
void free_node(Node *node);
void print_node(Node *node, int level);
void print_node_block(NodeBlock *block);