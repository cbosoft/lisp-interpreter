#pragma once
#include "types.h"

LispObject *add(LispObject *arg, struct LispEnvironment *env);
LispObject *subtract(LispObject *arg, struct LispEnvironment *env);
LispObject *multiply(LispObject *arg, struct LispEnvironment *env);
LispObject *divide(LispObject *arg, struct LispEnvironment *env);
LispObject *quote(LispObject *arg, struct LispEnvironment *env);
LispObject *define(LispObject *arg, struct LispEnvironment *env);
