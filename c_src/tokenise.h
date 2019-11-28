#pragma once
#include "types.h"

LispToken *tokenise(char *input, char *source);
int parenscheck(char *input);
