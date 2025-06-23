#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "utils.h"
#include "variable.h"

#define INITIAL_ARRAY_CAPACITY 4

static Token *advance(Parser *parser);
static Node *parse_primary(Parser *parser);

int get_precedence(TokenType type)
{
    switch (type)
    {
    case TK_PLUS:
    case TK_MINUS:
        return 1;
    case TK_STAR:
    case TK_SLASH:
        return 2;
    default:
        return 0;
    }
}

static Token *advance(Parser *parser)
{

    if (parser->current != NULL && parser->current->type != TK_COMMENT)
        parser->current = parser->current->next;

    while (parser->current != NULL && (parser->current->type == TK_SPACE || parser->current->type == TK_COMMENT))
    {

        parser->current = parser->current->next;
    }

    return parser->current;
}

bool is_variable(Token *token)
{
    char *identifier = token->value.identifier;

    char *var_list[] = {"int", "float", "str", "bool", "arr"};
    size_t var_len = sizeof(var_list) / sizeof(var_list[0]);
    for (size_t i = 0; i < var_len; i++)
    {
        if (strcmp(identifier, var_list[i]) == 0)
            return true;
    }
    return false;
}

Node *parse_primary(Parser *parser)
{

    Token *token = parser->current;

    if (token == NULL)
        raise_error("No token found", "");

    if (token->type == TK_L_PAREN)
    {
        advance(parser);
        if (parser->current == NULL)
            raise_error("Expected expression after '('", token->symbol);

        Node *expr = parse_expression(parser, 0);

        if (parser->current == NULL || parser->current->type != TK_R_PAREN)
            raise_error("Expected closing parenthesis", token->symbol);

        advance(parser);
        return expr;
    }

    if (token->type == TK_INTEGER)
    {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_NUMBER;
        node->number.int_value = token->value.int_val;
        node->number.type = NODE_INTEGER;

        advance(parser);
        return node;
    }
    if (token->type == TK_FLOAT)
    {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_NUMBER;
        node->number.float_value = token->value.float_val;
        node->number.type = NODE_FLOAT;

        advance(parser);
        return node;
    }
    if (token->type == TK_STRING)
    {
        advance(parser);
        Node *node = malloc(sizeof(Node));
        node->type = NODE_STRING;
        node->string.value = strdup(token->value.text_val);
        return node;
    }
    if (token->type == TK_BOOL)
    {
        advance(parser);
        Node *node = malloc(sizeof(Node));
        node->type = NODE_BOOL;
        node->boolean.value = token->value.bool_val;
        return node;
    }
    if (token->type == TK_NULL)
    {
        advance(parser);
        Node *node = malloc(sizeof(Node));
        node->type = NODE_NULL;
        node->boolean.value = token->value.null_val;
        return node;
    }

    if (token->type == TK_IDENTIFIER)
    {
        Token *identifier_token = token;
        Token *next = token->next;
        while (next && (next->type == TK_SPACE || next->type == TK_NEW_LINE))
            next = next->next;

        if (next && next->type == TK_L_PAREN)
        {
            return parse_function_call(parser, identifier_token);
        }
        else if (is_variable(identifier_token) && next && next->type != TK_GT)
        {

            return parse_variable(parser, identifier_token);
        }

        advance(parser);
        Node *node = malloc(sizeof(Node));
        node->type = NODE_IDENTIFIER;
        node->identifier.value = identifier_token->value.identifier;
        return node;
    }

    char symbol[2];
    symbol[0] = *token->symbol;
    symbol[1] = '\0';
    printf("---ERR---\n");
    print_token(token);
    raise_error("Unexpected token type", symbol);
    return NULL;
}

Node *parse_expression(Parser *parser, int min_precedence)
{
    if (parser->current == NULL)
        raise_error("Unexpected end of expression", "");
    if (parser->current->type == TK_BAD)
    {
        raise_error("Syntax Error: Bad Token", parser->current->symbol);
    }

    Node *left = parse_primary(parser);

    while (parser->current != NULL)
    {

        Token *token = parser->current;

        if (token->type == TK_NEW_LINE || token->type == TK_EOF || token->type == TK_COMMENT)
            break;

        if (token->type == TK_R_PAREN)
            break;
        int precedence = get_precedence(token->type);
        if (precedence < min_precedence)
            break;

        advance(parser);
        Node *right = parse_expression(parser, precedence + 1);

        Node *bin_op = malloc(sizeof(Node));
        bin_op->type = NODE_BINARY_OP;
        bin_op->binary_op.left = left;
        bin_op->binary_op.right = right;
        bin_op->binary_op.token = token;

        left = bin_op;
    }

    return left;
}
Node *create_node_array(char *generic_type)
{

    Node *node = malloc(sizeof(Node));
    node->type = NODE_ARRAY;
    node->array.capacity = INITIAL_ARRAY_CAPACITY;
    node->array.length = 0;
    node->array.elements = malloc(sizeof(Node *) * node->array.capacity);
    node->array.generic_type = strdup(generic_type);
    return node;
}

