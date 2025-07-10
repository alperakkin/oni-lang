#pragma once
#include <stdbool.h>
#include "value.h"

typedef struct
{
    Value *variables;
    int count;

} Scope;

void add_variable(Scope *scope, Value var);
int get_variable(Scope *scope, char *var_name);
void print_scope(Scope *scope);
Scope *init_scope();