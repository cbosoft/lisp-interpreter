#pragma once

#include "types.h"

LispObject *LispObject_new_nil();
LispObject *LispObject_new_string(char *value);
LispObject *LispObject_new_symbol(char *value);
LispObject *LispObject_new_int(long value);
LispObject *LispObject_new_float(double value);
LispObject *LispObject_new_list();

void LispObject_assign_value(LispObject *dest, LispObject *source);
void LispObject_print(LispObject *o);
void LispObject_free(LispObject *root);
char *LispObject_type(LispObject *o);
char *LispObject_repr(LispObject *o);
int LispObject_is_truthy(LispObject *o);
int LispObject_gt(LispObject *left, LispObject *right);
int LispObject_ge(LispObject *left, LispObject *right);
int LispObject_lt(LispObject *left, LispObject *right);
int LispObject_le(LispObject *left, LispObject *right);
int LispObject_eq(LispObject *left, LispObject *right);
int LispObject_or(LispObject *left, LispObject *right);
int LispObject_and(LispObject *left, LispObject *right);