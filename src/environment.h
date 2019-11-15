#pragma once
#include "object.h"

typedef struct LispEnvironment LispEnvironment;
struct LispEnvironment {
  LispObject *symbols;
  LispEnvironment *parent;
};

LispEnvironment *environment_new_environment(LispEnvironment *parent);
void environment_add_symbol(LispEnvironment *env, LispObject *value);
LispObject *environment_get_symbol(LispEnvironment *env, char *name);
