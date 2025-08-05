#pragma once
#include "ast.h"
#include "value.h"
#include "variable.h"
#include "functions.h"

Value interpret(Node *node, Scope *scope, Value *this);
void print_value(Value *v);
