#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "value.h"
#include "utils.h"
#include "builtins.h"

Value interpret(Node *node, GlobalScope *globals)
{
    Value result;
    result.type = VALUE_NULL;

    // if (node == NULL)
    //     raise_error("There is unknown eval node", "");

    switch (node->type)
    {
    case NODE_NUMBER:
    {

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
    }

    case NODE_BINARY_OP:
    {
        Value left = interpret(node->binary_op.left, globals);
        Value right = interpret(node->binary_op.right, globals);
        int is_float = (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT);
        int assignment = node->binary_op.token->type == TK_ASSIGN;
        if (assignment)
        {

            int index = get_variable(globals, node->binary_op.left->identifier.value);

            switch (right.type)
            {
            case VALUE_FLOAT:
                globals->variables[index].float_value = right.float_val;
                break;
            case VALUE_INT:
                globals->variables[index].int_value = right.int_val;
                break;
            case VALUE_STRING:
                globals->variables[index].string_value = strdup(right.str_val);
                break;
            case VALUE_BOOL:
                globals->variables[index].bool_value = right.bool_val;
                break;
            case VALUE_NULL:
                globals->variables[index].null_value = right.null_val;
                break;
            default:
                break;
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
            default:
                raise_error("Unknown binary operator", "");
            }
        }
        else if (is_float)
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
            default:
                raise_error("Unknown binary operator", "");
            }
        }
        else
        {
            raise_error("Unsupported right operand type", "");
        }
        return result;
    }
    case NODE_FUNCTION_CALL:
    {
        Value left = interpret(node->func_call.left, globals);
        Value right = interpret(node->func_call.right, globals);
        BuiltinFunction *fn = find_builtin(left.str_val);
        if (!fn)
            raise_error("Error: function not found", left.str_val);
        return fn->func(&right, 1);
    }
    case NODE_IDENTIFIER:
    {
        int index = get_variable(globals, node->identifier.value);

        if (index == -1)
        {
            result.str_val = node->identifier.value;
            return result;
        }

        Variable var = globals->variables[index];

        if (var.type == VARIABLE_INT)
        {

            result.type = VALUE_INT;
            result.int_val = var.int_value;
            return result;
        }
        if (var.type == VARIABLE_FLOAT)
        {
            result.type = VALUE_FLOAT;
            result.float_val = var.float_value;
            return result;
        }
        if (var.type == VARIABLE_BOOL)
        {
            result.type = VALUE_BOOL;
            result.bool_val = var.bool_value;
            return result;
        }
        if (var.type == VARIABLE_STR)
        {
            result.type = VALUE_STRING;
            result.str_val = strdup(var.string_value);

            return result;
        }
        return result;
    }
    case NODE_STRING:
    {
        result.type = VALUE_STRING;
        result.str_val = node->string.value;
        return result;
    }
    case NODE_VARIABLE:
        result.type = VALUE_VARIABLE;
        Variable var = {0};
        if (!node->variable.name)
            raise_error("Variable Name not assigned", "");
        var.name = strdup(node->variable.name);

        Value right = interpret(node->variable.value, globals);

        if (strcmp(node->variable.type, "int") == 0)
        {
            int int_val = right.type == VALUE_FLOAT ? (int)right.float_val : right.int_val;
            var.int_value = int_val;
            var.type = VARIABLE_INT;
        }
        if (strcmp(node->variable.type, "str") == 0)
        {
            var.string_value = strdup(right.str_val);
            var.type = VARIABLE_STR;
        }
        if (strcmp(node->variable.type, "float") == 0)
        {
            float float_val = right.type == VALUE_INT ? (float)right.int_val : right.float_val;

            var.float_value = float_val;
            var.type = VARIABLE_FLOAT;
        }
        if (strcmp(node->variable.type, "bool") == 0)
        {
            var.type = VARIABLE_BOOL;
            var.bool_value = right.bool_val;
        }
        add_variable(globals, var);

        return result;
    case NODE_NULL:
    {
        result.type = VALUE_NULL;
        result.null_val = node->null.value;
        return result;
    }
    case NODE_BOOL:
    {
        result.type = VALUE_BOOL;
        result.bool_val = node->boolean.value;
        return result;
    }
    default:
        printf("Node Type %d\n", node->type);
        raise_error("Error: unsupported node type", "");
    }
    return result;
}

void print_value(Value v)
{
    switch (v.type)
    {
    case VALUE_INT:
        printf("%d\n", v.int_val);
        break;
    case VALUE_NULL:
        printf("\n");
        break;
    case VALUE_BOOL:
        printf("%s", v.bool_val == true ? "true" : false);
    case VALUE_STRING:
        printf("%d\n", v.int_val);
        break;
    default:
        printf("<unknown value>\n");
    }
}
