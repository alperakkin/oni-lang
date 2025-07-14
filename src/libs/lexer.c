#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "utils.h"

void append_token(Token **head, TokenType TK_type, TokenValue value,
                  char *symbol)
{
    Token *new_token = malloc(sizeof(Token));
    if (!new_token)
        return;
    new_token->type = TK_type;
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

void print_token(Token *token)
{
    char symbol[2];
    symbol[0] = *token->symbol;
    symbol[1] = '\0';
    if (token->type == TK_INTEGER)
    {

        printf("TOKEN [INTEGER] -> %d\n", token->value.int_val);
    }
    else if (token->type == TK_FLOAT)
    {
        printf("TOKEN [FLOAT] -> %f\n", token->value.float_val);
    }
    else if (token->type == TK_PLUS)
    {
        printf("TOKEN [PLUS] -> +\n");
    }
    else if (token->type == TK_MINUS)
    {
        printf("TOKEN [MINUS] -> -\n");
    }
    else if (token->type == TK_STAR)
    {
        printf("TOKEN [STAR] -> *\n");
    }
    else if (token->type == TK_SLASH)
    {
        printf("TOKEN [SLASH] -> *\n");
    }
    else if (token->type == TK_QUOTE)
    {
        printf("TOKEN [QUOTE] -> \"\n");
    }
    else if (token->type == TK_STRING)
    {
        printf("TOKEN [STRING] -> %s\n", token->symbol);
    }
    else if (token->type == TK_L_PAREN)
    {
        printf("TOKEN [LEFT PARANTHESIS] -> (\n");
    }
    else if (token->type == TK_R_PAREN)
    {
        printf("TOKEN [RIGHT PARANTHESIS] -> )\n");
    }
    else if (token->type == TK_L_SQUARE)
    {
        printf("TOKEN [LEFT SQUARE] -> [\n");
    }
    else if (token->type == TK_R_SQUARE)
    {
        printf("TOKEN [RIGHT SQUARE] -> ]\n");
    }
    else if (token->type == TK_L_CURL)
    {
        printf("TOKEN [LEFT CURL] -> {\n");
    }
    else if (token->type == TK_R_CURL)
    {
        printf("TOKEN [RIGHT CURL] -> }\n");
    }
    else if (token->type == TK_LT)
    {
        printf("TOKEN [LESS THAN] -> <\n");
    }
    else if (token->type == TK_GT)
    {
        printf("TOKEN [GRATER THAN] -> >\n");
    }
    else if (token->type == TK_LTE)
    {
        printf("TOKEN [LESS THAN OR EQUAL] -> <\n");
    }
    else if (token->type == TK_GTE)
    {
        printf("TOKEN [GRATER THAN OR EQUAL] -> >\n");
    }
    else if (token->type == TK_COMMA)
    {
        printf("TOKEN [COMMA] -> ,\n");
    }
    else if (token->type == TK_BAD)
    {
        printf("Undefined Token -> %s\n", symbol);
    }
    else if (token->type == TK_SPACE)
    {

        printf("TOKEN [SPACE] -> %s\n", symbol);
    }
    else if (token->type == TK_IDENTIFIER)
    {
        printf("TOKEN [IDENTIFIER] -> %s\n", token->symbol);
    }
    else if (token->type == TK_NEW_LINE)
    {
        printf("TOKEN [NEW LINE] -> %s\n", symbol);
    }
    else if (token->type == TK_COMMENT)
    {
        printf("TOKEN [COMMENT] -> %s\n", token->value.text_val);
    }
    else if (token->type == TK_ASSIGN)
    {
        printf("TOKEN [ASSIGN] -> %s\n", symbol);
    }
    else if (token->type == TK_BOOL)
    {
        printf("TOKEN [BOOL] -> %s\n", symbol);
    }
    else if (token->type == TK_WHILE)
    {
        printf("TOKEN [WHILE] -> %s\n", token->symbol);
    }
    else if (token->type == TK_IF)
    {
        printf("TOKEN [IF] -> %s\n", token->symbol);
    }
    else if (token->type == TK_FOR)
    {
        printf("TOKEN [FOR] -> %s\n", token->symbol);
    }
    else if (token->type == TK_IN)
    {
        printf("TOKEN [IN] -> %s\n", token->symbol);
    }
    else if (token->type == TK_TO)
    {
        printf("TOKEN [TO] -> %s\n", token->symbol);
    }
    else if (token->type == TK_OF)
    {
        printf("TOKEN [OF] -> %s\n", token->symbol);
    }
    else if (token->type == TK_ELSE)
    {
        printf("TOKEN [ELSE] -> %s\n", token->symbol);
    }
    else if (token->type == TK_EQ)
    {
        printf("TOKEN [CMP EQUAL] -> %s\n", symbol);
    }
    else if (token->type == TK_NEQ)
    {
        printf("TOKEN [CMP NOT EQUAL] -> %s\n", symbol);
    }
    else if (token->type == TK_AND)
    {
        printf("TOKEN [AND] -> %s\n", token->symbol);
    }
    else if (token->type == TK_OR)
    {
        printf("TOKEN [OR] -> %s\n", token->symbol);
    }
    else if (token->type == TK_INC)
    {
        printf("TOKEN [INCREMENT] -> %s\n", token->symbol);
    }
    else if (token->type == TK_FUNC_DEF)
    {
        printf("TOKEN [FUNCTION DEFINITION] -> %s\n", token->symbol);
    }
    else if (token->type == TK_RETURN)
    {
        printf("TOKEN [RETURN] -> %s\n", token->symbol);
    }
    else if (token->type == TK_BREAK)
    {
        printf("TOKEN [BREAK] -> %s\n", token->symbol);
    }
    else if (token->type == TK_CONTINUE)
    {
        printf("TOKEN [CONTINUE] -> %s\n", token->symbol);
    }
    else if (token->type == TK_DECR)
    {
        printf("TOKEN [DECREMENT] -> %s\n", token->symbol);
    }
    else if (token->type == TK_NULL)
    {
        printf("TOKEN [NULL] -> %s\n", "null");
    }
    else
    {
        printf("Unexpected TOKEN!!\n");
    }
}

void print_tokens(Token *tokens)
{
    Token *tmp = tokens;
    while (tmp != NULL)
    {
        print_token(tmp);
        tmp = tmp->next;
    }
}

void handle_number(const char *source, int *cursor, Token **head)
{

    int start = *cursor;
    int dot_cursor = 0;

    while (isdigit(source[*cursor]))
    {
        (*cursor)++;
    }
    if (source[*cursor] == '.')
    {
        dot_cursor = 1;
        (*cursor)++;

        while (isdigit(source[*cursor]))
        {
            (*cursor)++;
        }
    }
    int len = *cursor - start;
    char *number_str = strndup(&source[start], len);

    if (dot_cursor)
    {
        float fval = atof(number_str);
        TokenValue val = {.float_val = fval};
        append_token(head, TK_FLOAT, val, number_str);
    }
    else
    {
        int ival = atoi(number_str);
        TokenValue val = {.int_val = ival};
        append_token(head, TK_INTEGER, val, number_str);
    }
    free(number_str);
}

void handle_plus(const char *source, int *cursor, Token **head)
{
    if (source[*cursor + 1] == '+')
    {

        *cursor += 2;
        append_token(head, TK_INC, (TokenValue){0}, "++");
    }
    else
    {
        (*cursor)++;
        append_token(head, TK_PLUS, (TokenValue){0}, "+");
    }
}
void handle_minus(const char *source, int *cursor, Token **head)
{
    if (source[*cursor + 1] == '-')
    {
        *cursor += 2;
        append_token(head, TK_DECR, (TokenValue){0}, "--");
    }
    else if (source[*cursor + 1] == '>')
    {
        *cursor += 2;
        append_token(head, TK_PUSH, (TokenValue){0}, "->");
    }
    else
    {
        (*cursor)++;
        append_token(head, TK_MINUS, (TokenValue){0}, "-");
    }
}
void handle_star(int *cursor, Token **head)
{
    (*cursor)++;
    append_token(head, TK_STAR, (TokenValue){0}, "*\0");
}

void handle_slash(int *cursor, Token **head)
{
    (*cursor)++;
    append_token(head, TK_SLASH, (TokenValue){0}, "/\0");
}

void handle_string(const char *source, int *cursor, Token **head)
{
    (*cursor)++;
    char current_char = source[*cursor];
    int start = *cursor;

    while (current_char != '"')
    {
        if (current_char == '\0')
            raise_error("String literal must be closed with", "\"");
        (*cursor)++;
        current_char = source[*cursor];
    }
    int len = *cursor - start;
    char *name = strndup(&source[start], len);
    TokenValue val = {.text_val = name};
    append_token(head, TK_STRING, val, name);
    (*cursor)++;
}
void handle_comment(const char *source, int *cursor, Token **head)
{
    bool is_comment = false;
    (*cursor)++;
    int start = *cursor;
    if (source[*cursor] == ':')
    {
        (*cursor)++;
        is_comment = true;
    }
    while (source[*cursor] != '\n' && source[*cursor] != '\0')
        (*cursor)++;

    int len = *cursor - start;
    TokenValue value = {.text_val = strndup(&source[start], len)};

    if (is_comment)
        append_token(head, TK_COMMENT, value, ":!");
}

void handle_equals(const char *source, int *cursor, Token **head)
{
    (*cursor)++;

    if (source[*cursor] == '=')
    {
        append_token(head, TK_EQ, (TokenValue){0}, "==");
        (*cursor)++;
    }
    else
        append_token(head, TK_ASSIGN, (TokenValue){0}, "=");
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
    if (strcmp(name, "true") == 0 || strcmp(name, "false") == 0)
    {
        val.bool_val = strcmp(name, "true") == 0 ? 1 : 0;
        append_token(head, TK_BOOL, val, name);
        return;
    }
    else if (strcmp(name, "null") == 0)
    {
        val.null_val = true;
        append_token(head, TK_NULL, val, name);
        return;
    }
    else if (strcmp(name, "if") == 0)
    {
        append_token(head, TK_IF, (TokenValue){0}, name);
        return;
    }

    else if (strcmp(name, "else") == 0)
    {
        append_token(head, TK_ELSE, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "and") == 0)
    {
        append_token(head, TK_AND, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "or") == 0)
    {
        append_token(head, TK_OR, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "while") == 0)
    {
        append_token(head, TK_WHILE, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "for") == 0)
    {
        append_token(head, TK_FOR, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "of") == 0)
    {
        append_token(head, TK_OF, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "in") == 0)
    {
        append_token(head, TK_IN, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "to") == 0)
    {
        append_token(head, TK_TO, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "return") == 0)
    {
        append_token(head, TK_RETURN, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "continue") == 0)
    {
        append_token(head, TK_CONTINUE, (TokenValue){0}, name);
        return;
    }
    else if (strcmp(name, "break") == 0)
    {
        append_token(head, TK_BREAK, (TokenValue){0}, name);
        return;
    }

    val.identifier = name;

    append_token(head, TK_IDENTIFIER, val, name);
}

Token *tokenize(const char *source)
{
    Token *head = NULL;

    int cursor = 0;
    while (source[cursor] != '\0')
    {
        char current_char = source[cursor];
        if (current_char == '!')
        {
            handle_comment(source, &cursor, &head);
        }
        else if (current_char == '"')
        {
            handle_string(source, &cursor, &head);
        }
        else if (isalpha(current_char))
            handle_identifier(source, &cursor, &head);
        else if (isdigit(current_char))
            handle_number(source, &cursor, &head);
        else if (current_char == '+')
        {
            handle_plus(source, &cursor, &head);
        }
        else if (current_char == '-')
        {
            handle_minus(source, &cursor, &head);
        }
        else if (current_char == '*')
        {
            handle_star(&cursor, &head);
        }
        else if (current_char == '/')
        {
            handle_slash(&cursor, &head);
        }
        else if (current_char == '=')
        {
            handle_equals(source, &cursor, &head);
        }

        else if (current_char == ' ')
        {
            append_token(&head, TK_SPACE, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '\n')
        {
            append_token(&head, TK_NEW_LINE, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '<')
        {

            append_token(&head, TK_LT, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '@')
        {

            append_token(&head, TK_FUNC_DEF, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '>')
        {
            append_token(&head, TK_GT, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '(')
        {
            append_token(&head, TK_L_PAREN, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == ')')
        {
            append_token(&head, TK_R_PAREN, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '[')
        {
            append_token(&head, TK_L_SQUARE, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == ']')
        {
            append_token(&head, TK_R_SQUARE, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '{')
        {
            append_token(&head, TK_L_CURL, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == '}')
        {
            append_token(&head, TK_R_CURL, (TokenValue){0}, &current_char);
            cursor++;
        }
        else if (current_char == ',')
        {
            append_token(&head, TK_COMMA, (TokenValue){0}, &current_char);
            cursor++;
        }

        else
        {
            char symbol[2];
            symbol[0] = current_char;
            symbol[1] = '\0';
            append_token(&head, TK_BAD, (TokenValue){0}, symbol);
            cursor++;
        }
    }
    return head;
}
