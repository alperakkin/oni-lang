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
    case VARIABLE_NULL:
        scope->variables[scope->count].null_value = var.null_value;
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
    }
    printf("}\n");
}