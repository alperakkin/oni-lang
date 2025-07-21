#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
#include "value.h"
#include "ast.h"
#include "variable.h"

typedef struct
{
    const char *name;
    bool is_builtin;
    Value (*func)(Value *args, Value *kwargs, int arg_count, int kwarg_count);
    NodeBlock *block;
    Node **args;
    int args_count;
    Node **kwargs;
    int kwargs_count;
} Function;

typedef struct
{
    Function **functions;
    int count;
    int capacity;
} FunctionRegistry;

extern Function registry[];

FunctionRegistry *init_function_registry();
void add_function(FunctionRegistry *registry, Function *func);
Function *get_function(char *name, FunctionRegistry *registry);
void print_function(Function *func);
Value call_function(
    Function *fn,
    Node **args, int args_count,
    Node **kwargs, int kwargs_count,
    Scope *caller_scope,
    FunctionRegistry *registry);
#endif