#pragma once
#include "ast.h"
#include "value.h"

Value eval(Node *node);
void print_value(Value v);
