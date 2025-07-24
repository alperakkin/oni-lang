#pragma once
#include <stdbool.h>
#include "value.h"

typedef struct
{
    Value *variables;
    int count;
    struct Scope *parent;

} Scope;

ValueFunction *find_method(ValueObject *class, const char *method_name);
Value create_instance(ValueObject *class);
void add_variable(Scope *scope, Value *var);
void register_builtins(Scope *global_scope);
int get_variable(Scope *scope, char *var_name, Scope **found);
void print_scope(Scope *scope, char *name);
Scope *init_scope(Scope *parent);