#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
#include "ast.h"
#include "value.h"
#include "variable.h"

void type_check(Value val, Node *node);
Value call_function(
    ValueFunction *fn,
    Node **args, int args_count,
    Node **kwargs, int kwargs_count,
    Scope *caller_scope);

Value create_builtin_function(const char *name, Value (*func)(Value *, Value *, int, int));
#endif