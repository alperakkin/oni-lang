
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "functions.h"
#include "builtins.h"

void type_check(Value val, Node *node)
{
    switch (val.type)
    {
    case VALUE_INT:
        if (strcmp(node->identifier.value, "int") != 0)
            raise_error("Invalid Return Type  got <int> but expected ", strdup(node->identifier.value));
        break;
    case VALUE_STRING:
        if (strcmp(node->identifier.value, "str") != 0)
            raise_error("Invalid Return Type  got <str> but expected ", strdup(node->identifier.value));
        break;
    case VALUE_ARRAY:
        if (strcmp(node->identifier.value, "arr") != 0)
            raise_error("Invalid Return Type  got <arr> but expected ", strdup(node->identifier.value));
        break;
    case VALUE_BOOL:
        if (strcmp(node->identifier.value, "bool") != 0)
            raise_error("Invalid Return Type  got <bool> but expected ", strdup(node->identifier.value));
        break;
    case VALUE_FLOAT:
        if (strcmp(node->identifier.value, "float") != 0)
            raise_error("Invalid Return Type  got <float> but expected ", strdup(node->identifier.value));
        break;

    default:
        raise_error("Undefined return type", "");
        break;
    }
}

Value create_builtin_function(const char *name, Value (*func)(Value *, Value *, int, int))
{
    Value val;
    val.type = VALUE_FUNCTION;
    val.name = strdup(name);

    ValueFunction *f = malloc(sizeof(ValueFunction));
    f->name = strdup(name);
    f->is_builtin = true;
    f->func = func;
    f->block = NULL;
    f->args = NULL;
    f->args_count = 0;
    f->kwargs = NULL;
    f->kwargs_count = 0;
    f->return_type = NULL;

    val.func_val = f;
    return val;
}

Value call_function(
    ValueFunction *fn,
    Node **args, int args_count,
    Node **kwargs, int kwargs_count,
    Scope *caller_scope)
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
            evaluated_args[i] = interpret(args[i], caller_scope);
        }

        for (int i = 0; i < kwargs_count; i++)
        {

            Node kwarg_node = *kwargs[i];
            if (kwarg_node.type != NODE_VARIABLE)
                raise_error("Expected keyword argument to be variable", "");

            evaluated_kwargs[i] = interpret(kwarg_node.variable.value, caller_scope);
            evaluated_kwargs[i].name = strdup(kwarg_node.variable.name);
        }

        Value result = fn->func(evaluated_args, evaluated_kwargs, args_count, kwargs_count);

        return result;
    }

    for (int i = 0; i < args_count; i++)
    {
        Value val = interpret(args[i], caller_scope);

        val.name = strdup(fn->args[i]->variable.name);

        add_variable(caller_scope, &val);
    }

    for (int i = 0; i < kwargs_count; i++)
    {
        Node *kwarg_node = kwargs[i];

        if (kwarg_node->type != NODE_VARIABLE)
            raise_error("Expected keyword argument to be variable", "");

        Value val = interpret(kwarg_node->variable.value, caller_scope);

        val.name = strdup(kwarg_node->variable.name);
        add_variable(caller_scope, &val);
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

        Value val = interpret(fn->kwargs[i]->variable.value, caller_scope);
        val.name = strdup(def_arg_name);

        add_variable(caller_scope, &val);
    }

    Value last = {0};
    last.type = VALUE_NULL;

    for (int i = 0; i < fn->block->count; i++)
    {
        Node *stmt = fn->block->statements[i];
        if (stmt->type == NODE_RETURN)
        {
            Value return_val = interpret(stmt->node_return.expression, caller_scope);
            type_check(return_val, fn->return_type);
            return return_val;
        }

        last = interpret(stmt, caller_scope);
    }

    return last;
}
