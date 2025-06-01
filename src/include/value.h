#pragma once
typedef enum
{
    VALUE_INT,
    VALUE_STRING,
    VALUE_FLOAT,
    VALUE_NONE
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        int int_val;
        char *str_val;
        float float_val;
    };
} Value;