void node_array_push(Node *array_node, Node *element)
{
    if (array_node->type != NODE_ARRAY)
        raise_error("Internal error: node_array_push on non-array node", "");

    if (array_node->array.length >= array_node->array.capacity)
    {
        array_node->array.capacity *= 2;
        array_node->array.elements = realloc(
            array_node->array.elements,
            sizeof(Node *) * array_node->array.capacity);

        if (!array_node->array.elements)
            raise_error("Memory allocation failed during array growth", "");
    }

    array_node->array.elements[array_node->array.length++] = element;
}

Node *parse_array_literal(Parser *parser, char *generic_type)
{

    if (parser->current == NULL || parser->current->type != TK_L_SQUARE)
        raise_error("Expected '[' to start array literal", parser->current ? parser->current->symbol : "");

    advance(parser);

    Node *node_array = create_node_array(generic_type);

    while (parser->current && parser->current->type != TK_R_SQUARE)
    {
        while (parser->current &&
               (parser->current->type == TK_SPACE || parser->current->type == TK_NEW_LINE || parser->current->type == TK_COMMENT))
        {
            advance(parser);
        }

        if (!parser->current)
            raise_error("Unexpected end of input in array literal", "");

        Node *element = NULL;
        Token *tok = parser->current;

        if (strcmp(generic_type, "int") == 0)
        {
            if (tok->type != TK_INTEGER)
                raise_error("Expected integer in int[]", tok->symbol);

            element = malloc(sizeof(Node));
            element->type = NODE_NUMBER;
            element->number.int_value = tok->value.int_val;
            element->number.type = NODE_INTEGER;
        }
        else if (strcmp(generic_type, "float") == 0)
        {
            if (tok->type != TK_FLOAT)
                raise_error("Expected float in float[]", tok->symbol);

            element = malloc(sizeof(Node));
            element->type = NODE_NUMBER;
            element->number.float_value = tok->value.float_val;
            element->number.type = NODE_FLOAT;
        }
        else if (strcmp(generic_type, "str") == 0)
        {
            if (tok->type != TK_STRING)
                raise_error("Expected string in str[]", tok->symbol);

            element = malloc(sizeof(Node));
            element->type = NODE_STRING;
            element->string.value = strdup(tok->value.text_val);
        }
        else if (strcmp(generic_type, "bool") == 0)
        {
            if (tok->type != TK_BOOL)
                raise_error("Expected boolean in bool[]", tok->symbol);

            element = malloc(sizeof(Node));
            element->type = NODE_BOOL;
            element->boolean.value = tok->value.bool_val;
        }
        else if (strcmp(generic_type, "null") == 0)
        {
            if (tok->type != TK_NULL)
                raise_error("Expected null in null[]", tok->symbol);

            element = malloc(sizeof(Node));
            element->type = NODE_NULL;
            element->null.value = true;
        }
        else
        {
            raise_error("Unknown generic type for array", generic_type);
        }

        node_array_push(node_array, element);
        advance(parser);

        while (parser->current &&
               (parser->current->type == TK_SPACE || parser->current->type == TK_NEW_LINE || parser->current->type == TK_COMMENT))
        {
            advance(parser);
        }

        if (parser->current && parser->current->type == TK_COMMA)
        {
            advance(parser);
        }
        else
        {
            break;
        }
    }

    if (!parser->current || parser->current->type != TK_R_SQUARE)
        raise_error("Expected ']' to close array literal", parser->current ? parser->current->symbol : "");

    advance(parser);
    return node_array;
}

