#ifndef BUILTINS_H
#define BUILTINS_H

#include "eval.h"

typedef enum
{
    PRINT_FUNCTION,
} BuiltinType;

typedef struct
{
    const char *name;
    Value (*func)(Value *args, int arg_count);
} BuiltinFunction;

extern BuiltinFunction builtin_func[];

Value builtin_print(Value *args, int arg_count);
BuiltinFunction *find_builtin(const char *name);

#endif
