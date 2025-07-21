#ifndef BUILTINS_H
#define BUILTINS_H

#include "interpreter.h"

typedef enum
{
    PRINT_FUNCTION,
} BuiltinType;

Value builtin_print(Value *args, Value *kwargs, int arg_count, int kwarg_count);

#endif