Node *parse_variable(Parser *parser, Token *identifier_token)
{
    char *generic_type = NULL;

    if (strcmp(identifier_token->symbol, "arr") == 0)
    {
        advance(parser);
        if (parser->current->type != TK_LT)
            raise_error("Generic type must be provided", "");

        advance(parser);
        if (parser->current->type != TK_IDENTIFIER)
            raise_error("Expected identifier inside '<>' for generic type", parser->current->symbol);

        generic_type = strdup(parser->current->value.identifier);

        advance(parser);
        if (parser->current->type != TK_GT)
            raise_error("Generic type must be closed by '>", "");
    }
    else
    {
        generic_type = strdup(identifier_token->value.identifier);
    }
    advance(parser);

    Token *token = parser->current;

    if (token->type != TK_IDENTIFIER)
        raise_error("Variable name not provided", token->symbol);
    Token *var_name = token;
    var_name->type = identifier_token->type;

    advance(parser);
    token = parser->current;

    Node *assigned_val = NULL;
    if (token->type == TK_ASSIGN)
    {
        advance(parser);

        if (parser->current->type == TK_L_SQUARE)
        {
            if (generic_type == NULL)
                raise_error("Undefined generic type", "");

            assigned_val = parse_array_literal(parser, generic_type);
        }
        else
        {

            assigned_val = parse_expression(parser, 0);
        }
    }
    else
    {
        assigned_val = malloc(sizeof(Node));
        assigned_val->type = NODE_NULL;
        assigned_val->null.value = true;
    }

    Node *variable_node = malloc(sizeof(Node));

    variable_node->type = NODE_VARIABLE;

    variable_node->variable.name = var_name->value.identifier;
    variable_node->variable.value = assigned_val;
    variable_node->variable.type = identifier_token->value.identifier;
    variable_node->variable.generic_type = generic_type;

    return variable_node;
}

Node *parse_function_call(Parser *parser, Token *identifier_token)
{
    advance(parser);

    if (parser->current == NULL || parser->current->type != TK_L_PAREN)
    {
        raise_error("Expected '(' after ->", parser->current ? parser->current->symbol : "");
    }

    advance(parser);

    Node *arg_expr = parse_expression(parser, 0);

    if (parser->current == NULL || parser->current->type != TK_R_PAREN)
    {
        raise_error("Expected ')' after expression in function call", parser->current ? parser->current->symbol : "");
    }

    advance(parser);

    Node *identifier_node = malloc(sizeof(Node));
    identifier_node->type = NODE_IDENTIFIER;
    identifier_node->identifier.value = identifier_token->value.identifier;

    Node *func_call = malloc(sizeof(Node));
    func_call->type = NODE_FUNCTION_CALL;
    func_call->func_call.left = identifier_node;
    func_call->func_call.right = arg_expr;

    return func_call;
}

NodeBlock *parse(Parser *parser)
{
    NodeBlock *block = malloc(sizeof(NodeBlock));
    block->statements = NULL;
    block->count = 0;
    while (parser->current != NULL)
    {

        if (parser->current->type == TK_NEW_LINE || parser->current->type == TK_COMMENT)
        {
            advance(parser);
            continue;
        }

        Node *stmt = parse_expression(parser, 0);
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
    case NODE_NUMBER:
        if (node->number.type == NODE_INTEGER)
            printf("NUMBER (INT): %d\n", node->number.int_value);
        else if (node->number.type == NODE_FLOAT)
            printf("NUMBER (FLOAT): %f\n", node->number.float_value);
        break;

    case NODE_IDENTIFIER:
        printf("Identifier: %s\n", node->identifier.value);
        break;

    case NODE_BINARY_OP:

        printf("BinaryOp: %s\n",
               node->binary_op.token->symbol);

        print_ast(node->binary_op.left, level + 1);
        print_ast(node->binary_op.right, level + 1);
        break;

    case NODE_FUNCTION_CALL:
        printf("FunctionCall: %s\n", "->");

        print_ast(node->func_call.left, level + 1);
        print_ast(node->func_call.right, level + 1);
        break;
    case NODE_VARIABLE:
        printf("Variable: %s (%s) \n", node->variable.name, node->variable.type);
        print_ast(node->variable.value, level + 1);
        break;
    case NODE_BOOL:
        printf("Bool: %s\n", node->boolean.value == 1 ? "true" : "false");
        break;
    case NODE_ARRAY:
        printf("Array: %d elements\n", node->array.length);
    case NODE_NULL:
        printf("Null\n");
        break;
    default:
        printf(": %d\n", node->type);
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
