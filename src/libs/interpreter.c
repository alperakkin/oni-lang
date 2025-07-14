#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "variable.h"
#include "value.h"
#include "utils.h"
#include "builtins.h"

Value interpret(Node *node, Scope *scope)
{
    Value result;
    result.type = VALUE_NULL;

    if (!node)
        raise_error("Unknown eval node", "");

    switch (node->type)
    {
    case NODE_NUMBER:
        if (node->number.type == NODE_INTEGER)
        {
            result.type = VALUE_INT;
            result.int_val = node->number.int_value;
        }
        else if (node->number.type == NODE_FLOAT)
        {
            result.type = VALUE_FLOAT;
            result.float_val = node->number.float_value;
        }
        return result;

    case NODE_UNARY_OP:
    {
        Scope *found_scope = NULL;
        int index = get_variable(scope, node->unary_op.operand->identifier.value, &found_scope);
        if (scope->variables[index].type != VALUE_INT)
            raise_error("Unary operations only valid for integers", "");

        if (node->unary_op.token->type == TK_INC)
        {
            found_scope->variables[index].int_val++;
        }
        else if (node->unary_op.token->type == TK_DECR)
        {
            found_scope->variables[index].int_val--;
        }
        break;
    }
    case NODE_BINARY_OP:
    {

        Value left = interpret(node->binary_op.left, scope);
        Value right = interpret(node->binary_op.right, scope);
        int is_float = (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT);
        int assignment = node->binary_op.token->type == TK_ASSIGN;

        if (assignment)
        {
            Scope *found_scope = NULL;
            int index = get_variable(scope, node->binary_op.left->identifier.value, &found_scope);
            switch (right.type)
            {
            case VALUE_FLOAT:
                found_scope->variables[index].float_val = right.float_val;
                found_scope->variables[index].type = VALUE_FLOAT;
                break;
            case VALUE_INT:
                found_scope->variables[index].int_val = right.int_val;
                found_scope->variables[index].type = VALUE_INT;
                break;
            case VALUE_STRING:
                free(found_scope->variables[index].str_val);
                found_scope->variables[index].str_val = strdup(right.str_val);
                found_scope->variables[index].type = VALUE_STRING;
                break;
            case VALUE_BOOL:
                found_scope->variables[index].bool_val = right.bool_val;
                found_scope->variables[index].type = VALUE_BOOL;
                break;
            case VALUE_NULL:
                found_scope->variables[index].type = VALUE_NULL;
                break;
            default:
                raise_error("Unsupported assignment type", "");
            }
        }
        else if (!is_float)
        {
            result.type = VALUE_INT;
            int l = left.int_val;
            int r = right.int_val;

            switch (node->binary_op.token->type)
            {
            case TK_PLUS:
                result.int_val = l + r;
                break;
            case TK_MINUS:
                result.int_val = l - r;
                break;
            case TK_STAR:
                result.int_val = l * r;
                break;
            case TK_SLASH:
                result.int_val = l / r;
                break;
            case TK_GT:
                result.type = VALUE_BOOL;
                result.bool_val = l > r;
                break;
            case TK_GTE:
                result.type = VALUE_BOOL;
                result.bool_val = l >= r;
                break;
            case TK_LT:
                result.type = VALUE_BOOL;
                result.bool_val = l < r;
                break;
            case TK_LTE:
                result.type = VALUE_BOOL;
                result.bool_val = l <= r;
                break;
            case TK_EQ:
                result.type = VALUE_BOOL;
                result.bool_val = l == r;
                break;
            case TK_NEQ:
                result.type = VALUE_BOOL;
                result.bool_val = l != r;
                break;
            case TK_AND:
                result.type = VALUE_BOOL;
                result.bool_val = l && r;
                break;
            case TK_OR:
                result.type = VALUE_BOOL;
                result.bool_val = l || r;
                break;
            default:
                print_token(node->binary_op.token);
                raise_error("Unknown binary operator", "");
            }
        }
        else
        {
            result.type = VALUE_FLOAT;
            float l = (left.type == VALUE_INT) ? (float)left.int_val : left.float_val;
            float r = (right.type == VALUE_INT) ? (float)right.int_val : right.float_val;
            switch (node->binary_op.token->type)
            {
            case TK_PLUS:
                result.float_val = l + r;
                break;
            case TK_MINUS:
                result.float_val = l - r;
                break;
            case TK_STAR:
                result.float_val = l * r;
                break;
            case TK_SLASH:
                result.float_val = l / r;
                break;
            case TK_GT:
                result.type = VALUE_BOOL;
                result.bool_val = l > r;
                break;
            case TK_GTE:
                result.type = VALUE_BOOL;
                result.bool_val = l >= r;
                break;
            case TK_LT:
                result.type = VALUE_BOOL;
                result.bool_val = l < r;
                break;
            case TK_LTE:
                result.type = VALUE_BOOL;
                result.bool_val = l <= r;
                break;
            case TK_EQ:
                result.type = VALUE_BOOL;
                result.bool_val = l == r;
                break;
            case TK_NEQ:
                result.type = VALUE_BOOL;
                result.bool_val = l != r;
                break;
            case TK_AND:
                result.type = VALUE_BOOL;
                result.bool_val = l && r;
                break;
            case TK_OR:
                result.type = VALUE_BOOL;
                result.bool_val = l || r;
                break;
            default:
                raise_error("Unknown binary operator", "");
            }
        }
        return result;
    }
    case NODE_IF:
    {

        Value condition = interpret(node->node_if.condition, scope);
        if (condition.bool_val == true)
        {
            Scope *locals = init_scope(scope);

            for (int i = 0; i < node->node_if.if_block->count; i++)
            {
                result = interpret(node->node_if.if_block->statements[i], locals);
            }
        }
        else
        {
            if (node->node_if.else_block)
            {
                Scope *locals = init_scope(scope);

                for (int i = 0; i < node->node_if.else_block->count; i++)
                {
                    result = interpret(node->node_if.else_block->statements[i], locals);
                }
            }
        }
        return result;
    }
    case NODE_WHILE:
    {

        Value condition = interpret(node->node_while.condition, scope);
        while (condition.bool_val != true)
        {
            for (int i = 0; i < node->node_while.if_block->count; i++)
            {
                result = interpret(node->node_while.if_block->statements[i], scope);
            }
            condition = interpret(node->node_while.condition, scope);
        }

        return result;
    }
    case NODE_FOR:
    {
        Scope *locals = init_scope(scope);
        Value iterator = interpret(node->node_for.iterator, locals);
        add_variable(locals, iterator);
        for (int i = 0; i < node->node_for.iterable->array.length; i++)
        {
            for (int i = 0; i < node->node_while.if_block->count; i++)
            {
                result = interpret(node->node_while.if_block->statements[i], locals);
            }
        }

        return result;
    }

    case NODE_ARRAY:
    {

        result.type = VALUE_ARRAY;
        int len = node->array.length;
        result.array_val.length = len;
        result.array_val.capacity = node->array.capacity;
        result.array_val.elements = malloc(sizeof(Value *) * len);
        result.array_val.generic_type = strdup(node->array.generic_type);

        for (int i = 0; i < len; i++)
        {
            Value *val = malloc(sizeof(Value));
            *val = interpret(node->array.elements[i], scope);
            result.array_val.elements[i] = val;
        }
        return result;
    }

    case NODE_FUNCTION_CALL:
    {

        Value left = interpret(node->func_call.left, scope);
        Value right = interpret(node->func_call.right, scope);

        BuiltinFunction *fn = find_builtin(left.str_val);
        if (!fn)
            raise_error("Error: function not found", left.str_val);
        return fn->func(&right, 1);
    }

    case NODE_IDENTIFIER:
    {
        Scope *found_scope = NULL;
        int index = get_variable(scope, node->identifier.value, &found_scope);

        if (index == -1)
        {
            result.type = VALUE_STRING;
            result.str_val = node->identifier.value;
            return result;
        }

        Value var = found_scope->variables[index];

        switch (var.type)
        {
        case VALUE_INT:
            result.type = VALUE_INT;
            result.int_val = var.int_val;
            break;
        case VALUE_FLOAT:
            result.type = VALUE_FLOAT;
            result.float_val = var.float_val;
            break;
        case VALUE_BOOL:
            result.type = VALUE_BOOL;
            result.bool_val = var.bool_val;
            break;
        case VALUE_STRING:
            result.type = VALUE_STRING;
            result.str_val = strdup(var.str_val);
            break;
        case VALUE_ARRAY:
            result.type = VALUE_ARRAY;
            result.array_val = var.array_val;

            if (var.array_val.generic_type)
                result.array_val.generic_type = strdup(var.array_val.generic_type);

            break;
        case VALUE_NULL:
        default:
            result.type = VALUE_NULL;
            break;
        }
        return result;
    }

    case NODE_STRING:
        result.type = VALUE_STRING;
        result.str_val = node->string.value;
        return result;

    case NODE_VARIABLE:
    {
        result.type = VALUE_VARIABLE;
        Value var = {0};
        if (!node->variable.name)
            raise_error("Variable Name not assigned", "");
        var.name = strdup(node->variable.name);
        Value right = interpret(node->variable.value, scope);

        if (strcmp(node->variable.type, "int") == 0)
        {
            if (right.type == VALUE_NULL)
                var.type = VALUE_NULL;
            else
            {
                var.int_val = (right.type == VALUE_FLOAT) ? (int)right.float_val : right.int_val;
                var.type = VALUE_INT;
            }
        }
        else if (strcmp(node->variable.type, "str") == 0)
        {
            var.str_val = strdup(right.str_val);
            var.type = VALUE_STRING;
        }
        else if (strcmp(node->variable.type, "float") == 0)
        {
            var.float_val = (right.type == VALUE_INT) ? (float)right.int_val : right.float_val;
            var.type = VALUE_FLOAT;
        }
        else if (strcmp(node->variable.type, "bool") == 0)
        {
            var.bool_val = right.bool_val;
            var.type = VALUE_BOOL;
        }
        else if (strcmp(node->variable.type, "arr") == 0)
        {
            var.array_val = right.array_val;
            var.type = VALUE_ARRAY;
            var.array_val.generic_type = strdup(right.array_val.generic_type);
        }

        add_variable(scope, var);
        return result;
    }

    case NODE_NULL:
        result.type = VALUE_NULL;
        result.null_val = node->null.value;
        return result;

    case NODE_BOOL:
        result.type = VALUE_BOOL;
        result.bool_val = node->boolean.value;
        return result;

    default:
        printf("Node Type %d\n", node->type);
        raise_error("Error: unsupported node type", "");
    }

    return result;
}

void print_value(Value v)
{

    if (v.type == VALUE_NULL)
    {
        printf("<null>\n");
        return;
    }

    switch (v.type)
    {
    case VALUE_INT:
        printf("%d\n", v.int_val);
        break;
    case VALUE_FLOAT:
        printf("%f\n", v.float_val);
        break;
    case VALUE_BOOL:
        printf("%s\n", v.bool_val ? "true" : "false");
        break;
    case VALUE_STRING:
        printf("%s\n", v.str_val);
        break;
    case VALUE_ARRAY:
        printf("[");
        for (int i = 0; i < v.array_val.length; i++)
        {
            print_value(*v.array_val.elements[i]);
            if (i < v.array_val.length - 1)
                printf(", ");
        }
        printf("]\n");
        break;
    default:
        printf("<unknown value>\n");
    }
}
