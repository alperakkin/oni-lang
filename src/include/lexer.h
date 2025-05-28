#include <stdbool.h>
#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TOKEN_INTEGER,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_BAD,
    TOKEN_SPACE,
    TOKEN_FUNCTION_CALL,
    TOKEN_COMMA,
    TOKEN_NEW_LINE,
} TokenType;

typedef union
{
    int int_val;
    double float_val;
    char *text_val;
    char *identifier;
} TokenValue;

typedef struct Token
{
    TokenType type;
    TokenValue value;
    char *symbol;
    struct Token *next;
} Token;

bool is_function(const char *source, int *cursor);
void append_token(Token **head, TokenType token_type, TokenValue value,
                  char *symbol);
void free_tokens(Token *head);
void print_tokens(Token *tokens);
void handle_number(const char *source, int *cursor, Token **head);
void handle_plus(int *cursor, Token **head);
void handle_identifier(const char *source, int *cursor, Token **head);
Token *tokenize(const char *source);
#endif