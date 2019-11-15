#pragma once
#include "object.h"
#include "environment.h"

LispObject *eval(LispObject *ast, LispEnvironment *env);
