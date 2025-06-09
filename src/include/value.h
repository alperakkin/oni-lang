#pragma once
#include <stdbool.h>
typedef enum
{
    VALUE_INT,
    VALUE_STRING,
    VALUE_FLOAT,
    VALUE_VARIABLE,
    VALUE_BOOL,
    VALUE_NULL
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        int int_val;
        char *str_val;
        float float_val;
        bool bool_val;
        bool null_val;
    };
} Value;