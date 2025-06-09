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
        switch (args[i].type)
        {
        case VALUE_INT:
            printf("%d", args[i].int_val);
            break;
        case VALUE_FLOAT:
            printf("%f", args[i].float_val);
            break;
        case VALUE_STRING:
            printf("%s", args[i].str_val);
            break;
        case VALUE_BOOL:
            printf("%s", args[i].bool_val == 1 ? "true" : "false");
            break;
        case VALUE_NULL:
            printf("\n");
            break;
        default:
            break;
        }
        printf(" ");
    }
    printf("\n");
    Value v;
    v.type = VALUE_NULL;
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
