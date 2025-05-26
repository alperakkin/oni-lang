#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

void append_token(Token **head, TokenType token_type, TokenValue value)
{
    Token *new_token = malloc(sizeof(Token));
    if (!new_token)
        return;

    new_token->type = token_type;
    new_token->value = value;
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

        free(current);
        current = next;
    }
}

void print_tokens(Token *tokens)
{
    Token *tmp = tokens;
    while (tmp != NULL)
    {
        if (tmp->type == TOKEN_NUMBER)
        {

            printf("TOKEN [NUMBER] -> %d\n", tmp->value.int_val);
        }
        else if (tmp->type == TOKEN_PLUS)
        {
            printf("TOKEN [PLUS] -> +\n");
        }
        else
        {
            printf("Undefined Token\n");
        }

        tmp = tmp->next;
    }
}

void handle_number(const char *source, int *cursor, Token **head)
{
    TokenValue value = {.int_val = 0};

    while (isdigit(source[*cursor]))
    {
        value.int_val = value.int_val * 10 + (source[*cursor] - '0');
        (*cursor)++;
    }
    append_token(head, TOKEN_NUMBER, value);
}

void handle_plus(const char *source, int *cursor, Token **head)
{
    (*cursor)++;
    append_token(head, TOKEN_PLUS, (TokenValue){0});
}

Token *tokenize(const char *source)
{
    Token *head = NULL;

    int cursor = 0;
    while (source[cursor] != '\0')
    {
        char current_char = source[cursor];

        if (isdigit(current_char))
            handle_number(source, &cursor, &head);
        else if (current_char == '+')
        {
            handle_plus(source, &cursor, &head);
        }

        else
        {
            cursor++;
        }
    }
    return head;
}
