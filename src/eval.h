#pragma once
#include "object.h"
#include "environment.h"

LispObject *eval(LispObject *ast, LispEnvironment *env);
LispObject *eval_file(char *filename, LispEnvironment *env);
LispObject *eval_string(char *s, LispEnvironment *env);
