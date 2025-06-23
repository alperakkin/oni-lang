#pragma once
#include <stdbool.h>
#include "value.h"
typedef enum
{
    VARIABLE_STR,
    VARIABLE_INT,
    VARIABLE_FLOAT,
    VARIABLE_BOOL,
    VARIABLE_NULL,
    VARIABLE_ARRAY,
} VariableType;

typedef struct
{
    int **int_array;
    float **float_array;
    char **string_array;
    bool **boolean_array;
} VariableArray;
typedef struct
{
    char *name;
    VariableType type;
    union
    {
        int int_value;
        bool bool_value;
        float float_value;
        char *string_value;
        ValueArray array_value;
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