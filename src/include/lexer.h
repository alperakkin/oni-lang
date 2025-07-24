#include <stdbool.h>
#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TK_INTEGER,    // 00
    TK_FLOAT,      // 01
    TK_BOOL,       // 02
    TK_IDENTIFIER, // 03
    TK_PLUS,       // 04
    TK_MINUS,      // 05
    TK_STAR,       // 06
    TK_SLASH,      // 07
    TK_ASSIGN,     // 08
    TK_RPAREN,     // 09
    TK_EOF,        // 10
    TK_BAD,        // 11
    TK_SPACE,      // 12
    TK_COMMA,      // 13
    TK_NEW_LINE,   // 14
    TK_L_PAREN,    // 15
    TK_R_PAREN,    // 16
    TK_L_SQUARE,   // 17
    TK_R_SQUARE,   // 18
    TK_L_CURL,     // 19
    TK_R_CURL,     // 20
    TK_LT,         // 21
    TK_LTE,        // 22
    TK_GT,         // 23
    TK_GTE,        // 24
    TK_PUSH,       // 25
    TK_QUOTE,      // 26
    TK_STRING,     // 27
    TK_COMMENT,    // 28
    TK_COMP,       // 29
    TK_NULL,       // 30
    TK_WHILE,      // 31
    TK_FOR,        // 32
    TK_TO,         // 33
    TK_IN,         // 34
    TK_OF,         // 35
    TK_IF,         // 36
    TK_ELSE,       // 37
    TK_EQ,         // 38
    TK_NEQ,        // 39
    TK_AND,        // 40
    TK_OR,         // 41
    TK_INC,        // 42
    TK_DECR,       // 43
    TK_FUNC_DEF,   // 44
    TK_RETURN,     // 45
    TK_BREAK,      // 46
    TK_CONTINUE,   // 47
    TK_CLASS,      // 48
    TK_THIS,       // 49
    TK_DOT,        // 50
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