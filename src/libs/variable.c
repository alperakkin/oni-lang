#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variable.h"
#include "utils.h"

void add_variable(GlobalScope *scope, Variable var)
{
    Variable *new_vars = realloc(scope->variables, sizeof(Variable) * (scope->count + 1));
    if (new_vars == NULL)
    {
        fprintf(stderr, "Memory allocation failed in add_variable\n");
        exit(1);
    }

    scope->variables = new_vars;
    scope->variables[scope->count].type = var.type;
    scope->variables[scope->count].name = strdup(var.name);
    switch (var.type)
    {
    case VARIABLE_INT:
        scope->variables[scope->count].int_value = var.int_value;
        break;
    case VARIABLE_FLOAT:
        scope->variables[scope->count].float_value = var.float_value;
        break;
    case VARIABLE_STR:
        scope->variables[scope->count].string_value = strdup(var.string_value);
        break;
    case VARIABLE_BOOL:
        printf("scope length: %d - var name: %s - bool: %d\n", scope->count, var.name, var.bool_value);
        scope->variables[scope->count].bool_value = var.bool_value;
        break;
    case VARIABLE_ARRAY:
        scope->variables[scope->count].array_value.length = var.array_value.length;
        scope->variables[scope->count].array_value.capacity = var.array_value.capacity;
        scope->variables[scope->count].array_value.generic_type = strdup(var.array_value.generic_type);
        int len = var.array_value.length;
        scope->variables[scope->count].array_value.elements = malloc(sizeof(var.array_value) * len);
        if (scope->variables[scope->count].array_value.elements == NULL)
            raise_error("Memory allocation failed for array elements\n", "");

        for (int i = 0; i < len; i++)
        {
            Value *src_val = var.array_value.elements[i];

            Value *new_val = malloc(sizeof(Value));
            if (!new_val)
                raise_error("Memory allocation failed for array element\n", "");

            *new_val = *src_val;

            if (new_val->type == VALUE_STRING)
            {
                new_val->str_val = strdup(src_val->str_val);
            }
            else if (new_val->type == VALUE_INT)
            {
                new_val->int_val = src_val->int_val;
            }
            else if (new_val->type == VALUE_FLOAT)
            {
                new_val->float_val = src_val->float_val;
            }
            else if (new_val->type == VALUE_BOOL)
            {
                new_val->bool_val = src_val->bool_val;
            }
            else if (new_val->type == VALUE_ARRAY)
            {
                // TODO: multidimensional arrays will be defined here
            }
            scope->variables[scope->count]
                .array_value.elements[i] = new_val;
        }
        break;
    case VARIABLE_NULL:
        break;
    default:
        raise_error("Can not assign variable", var.name);
    }
    scope->count++;
}

int get_variable(GlobalScope *scope, char *var_name)
{

    for (int index = 0; index < scope->count; index++)
    {
        Variable var = scope->variables[index];

        if (strcmp(var.name, var_name) == 0)
            return index;
    }
    return -1;
}

GlobalScope *init_globals()
{
    GlobalScope *scope = malloc(sizeof(GlobalScope));
    scope->variables = NULL;
    scope->count = 0;
    return scope;
}

void print_globals(GlobalScope *scope)
{

    printf("globals = {\n");
    for (int i = 0; i < scope->count; i++)
    {
        Variable var = scope->variables[i];
        if (var.name == NULL)
            raise_error("Undefined Variable Name", "");

        if (var.type == VARIABLE_STR)
        {
            printf("    %s (str): ", var.name);
            printf("\"%s\",\n", var.string_value);
        }
        else if (var.type == VARIABLE_FLOAT)
        {
            printf("    %s (float): ", var.name);
            printf("%f,\n", var.float_value);
        }
        else if (var.type == VARIABLE_INT)
        {
            printf("    %s (int): ", var.name);
            printf("%d,\n", var.int_value);
        }
        else if (var.type == VARIABLE_BOOL)
        {
            printf("    %s (bool): ", var.name);
            printf("%s,\n", var.bool_value == 1 ? "true" : "false");
        }
        else if (var.type == VARIABLE_NULL)
        {
            printf("    %s (null): ", var.name);
            printf("%s,\n", "<null>");
        }
    }
    printf("}\n");
}