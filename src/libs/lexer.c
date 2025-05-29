#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

void append_token(Token **head, TokenType token_type, TokenValue value,
                  char *symbol)
{
    Token *new_token = malloc(sizeof(Token));
    if (!new_token)
        return;
    new_token->type = token_type;
    new_token->value = value;
    new_token->symbol = symbol ? strdup(symbol) : NULL;
    new_token->next = NULL;

    if (*head == NULL)
        *head = new_token;
    else
    {
        Token *temp = *head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_token;
    }
}
void free_tokens(Token *head)
{
    Token *current = head;
    while (current != NULL)
    {
        Token *next = current->next;
        if (current->symbol)
            free(current->symbol);
        free(current);
        current = next;
    }
}

void print_tokens(Token *tokens)
{
    Token *tmp = tokens;
    while (tmp != NULL)
    {
        if (tmp->type == TOKEN_INTEGER)
        {

            printf("TOKEN [INTEGER] -> %d\n", tmp->value.int_val);
        }
        else if (tmp->type == TOKEN_PLUS)
        {
            printf("TOKEN [PLUS] -> +\n");
        }
        else if (tmp->type == TOKEN_BAD)
        {
            printf("Undefined Token -> %s\n", tmp->symbol);
        }
        else if (tmp->type == TOKEN_SPACE)
        {
            printf("TOKEN [SPACE] -> %s\n", tmp->symbol);
        }
        else if (tmp->type == TOKEN_IDENTIFIER)
        {
            printf("TOKEN [IDENTIFIER] -> %s\n", tmp->symbol);
        }
        else if (tmp->type == TOKEN_FUNCTION_CALL)
        {
            printf("TOKEN [FUNCTION] -> %s\n", tmp->symbol);
        }
        else if (tmp->type == TOKEN_NEW_LINE)
        {
            printf("TOKEN [NEW LINE] -> %s\n", tmp->symbol);
        }
        else
        {
            printf("Unexpected Error!!\n");
        }

        tmp = tmp->next;
    }
}

void handle_integer(const char *source, int *cursor, Token **head)
{
    TokenValue value = {.int_val = 0};

    while (isdigit(source[*cursor]))
    {
        value.int_val = value.int_val * 10 + (source[*cursor] - '0');
        (*cursor)++;
    }

    char symbol[32];
    sprintf(symbol, "%d", value.int_val);
    append_token(head, TOKEN_INTEGER, value, symbol);
}

void handle_plus(int *cursor, Token **head)
{
    (*cursor)++;
    append_token(head, TOKEN_PLUS, (TokenValue){0}, "+\0");
}
void handle_minus(int *cursor, Token **head)
{

    (*cursor)++;
    append_token(head, TOKEN_MINUS, (TokenValue){0}, "-\0");
}

void handle_identifier(const char *source, int *cursor, Token **head)
{
    int start = *cursor;

    while (isalpha(source[*cursor]))
    {

        (*cursor)++;
    }
    int len = *cursor - start;
    char *name = strndup(&source[start], len);

    TokenValue val = {0};
    val.identifier = name;

    append_token(head, TOKEN_IDENTIFIER, val, name);
}

bool is_function(const char *source, int *cursor)
{
    if (source[*cursor] == '-' && source[*cursor + 1] == '>')
        return true;
    return false;
}
Token *tokenize(const char *source)
{
    Token *head = NULL;

    int cursor = 0;
    while (source[cursor] != '\0')
    {
        char current_char = source[cursor];
        if (isalpha(current_char))
            handle_identifier(source, &cursor, &head);
        else if (is_function(source, &cursor))
        {
            append_token(&head, TOKEN_FUNCTION_CALL, (TokenValue){0}, "->");
            cursor += 2;
        }
        else if (isdigit(current_char))
            handle_integer(source, &cursor, &head);
        else if (current_char == '+')
        {
            handle_plus(&cursor, &head);
        }
        else if (current_char == '-')
        {
            handle_minus(&cursor, &head);
        }
        else if (current_char == ' ')
        {
            append_token(&head, TOKEN_SPACE, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '\n')
        {
            append_token(&head, TOKEN_NEW_LINE, (TokenValue){0}, &current_char);
            cursor++;
        }

        else
        {
            char symbol[2];
            symbol[0] = current_char;
            symbol[1] = '\0';
            append_token(&head, TOKEN_BAD, (TokenValue){0}, symbol);
            cursor++;
        }
    }
    return head;
}
