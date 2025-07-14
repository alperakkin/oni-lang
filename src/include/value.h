// value.h
#pragma once
#include <stdbool.h>

typedef enum
{
    VALUE_INT,              // 0
    VALUE_STRING,           // 1
    VALUE_FLOAT,            // 2
    VALUE_VARIABLE,         // 3
    VALUE_BOOL,             // 4
    VALUE_NULL,             // 5
    VALUE_ARRAY,            // 6
    VALUE_CONTROL_BREAK,    // 7
    VALUE_CONTROL_CONTINUE, // 8
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
    char *name;
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
