
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "functions.h"
#include "builtins.h"

void add_function(FunctionRegistry *registry, Function *func)
{
    if (registry->count >= registry->capacity)
    {
        registry->capacity *= 2;
        registry->functions = realloc(registry->functions, sizeof(Function) * registry->capacity);
    }
    registry->functions[registry->count++] = func;
}

FunctionRegistry *init_function_registry()
{
    FunctionRegistry *registry = malloc(sizeof(FunctionRegistry));
    registry->capacity = 8;
    registry->count = 0;
    registry->functions = malloc(sizeof(Function) * registry->capacity);

    Function *print_func = malloc(sizeof(Function));
    print_func->name = strdup("print");
    print_func->is_builtin = true;
    print_func->func = builtin_print;
    add_function(registry, print_func);
    // TODO make a add register function to set all builtins at once
    return registry;
}

Function *get_function(char *name, FunctionRegistry *registry)
{
    for (int i = 0; i < registry->count; ++i)
    {
        if (strcmp(registry->functions[i]->name, name) == 0)
        {
            return registry->functions[i];
        }
    }
    return NULL;
}

void print_function(Function *func)
{
    printf("FUNCTION: \n");
    printf("\tname: %s\n", func->name);
    printf("\tbuiltin: %s\n", func->is_builtin == true ? "true" : "false");
}

Value call_function(
    Function *fn,
    Node **args, int args_count,
    Node **kwargs, int kwargs_count,
    Scope *caller_scope,
    FunctionRegistry *registry)
{

    if (fn == NULL)
    {
        raise_error("Function is not defined", "");
        Value none = {0};
        none.type = VALUE_NULL;
        return none;
    }

    if (fn->is_builtin && fn->func != NULL)
    {

        Value *evaluated_args = malloc(sizeof(Value) * args_count);
        Value *evaluated_kwargs = malloc(sizeof(Value) * kwargs_count);

        for (int i = 0; i < args_count; i++)
        {
            evaluated_args[i] = interpret(args[i], caller_scope, registry);
        }

        for (int i = 0; i < kwargs_count; i++)
        {

            Node kwarg_node = *kwargs[i];
            if (kwarg_node.type != NODE_VARIABLE)
                raise_error("Expected keyword argument to be variable", "");

            evaluated_kwargs[i] = interpret(kwarg_node.variable.value, caller_scope, registry);
            evaluated_kwargs[i].name = strdup(kwarg_node.variable.name);
        }

        Value result = fn->func(evaluated_args, evaluated_kwargs, args_count, kwargs_count);

        return result;
    }

    for (int i = 0; i < args_count; i++)
    {
        Value val = interpret(args[i], caller_scope, registry);

        val.name = strdup(fn->args[i]->variable.name);

        add_variable(caller_scope, val);
    }

    for (int i = 0; i < kwargs_count; i++)
    {
        Node *kwarg_node = kwargs[i];

        if (kwarg_node->type != NODE_VARIABLE)
            raise_error("Expected keyword argument to be variable", "");

        Value val = interpret(kwarg_node->variable.value, caller_scope, registry);

        val.name = strdup(kwarg_node->variable.name);
        add_variable(caller_scope, val);
    }

    // set default values
    for (int i = 0; i < fn->kwargs_count; i++)
    {
        char *def_arg_name = fn->kwargs[i]->variable.name;

        int given = 0;
        for (int j = 0; j < kwargs_count; j++)
        {
            if (strcmp(kwargs[j]->variable.name, def_arg_name) == 0)
            {
                given = 1;
                break;
            }
        }
        if (given)
            continue;

        Value val = interpret(fn->kwargs[i]->variable.value, caller_scope, registry);
        val.name = strdup(def_arg_name);

        add_variable(caller_scope, val);
    }

    Value last = {0};
    last.type = VALUE_NULL;

    for (int i = 0; i < fn->block->count; i++)
    {
        Node *stmt = fn->block->statements[i];
        if (stmt->type == NODE_RETURN)
            return interpret(stmt->node_return.expression, caller_scope, registry);

        last = interpret(stmt, caller_scope, registry);
    }

    return last;
}
