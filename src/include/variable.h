#pragma once
#include <stdbool.h>

typedef enum
{
    VARIABLE_STR,
    VARIABLE_INT,
    VARIABLE_FLOAT,
    VARIABLE_BOOL,
    VARIABLE_NULL
} VariableType;

typedef struct
{
    char *name;
    VariableType type;
    union
    {
        int int_value;
        bool bool_value;
        bool null_value;
        float float_value;
        char *string_value;
    };
} Variable;

typedef struct
{
    Variable *variables;
    int count;

} GlobalScope;

typedef struct
{
    Variable *variables;
    int count;

} LocalScope;

void add_variable(GlobalScope *scope, Variable var);
int get_variable(GlobalScope *scope, char *var_name);
void print_globals(GlobalScope *scope);
GlobalScope *init_globals();