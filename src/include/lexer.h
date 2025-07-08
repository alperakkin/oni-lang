#include <stdbool.h>
#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TK_INTEGER,
    TK_FLOAT,
    TK_BOOL,
    TK_IDENTIFIER,
    TK_PLUS,
    TK_MINUS,
    TK_STAR,
    TK_SLASH,
    TK_ASSIGN,
    TK_RPAREN,
    TK_EOF,
    TK_BAD,
    TK_SPACE,
    TK_COMMA,
    TK_NEW_LINE,
    TK_L_PAREN,
    TK_R_PAREN,
    TK_L_SQUARE,
    TK_R_SQUARE,
    TK_L_CURL,
    TK_R_CURL,
    TK_LT,
    TK_LTE,
    TK_GT,
    TK_GTE,
    TK_PUSH,
    TK_QUOTE,
    TK_STRING,
    TK_COMMENT,
    TK_COMP,
    TK_NULL,
    TK_IF,
    TK_ELSE,
    TK_EQ,
    TK_NEQ,
    TK_AND,
    TK_OR,
    TK_INC,
    TK_DECR,
} TokenType;

typedef union
{
    int int_val;
    bool bool_val;
    bool null_val;
    float float_val;
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

void append_token(Token **head, TokenType TK_type, TokenValue value,
                  char *symbol);
void free_tokens(Token *head);
void print_token(Token *token);
void print_tokens(Token *tokens);
void handle_comment(const char *source, int *cursor, Token **head);
void handle_number(const char *source, int *cursor, Token **head);
void handle_plus(const char *source, int *cursor, Token **head);
void handle_minus(const char *source, int *cursor, Token **head);
void handle_string(const char *source, int *cursor, Token **head);
void handle_identifier(const char *source, int *cursor, Token **head);
void handle_equals(const char *source, int *cursor, Token **head);
Token *tokenize(const char *source);
#endif