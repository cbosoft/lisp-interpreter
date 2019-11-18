#pragma once

#include "types.h"

LispObject *LispObject_new_string(char *value);
LispObject *LispObject_new_int(int value);
LispObject *LispObject_new_float(double value);
LispObject *LispObject_new_bool(int value);
LispObject *LispObject_new_list();
LispObject *LispObject_new_guess_type(char *s);
void LispObject_assign_value(LispObject *dest, LispObject *source);
void LispObject_print(LispObject *o);
void LispObject_free(LispObject *root);
char *LispObject_type(LispObject *o);
char *LispObject_repr(LispObject *o);
