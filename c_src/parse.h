#pragma once
#include "types.h"

LispListElement *parse(LispToken *tokens);
int parser_string_is_int(char *s);
int parser_string_is_float(char *s);
void parser_init();
