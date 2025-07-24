#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variable.h"
#include "utils.h"
#include "builtins.h"

void add_variable(Scope *scope, Value *var)
{

    Value *new_vars = realloc(scope->variables, sizeof(Value) * (scope->count + 1));
    if (new_vars == NULL)
    {
        fprintf(stderr, "Memory allocation failed in add_variable\n");
        exit(1);
    }

    scope->variables = new_vars;
    scope->variables[scope->count].type = var->type;
    scope->variables[scope->count].name = strdup(var->name);

    switch (var->type)
    {
    case VALUE_INT:
        scope->variables[scope->count].int_val = var->int_val;
        break;
    case VALUE_FLOAT:
        scope->variables[scope->count].float_val = var->float_val;
        break;
    case VALUE_STRING:
        scope->variables[scope->count].str_val = strdup(var->str_val);
        break;
    case VALUE_BOOL:
        scope->variables[scope->count].bool_val = var->bool_val;
        break;
    case VALUE_ARRAY:
        scope->variables[scope->count].array_val.length = var->array_val.length;
        scope->variables[scope->count].array_val.capacity = var->array_val.capacity;
        scope->variables[scope->count].array_val.generic_type = strdup(var->array_val.generic_type);
        int len = var->array_val.length;
        scope->variables[scope->count].array_val.elements = malloc(sizeof(var->array_val) * len);
        if (scope->variables[scope->count].array_val.elements == NULL)
            raise_error("Memory allocation failed for array elements\n", "");

        for (int i = 0; i < len; i++)
        {
            Value *src_val = var->array_val.elements[i];

            Value *new_val = malloc(sizeof(Value));
            if (!new_val)
                raise_error("Memory allocation failed for array element\n", "");

            *new_val = *src_val;

            if (new_val->type == VALUE_STRING)
            {
                new_val->str_val = strdup(src_val->str_val);
            }
            else if (new_val->type == VALUE_INT)
            {
                new_val->int_val = src_val->int_val;
            }
            else if (new_val->type == VALUE_FLOAT)
            {
                new_val->float_val = src_val->float_val;
            }
            else if (new_val->type == VALUE_BOOL)
            {
                new_val->bool_val = src_val->bool_val;
            }
            else if (new_val->type == VALUE_ARRAY)
            {
            }
            scope->variables[scope->count]
                .array_val.elements[i] = new_val;
        }
        break;

    case VALUE_FUNCTION:
    {

        ValueFunction *src_func = var->func_val;
        ValueFunction *new_func = malloc(sizeof(ValueFunction));
        if (!new_func)
            raise_error("Memory allocation failed for function\n", "");

        *new_func = *src_func;
        new_func->name = strdup(src_func->name);

        scope->variables[scope->count].func_val = new_func;

        break;
    }
    case VALUE_CLASS:
    case VALUE_OBJ:
    {

        ValueObject *src = var->obj_val;
        ValueObject *copy = malloc(sizeof(ValueObject));
        if (!copy)
            raise_error("Memory allocation failed", "");

        copy->name = strdup(src->name);
        copy->attr_count = src->attr_count;

        copy->attrs = malloc(sizeof(Value *) * src->attr_count);
        if (!copy->attrs)
            raise_error("Memory allocation failed for object attributes", "");

        for (int i = 0; i < src->attr_count; i++)
        {
            Value *attr = src->attrs[i];
            if (!attr)
            {
                copy->attrs[i] = NULL;
                continue;
            }

            Value *copied_attr = malloc(sizeof(Value));
            if (!copied_attr)
                raise_error("Memory allocation failed for attribute", "");

            *copied_attr = *attr;

            if (attr->type == VALUE_STRING)
                copied_attr->str_val = strdup(attr->str_val);
            else if (attr->type == VALUE_OBJ || attr->type == VALUE_CLASS)
            {

                *copied_attr = copy_value(attr);
            }

            copy->attrs[i] = copied_attr;
        }
        copy->method_count = src->method_count;
        copy->methods = malloc(sizeof(ValueFunction *) * copy->method_count);
        for (int i = 0; i < copy->method_count; i++)
        {
            copy->methods[i] = copy_function(src->methods[i]);
        }

        scope->variables[scope->count].obj_val = copy;

        break;
    }
    case VALUE_NULL:
        break;
    default:
        raise_error("Can not assign variable", var->name);
    }
    scope->count++;
}

void register_builtins(Scope *global_scope)
{
    Value builtin = create_builtin_function("print", builtin_print);

    add_variable(global_scope, &builtin);
}

ValueFunction *copy_function(ValueFunction *src)
{
    ValueFunction *copy = malloc(sizeof(ValueFunction));
    *copy = *src;
    copy->name = strdup(src->name);
    return copy;
}

