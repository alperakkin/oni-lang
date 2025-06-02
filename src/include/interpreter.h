#pragma once
#include "ast.h"
#include "value.h"

Value interpret(Node *node);
void print_value(Value v);
