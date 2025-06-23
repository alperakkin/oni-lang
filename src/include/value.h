// value.h
#pragma once
#include <stdbool.h>

typedef enum
{
    VALUE_INT,
    VALUE_STRING,
    VALUE_FLOAT,
    VALUE_VARIABLE,
    VALUE_BOOL,
    VALUE_NULL,
    VALUE_ARRAY,
} ValueType;

typedef struct Value Value;

typedef struct ValueArray
{
    Value **elements;
    int length;
    int capacity;
    char *generic_type;
} ValueArray;

struct Value
{
    ValueType type;
    union
    {
        int int_val;
        char *str_val;
        float float_val;
        bool bool_val;
        bool null_val;
        ValueArray array_val;
    };
};
