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

        int index = get_variable(globals, node->unary_op.operand->identifier.value);
        if (globals->variables[index].type != VARIABLE_INT)
            raise_error("Unary operations only valid for integers", "");

        if (node->unary_op.token->type == TK_INC)
        {
            globals->variables[index].int_value++;
        }
        else if (node->unary_op.token->type == TK_DECR)
        {
            globals->variables[index].int_value--;
        }
        break;
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
                globals->variables[index].type = VARIABLE_FLOAT;
                break;
            case VALUE_INT:
                globals->variables[index].int_value = right.int_val;
                globals->variables[index].type = VARIABLE_INT;
                break;
            case VALUE_STRING:
                free(globals->variables[index].string_value);
                globals->variables[index].string_value = strdup(right.str_val);
                globals->variables[index].type = VARIABLE_STR;
                break;
            case VALUE_BOOL:
                globals->variables[index].bool_value = right.bool_val;
                globals->variables[index].type = VARIABLE_BOOL;
                break;
            case VALUE_NULL:
                globals->variables[index].type = VARIABLE_NULL;
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

        Value condition = interpret(node->node_if.condition, globals);
        if (condition.bool_val == true)
        {
            for (int i = 0; i < node->node_if.if_block->count; i++)
            {
                result = interpret(node->node_if.if_block->statements[i], globals);
            }
        }
        else
        {
            if (node->node_if.else_block)
            {

                for (int i = 0; i < node->node_if.else_block->count; i++)
                {
                    result = interpret(node->node_if.else_block->statements[i], globals);
                }
            }
        }
        return result;
    }
    case NODE_WHILE:
    {

        Value condition = interpret(node->node_while.condition, globals);
        while (condition.bool_val != true)
        {
            for (int i = 0; i < node->node_while.if_block->count; i++)
            {
                result = interpret(node->node_while.if_block->statements[i], globals);
            }
            condition = interpret(node->node_while.condition, globals);
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
            *val = interpret(node->array.elements[i], globals);
            result.array_val.elements[i] = val;
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
            result.type = VALUE_STRING;
            result.str_val = node->identifier.value;
            return result;
        }

        Variable var = globals->variables[index];

        switch (var.type)
        {
        case VARIABLE_INT:
            result.type = VALUE_INT;
            result.int_val = var.int_value;
            break;
        case VARIABLE_FLOAT:
            result.type = VALUE_FLOAT;
            result.float_val = var.float_value;
            break;
        case VARIABLE_BOOL:
            result.type = VALUE_BOOL;
            result.bool_val = var.bool_value;
            break;
        case VARIABLE_STR:
            result.type = VALUE_STRING;
            result.str_val = strdup(var.string_value);
            break;
        case VARIABLE_ARRAY:
            result.type = VALUE_ARRAY;
            result.array_val = var.array_value;

            if (var.array_value.generic_type)
                result.array_val.generic_type = strdup(var.array_value.generic_type);

            break;
        case VARIABLE_NULL:
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
        Variable var = {0};
        if (!node->variable.name)
            raise_error("Variable Name not assigned", "");
        var.name = strdup(node->variable.name);
        Value right = interpret(node->variable.value, globals);

        if (strcmp(node->variable.type, "int") == 0)
        {
            if (right.type == VALUE_NULL)
                var.type = VARIABLE_NULL;
            else
            {
                var.int_value = (right.type == VALUE_FLOAT) ? (int)right.float_val : right.int_val;
                var.type = VARIABLE_INT;
            }
        }
        else if (strcmp(node->variable.type, "str") == 0)
        {
            var.string_value = strdup(right.str_val);
            var.type = VARIABLE_STR;
        }
        else if (strcmp(node->variable.type, "float") == 0)
        {
            var.float_value = (right.type == VALUE_INT) ? (float)right.int_val : right.float_val;
            var.type = VARIABLE_FLOAT;
        }
        else if (strcmp(node->variable.type, "bool") == 0)
        {
            var.bool_value = right.bool_val;
            var.type = VARIABLE_BOOL;
        }
        else if (strcmp(node->variable.type, "arr") == 0)
        {
            var.array_value = right.array_val;
            var.type = VARIABLE_ARRAY;
            var.array_value.generic_type = strdup(right.array_val.generic_type);
        }

        add_variable(globals, var);
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
