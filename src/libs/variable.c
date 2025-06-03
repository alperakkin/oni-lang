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
    printf("Name: %s, ValType: %u, Val:%d\n", var.name, var.type, var.int_value);

    scope->variables = new_vars;
    scope->variables[scope->count].type = var.type;
    scope->variables[scope->count].name = strdup(var.name);
    scope->variables[scope->count].int_value = var.int_value;
    scope->variables[scope->count].float_value = var.float_value;
    scope->count++;
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
            printf("%d,\n", var.int_value);
        }
        else if (var.type == VARIABLE_FLOAT)
        {
            printf("    %s (float): ", var.name);
            printf("%d,\n", var.int_value);
        }
        else if (var.type == VARIABLE_INT)
        {
            printf("    %s (int): ", var.name);
            printf("%d,\n", var.int_value);
        }
    }
    printf("}\n");
}