#include <stdio.h>
#include <stdlib.h>
#include "eval.h"
#include "value.h"
#include "utils.h"
#include "builtins.h"

Value eval(Node *node)
{
    Value result;
    result.type = VALUE_NONE;

    if (node == NULL)
        raise_error("There is known eval node", "");
    switch (node->type)
    {
    case NODE_INTEGER:
    {

        result.type = VALUE_INT;
        result.int_val = node->integer.value;
        return result;
    }
    case NODE_BINARY_OP:
    {
        Value left = eval(node->binary_op.left);
        Value right = eval(node->binary_op.right);

        if (left.type == VALUE_INT && right.type == VALUE_INT)
        {
            result.type = VALUE_INT;

            switch (node->binary_op.token->type)
            {
            case TOKEN_PLUS:
                result.int_val = left.int_val + right.int_val;
                break;
            case TOKEN_MINUS:
                result.int_val = left.int_val - right.int_val;
                break;
            case TOKEN_STAR:
                result.int_val = left.int_val * right.int_val;
                break;
            default:
                raise_error("Error: unknown binary operator", "");
            }
            return result;
        }
        else
        {
            raise_error("Error: unsupported operand type", "");
        }
    }
    case NODE_FUNCTION_CALL:
    {
        Value left = eval(node->func_call.left);
        Value right = eval(node->func_call.right);
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