int get_variable(Scope *scope, char *var_name, Scope **found)
{
    Scope *current = scope;
    while (current != NULL)
    {

        for (int index = 0; index < current->count; index++)
        {
            Value var = current->variables[index];

            if (strcmp(var.name, var_name) == 0)
            {
                *found = current;
                return index;
            }
        }
        current = current->parent;
    }
    return -1;
}

Scope *init_scope(Scope *parent)
{
    Scope *scope = malloc(sizeof(Scope));
    scope->variables = NULL;
    scope->count = 0;
    scope->parent = parent;
    return scope;
}

void print_scope(Scope *scope, char *name)
{

    printf("%s = {\n", name);
    for (int i = 0; i < scope->count; i++)
    {
        Value var = scope->variables[i];
        if (var.name == NULL)
            raise_error("Undefined Variable Name", "");

        if (var.type == VALUE_STRING)
        {
            printf("    %s (str): ", var.name);
            printf("\"%s\",\n", var.str_val);
        }
        else if (var.type == VALUE_FLOAT)
        {
            printf("    %s (float): ", var.name);
            printf("%f,\n", var.float_val);
        }
        else if (var.type == VALUE_INT)
        {
            printf("    %s (int): ", var.name);
            printf("%d,\n", var.int_val);
        }
        else if (var.type == VALUE_BOOL)
        {
            printf("    %s (bool): ", var.name);
            printf("%s,\n", var.bool_val == 1 ? "true" : "false");
        }
        else if (var.type == VALUE_FUNCTION)
        {
            printf("    %s (function): ", var.name);
            printf("<%s>,\n", var.name);
        }
        else if (var.type == VALUE_CLASS)
        {
            printf("    %s (class): ", var.name);
            printf("<%s>,\n", var.name);
        }
        else if (var.type == VALUE_OBJ)
        {
            printf("    %s (object): ", var.name);
            printf("<%s>,\n", var.name);
        }
        else if (var.type == VALUE_NULL)
        {
            printf("    %s (null): ", var.name);
            printf("%s,\n", "<null>");
        }
    }
    printf("}\n");
}

Value copy_value(Value *original)
{
    Value copy;
    copy.type = original->type;
    copy.name = strdup(original->name);

    switch (original->type)
    {
    case VALUE_INT:
        copy.int_val = original->int_val;
        break;
    case VALUE_FLOAT:
        copy.float_val = original->float_val;
        break;
    case VALUE_BOOL:
        copy.bool_val = original->bool_val;
        break;
    case VALUE_STRING:
        copy.str_val = original->str_val ? strdup(original->str_val) : NULL;
        break;
    case VALUE_FUNCTION:
        copy.func_val = copy_function(original->func_val);
        break;
    case VALUE_OBJ:
    case VALUE_CLASS:
        copy = create_instance(original);
        break;
    case VALUE_ARRAY:
        copy.array_val.length = original->array_val.length;
        copy.array_val.capacity = original->array_val.capacity;
        copy.array_val.generic_type = strdup(original->array_val.generic_type);
        copy.array_val.elements = malloc(sizeof(Value *) * original->array_val.length);
        for (int i = 0; i < original->array_val.length; i++)
        {
            Value *element = malloc(sizeof(Value));
            *element = copy_value(original->array_val.elements[i]);
            copy.array_val.elements[i] = element;
        }
        break;
    case VALUE_NULL:
    default:
        break;
    }

    return copy;
}

Value create_instance(ValueObject *class)
{

    Value *obj = malloc(sizeof(Value));
    obj->obj_val = malloc(sizeof(ValueObject));
    obj->type = VALUE_OBJ;
    obj->obj_val->attr_count = class->attr_count;
    obj->obj_val->attrs = malloc(sizeof(Value *) * class->attr_count);

    for (int i = 0; i < class->attr_count; i++)
    {
        Value *element = malloc(sizeof(Value));
        *element = copy_value(class->attrs[i]);
        obj->obj_val->attrs[i] = element;
    }

    char *instance_name = malloc(strlen("instance_") + strlen(class->name) + 32);
    sprintf(instance_name, "<class object -> %s>", class->name);
    obj->name = instance_name;
    obj->obj_val->name = instance_name;

    obj->obj_val->method_count = class->method_count;
    obj->obj_val->methods = malloc(sizeof(ValueFunction *) * obj->obj_val->method_count);
    for (int i = 0; i < obj->obj_val->method_count; i++)
    {
        obj->obj_val->methods[i] = copy_function(class->methods[i]);
    }

    return *obj;
}

ValueFunction *find_method(ValueObject *class, const char *method_name)
{
    for (int i = 0; i < class->method_count; i++)
    {
        if (strcmp(class->methods[i]->name, method_name) == 0)
            return class->methods[i];
    }
    return NULL;
}
