#include <stdio.h>
#include <stdlib.h>
#include "eval.h"
#include "utils.h"

Value eval(Node *node)
{
    if (node == NULL)
        raise_error("There is known eval node", "");
    switch (node->type)
    {
    case NODE_INTEGER:
    {
        Value v;
        v.type = VALUE_INT;
        v.int_val = node->integer.value;
        return v;
    }
    case NODE_BINARY_OP:
    {
        Value left = eval(node->binary_op.left);
        Value right = eval(node->binary_op.right);

        if (left.type == VALUE_INT && right.type == VALUE_INT)
        {
            Value result;
            result.type = VALUE_INT;

            switch (node->binary_op.token->type)
            {
            case TOKEN_PLUS:
                result.int_val = left.int_val + right.int_val;
                break;
            case TOKEN_MINUS:
                result.int_val = left.int_val - right.int_val;
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

    default:
        raise_error("Error: unsupported node type", "");
    }
}

void print_value(Value v)
{
    switch (v.type)
    {
    case VALUE_INT:
        printf("%d\n", v.int_val);
        break;

    default:
        printf("<unknown value>\n");
    }
}
