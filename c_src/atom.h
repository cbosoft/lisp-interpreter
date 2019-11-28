#pragma once
#include "types.h"

LispAtom *LispAtom_new(long *value_int, double *value_float, char *value_string);
char *LispAtom_repr(LispAtom *a);
int LispAtom_is_truthy(LispAtom *a);

int LispAtom_gt(LispAtom *left, LispAtom *right);
int LispAtom_ge(LispAtom *left, LispAtom *right);
int LispAtom_lt(LispAtom *left, LispAtom *right);
int LispAtom_le(LispAtom *left, LispAtom *right);
int LispAtom_eq(LispAtom *left, LispAtom *right);
void LispAtom_cast_together(LispAtom *left, LispAtom *right, LispAtom **cleft, LispAtom **cright);
LispAtom *LispAtom_cast_as(LispAtom *a, int type);
