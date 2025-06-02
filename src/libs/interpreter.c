#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "value.h"
#include "utils.h"
#include "builtins.h"

Value interpret(Node *node)
{
    Value result;
    result.type = VALUE_NONE;

    if (node == NULL)
        raise_error("There is unknown eval node", "");
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
        Value left = interpret(node->binary_op.left);
        Value right = interpret(node->binary_op.right);
        int is_float = (left.type == VALUE_FLOAT || right.type == VALUE_FLOAT);

        if (!is_float)
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
        Value left = interpret(node->func_call.left);
        Value right = interpret(node->func_call.right);
        BuiltinFunction *fn = find_builtin(left.str_val);
        if (!fn)
            raise_error("Error: function not found", left.str_val);
        return fn->func(&right, 1);
    }
    case NODE_IDENTIFIER:
    {
        result.str_val = node->identifier.value;
        return result;
    }
    case NODE_STRING:
    {
        result.type = VALUE_STRING;
        result.str_val = node->string.value;
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
    case VALUE_NONE:
        printf("\n");
        break;
    case VALUE_STRING:
        printf("%d\n", v.int_val);
        break;
    default:
        printf("<unknown value>\n");
    }
}
