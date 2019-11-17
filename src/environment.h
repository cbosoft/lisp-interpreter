#pragma once
#include "types.h"

LispEnvironment *LispEnvironment_new_global_environment();
LispEnvironment *LispEnvironment_new_environment(LispEnvironment *parent);
void LispEnvironment_add_variable(LispEnvironment *env, char *name, LispFunction *lfunc, LispBuiltin *bfunc, LispObject *value);
void LispEnvironment_add_variable_with_alias(LispEnvironment *env, char *name, char *alias, LispFunction *lfunc, LispBuiltin *bfunc, LispObject *value);
void LispEnvironment_free(LispEnvironment *env);
int LispEnvironment_get(LispEnvironment *env, char *name, LispFunction **lfunc, LispBuiltin **bfunc, LispObject **lobj);
LispObject *LispEnvironment_get_variable(LispEnvironment *env, char *name);
LispFunction *LispEnvironment_get_lispfunction(LispEnvironment *env, char *name);
LispBuiltin *LispEnvironment_get_builtinfunction(LispEnvironment *env, char *name);
