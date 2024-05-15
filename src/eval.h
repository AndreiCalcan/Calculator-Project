#include "stack.h"
#include <stdio.h>
#include <ctype.h>
#define DT double

DT precedence(char op);
DT applyOp(DT a, DT b, char op);
DT evaluate(char *tokens);