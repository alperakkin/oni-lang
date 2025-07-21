// value.h
#pragma once
#include <stdbool.h>

typedef enum
{
    VALUE_INT = 2000,       // 2000
    VALUE_STRING,           // 2001
    VALUE_FLOAT,            // 2002
    VALUE_VARIABLE,         // 2003
    VALUE_BOOL,             // 2004
    VALUE_NULL,             // 2005
    VALUE_ARRAY,            // 2006
    VALUE_CONTROL_BREAK,    // 2007
    VALUE_CONTROL_CONTINUE, // 2008
    VALUE_RETURN,           // 2009
    VALUE_FUNCTION,         // 2010
} ValueType;

typedef struct Value Value;

typedef struct ValueArray
{
    Value **elements;
    int length;
    int capacity;
    char *generic_type;
} ValueArray;

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
    Node *return_type;
} ValueFunction;
struct Value
{
    ValueType type;
    char *name;
    union
    {
        int int_val;
        char *str_val;
        float float_val;
        bool bool_val;
        bool null_val;
        ValueArray array_val;
        ValueFunction *func_val;
    };
};
