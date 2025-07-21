#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
#include "value.h"
#include "ast.h"
#include "variable.h"

void type_check(Value val, Node *node);
Value call_function(
    ValueFunction *fn,
    Node **args, int args_count,
    Node **kwargs, int kwargs_count,
    Scope *caller_scope);

#endif