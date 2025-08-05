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
ValueFunction *copy_function(ValueFunction *src);
Value copy_value(Value *original, Scope *scope);
ValueFunction *find_method(ValueObject *class, const char *method_name);
Value create_builtin_function(const char *name, Value (*func)(Value *, Value *, int, int));
#endif