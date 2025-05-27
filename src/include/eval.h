#include "ast.h"
typedef enum
{
    VALUE_INT,
    VALUE_NONE
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        int int_val;
    };
} Value;

Value eval(Node *node);
void print_value(Value v);