#pragma once
#include "object.h"

typedef struct LispEnvironment LispEnvironment;
struct LispEnvironment {
  LispObject *symbols;
  LispEnvironment *parent;
};

LispEnvironment *LispEnvironment_new_environment(LispEnvironment *parent);
void LispEnvironment_add_symbol(LispEnvironment *env, LispObject *value);
void LispEnvironment_free(LispEnvironment *env);
LispObject *LispEnvironment_get_symbol(LispEnvironment *env, char *name);
