#include <stdio.h>
#include <string.h>
#include "builtins.h"

BuiltinFunction builtin_func[] = {
    {"print", builtin_print},
    {NULL, NULL}};

Value builtin_print(Value *args, int arg_count)
{
    for (int i = 0; i < arg_count; ++i)
    {
        if (args[i].type == VALUE_INT)
        {
            printf("%d", args[i].int_val);
        }
        else if (args[i].type == VALUE_STRING)
        {
            printf("%s", args[i].str_val);
        }

        printf(" ");
    }
    printf("\n");
    Value v;
    v.type = VALUE_NONE;
    return v;
}

BuiltinFunction *find_builtin(const char *name)
{
    for (int i = 0; builtin_func[i].name != NULL; ++i)
    {
        if (strcmp(builtin_func[i].name, name) == 0)
        {
            return &builtin_func[i];
        }
    }
    return NULL;
}
