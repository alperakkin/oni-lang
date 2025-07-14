#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variable.h"
#include "utils.h"

void add_variable(Scope *scope, Value var)
{
    Value *new_vars = realloc(scope->variables, sizeof(Value) * (scope->count + 1));
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
    case VALUE_INT:
        scope->variables[scope->count].int_val = var.int_val;
        break;
    case VALUE_FLOAT:
        scope->variables[scope->count].float_val = var.float_val;
        break;
    case VALUE_STRING:
        scope->variables[scope->count].str_val = strdup(var.str_val);
        break;
    case VALUE_BOOL:
        printf("scope length: %d - var name: %s - bool: %d\n", scope->count, var.name, var.bool_val);
        scope->variables[scope->count].bool_val = var.bool_val;
        break;
    case VALUE_ARRAY:
        scope->variables[scope->count].array_val.length = var.array_val.length;
        scope->variables[scope->count].array_val.capacity = var.array_val.capacity;
        scope->variables[scope->count].array_val.generic_type = strdup(var.array_val.generic_type);
        int len = var.array_val.length;
        scope->variables[scope->count].array_val.elements = malloc(sizeof(var.array_val) * len);
        if (scope->variables[scope->count].array_val.elements == NULL)
            raise_error("Memory allocation failed for array elements\n", "");

        for (int i = 0; i < len; i++)
        {
            Value *src_val = var.array_val.elements[i];

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
                .array_val.elements[i] = new_val;
        }
        break;
    case VALUE_NULL:
        break;
    default:
        raise_error("Can not assign variable", var.name);
    }
    scope->count++;
}

int get_variable(Scope *scope, char *var_name, Scope **found)
{

    Scope *current = scope;
    while (current != NULL)
    {

        for (int index = 0; index < scope->count; index++)
        {
            Value var = scope->variables[index];

            if (strcmp(var.name, var_name) == 0)
            {
                *found = current;
                return index;
            }
        }
        current = current->parent;
    }
    return -1;
}

Scope *init_scope(Scope *parent)
{
    Scope *scope = malloc(sizeof(Scope));
    scope->variables = NULL;
    scope->count = 0;
    scope->parent = parent;
    return scope;
}

void print_scope(Scope *scope)
{

    printf("globals = {\n");
    for (int i = 0; i < scope->count; i++)
    {
        Value var = scope->variables[i];
        if (var.name == NULL)
            raise_error("Undefined Variable Name", "");

        if (var.type == VALUE_STRING)
        {
            printf("    %s (str): ", var.name);
            printf("\"%s\",\n", var.str_val);
        }
        else if (var.type == VALUE_FLOAT)
        {
            printf("    %s (float): ", var.name);
            printf("%f,\n", var.float_val);
        }
        else if (var.type == VALUE_INT)
        {
            printf("    %s (int): ", var.name);
            printf("%d,\n", var.int_val);
        }
        else if (var.type == VALUE_BOOL)
        {
            printf("    %s (bool): ", var.name);
            printf("%s,\n", var.bool_val == 1 ? "true" : "false");
        }
        else if (var.type == VALUE_NULL)
        {
            printf("    %s (null): ", var.name);
            printf("%s,\n", "<null>");
        }
    }
    printf("}\n");
}