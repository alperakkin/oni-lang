#pragma once
#include "ast.h"
#include "value.h"
#include "variable.h"

Value interpret(Node *node, Scope *scope);
void print_value(Value v);
